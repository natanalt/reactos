/*
 * PROJECT:     ReactOS NVM Express Miniport Driver
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     Controller and namespace identification
 * COPYRIGHT:   Copyright 2026 Justin Miller <justin.miller@reactos.org>
 *              Copyright 2026 Natalia Cholewa <natanalt@proton.me>
 */

/* INCLUDES *******************************************************************/

#include "stornvme.h"

#define NDEBUG
#include <debug.h>


/* FUNCTIONS ******************************************************************/

/**
 * @brief Issues Identify with the given CNS into the scratch page.
 */
static
BOOLEAN
NvmpIdentify(
    _In_ PNVME_ADAPTER_EXTENSION Adapter,
    _In_ ULONG NamespaceId,
    _In_ UCHAR ControllerOrNamespace)
{
    NVME_COMMAND Command;

    RtlZeroMemory(&Command, sizeof(Command));
    RtlZeroMemory(Adapter->ScratchBuffer, PAGE_SIZE);

    Command.CDW0.OPC = NVME_ADMIN_COMMAND_IDENTIFY;
    Command.NSID = NamespaceId;
    Command.PRP1 = Adapter->ScratchAddress.QuadPart;
    Command.u.IDENTIFY.CDW10.CNS = ControllerOrNamespace;

    return NvmpIssueAdminCommand(Adapter, &Command, NULL);
}


/**
 * @brief Converts MDTS to bytes, capped at NVME_MAX_TRANSFER_LENGTH.
 *
 * MDTS is a power of two in units of CAP.MPSMIN, 0 means no limit.
 */
static
ULONG
NvmpMaximumTransfer(
    _In_ PNVME_ADAPTER_EXTENSION Adapter,
    _In_ PNVME_IDENTIFY_CONTROLLER_DATA Data)
{
    ULONG MinimumPage;
    ULONG Length;

    if (Data->MDTS == 0 || Data->MDTS >= 32)
        return NVME_MAX_TRANSFER_LENGTH;

    MinimumPage = 1UL << (NVME_MIN_PAGE_SHIFT + Adapter->Capabilities.MPSMIN);

    /* Avoid overflowing the shift */
    if (Data->MDTS >= 31 || (ULONG)Data->MDTS > 31 - NVME_MIN_PAGE_SHIFT)
        return NVME_MAX_TRANSFER_LENGTH;

    Length = MinimumPage << Data->MDTS;
    if (Length > NVME_MAX_TRANSFER_LENGTH)
        Length = NVME_MAX_TRANSFER_LENGTH;

    return Length;
}

/**
 * @brief Scans the controller for exact information about DSM support.
 * 
 * IMPORTANT: This overwrites the scratch buffer, as an additional Identify command call is
 * needed to get exact DSM limits.
 */
static
BOOLEAN
NvmpCheckForDsmSupport(
    _In_ PNVME_ADAPTER_EXTENSION Adapter,
    _In_ PNVME_IDENTIFY_CONTROLLER_DATA Data)
{
    PNVME_IDENTIFY_NVM_SPECIFIC_CONTROLLER_IO_COMMAND_SET DataCns06;
    BOOLEAN DatasetManagement;

    /*
     * Simply looking at ONCS.DatasetManagement is not enough, as DSM may be enabled even if the
     * value is 0.
     * 
     * If ONCS.DatasetManagement is 0, the DMRL, DMRSL and DMSL fields being nonzero indicates
     * valid DSM support, and the absolute maximum values for these limits. If that field is 1,
     * these limits should still be polled, but the spec implies they're just suggestions then.
     * 
     * Either way, we use DMRL/DMRSL/DMSL as absolute maxima, so we need to fetch them via a second
     * Identify call.
     */

    DatasetManagement = Data->ONCS.DatasetManagement;

    if (!NvmpIdentify(Adapter, 0, NVME_IDENTIFY_CNS_SPECIFIC_CONTROLLER_IO_COMMAND_SET))
    {
        DPRINT1("Failed to Identify CNS 06h\n");
        return FALSE;
    }
    
    DataCns06 = Adapter->ScratchBuffer;
    
    if (DatasetManagement || (Adapter->DsmMaxRanges != 0 &&
        Adapter->DsmMaxLbasOnSingleRange != 0 &&
        Adapter->DsmMaxTotalLbas != 0))
    {
        Adapter->DsmSupported = TRUE;
        Adapter->DsmMaxLbasOnSingleRange = DataCns06->DMRSL;
        Adapter->DsmMaxTotalLbas = DataCns06->DMSL;
        
        /* Apply our own maximum of exactly one page (256 ranges @ 4K pages) */
        /* This is also the NVMe maximum (1 byte for NR in CD10 for the command) */
        Adapter->DsmMaxRanges = min(DataCns06->DMRL, PAGE_SIZE / sizeof(NVME_LBA_RANGE));
        if (Adapter->DsmMaxRanges == 0)
            Adapter->DsmMaxRanges = (UCHAR)(PAGE_SIZE / sizeof(NVME_LBA_RANGE));
    }

    return TRUE;
}


BOOLEAN
NvmpIdentifyController(
    _In_ PNVME_ADAPTER_EXTENSION Adapter)
{
    PNVME_IDENTIFY_CONTROLLER_DATA Data;

    if (!NvmpIdentify(Adapter, 0, NVME_IDENTIFY_CNS_CONTROLLER))
    {
        DPRINT1("Identify controller failed\n");
        return FALSE;
    }

    Data = Adapter->ScratchBuffer;

    Adapter->VendorId = Data->VID;
    Adapter->PowerStateCount = Data->NPSS;
    Adapter->NamespaceCount = Data->NN;
    Adapter->VolatileWriteCache = (Data->VWC.Present != 0);
    Adapter->MaximumTransferLength = NvmpMaximumTransfer(Adapter, Data);

    RtlCopyMemory(Adapter->SerialNumber, Data->SN, sizeof(Adapter->SerialNumber));
    RtlCopyMemory(Adapter->ModelNumber, Data->MN, sizeof(Adapter->ModelNumber));
    RtlCopyMemory(Adapter->FirmwareRevision, Data->FR, sizeof(Adapter->FirmwareRevision));

    DPRINT1("Controller has %lu namespaces, transfers up to %lu bytes%s\n",
            Adapter->NamespaceCount,
            Adapter->MaximumTransferLength,
            Adapter->VolatileWriteCache ? ", with a write cache" : "");
    
    DPRINT1("Dataset management is %s\n", Adapter->DsmSupported ? "on" : "off");

    if (Adapter->NamespaceCount > NVME_MAX_NAMESPACES)
        Adapter->NamespaceCount = NVME_MAX_NAMESPACES;
    
    /* Check whether we have Dataset Management command support, aka TRIM or UNMAP. */
    if (!NvmpCheckForDsmSupport(Adapter, Data))
        return FALSE;
    /* (old Data ScratchBuffer is now overwritten) */

    return TRUE;
}


BOOLEAN
NvmpIdentifyNamespace(
    _In_ PNVME_ADAPTER_EXTENSION Adapter,
    _In_ ULONG NamespaceId,
    _Out_ PNVME_NAMESPACE Namespace)
{
    PNVME_IDENTIFY_NAMESPACE_DATA Data;
    ULONG Format;

    RtlZeroMemory(Namespace, sizeof(*Namespace));

    if (!NvmpIdentify(Adapter, NamespaceId, NVME_IDENTIFY_CNS_SPECIFIC_NAMESPACE))
    {
        DPRINT1("Identify namespace %lu failed\n", NamespaceId);
        return FALSE;
    }

    Data = Adapter->ScratchBuffer;

    /* NSZE is 0 for an inactive namespace */
    if (Data->NSZE == 0)
        return FALSE;

    Format = Data->FLBAS.LbaFormatIndex;
    if (Format > Data->NLBAF)
    {
        DPRINT1("Namespace %lu names format %lu of %u\n",
                NamespaceId, Format, Data->NLBAF);
        return FALSE;
    }

    /* Metadata formats are not supported */
    if (Data->LBAF[Format].MS != 0)
    {
        DPRINT1("Namespace %lu wants %u metadata bytes per block\n",
                NamespaceId, Data->LBAF[Format].MS);
        return FALSE;
    }

    /* LBADS below 9 means the format is not supported */
    if (Data->LBAF[Format].LBADS < NVME_MIN_BLOCK_SHIFT)
    {
        DPRINT1("Namespace %lu names an unusable block size of 2^%u\n",
                NamespaceId, Data->LBAF[Format].LBADS);
        return FALSE;
    }

    Namespace->NamespaceId = NamespaceId;
    Namespace->BlockCount = Data->NSZE;
    Namespace->BlockShift = Data->LBAF[Format].LBADS;
    Namespace->BlockSize = 1UL << Namespace->BlockShift;
    Namespace->ThinProvisioning = Data->NSFEAT.ThinProvisioning;
    RtlCopyMemory(Namespace->Nguid, Data->NGUID, sizeof(Namespace->Nguid));
    RtlCopyMemory(Namespace->Eui64, Data->EUI64, sizeof(Namespace->Eui64));
    Namespace->Present = TRUE;

    if (Adapter->DsmSupported)
    {
        Namespace->WriteZeroesCanDeallocate = Data->DLFEAT.WriteZeroes;

        if (Data->DLFEAT.ReadBehavior == NVME_READ_BEHAVIOR_RETURN_ZERO)
            Namespace->DeallocatedBlocksReadZero = 1;
        
        /* TODO: Newer NVMe specs widen this to 2 bits, where NPDG may(?)
           behave a bit oddly */
        if (Data->NSFEAT.NameSpaceIoOptimization != 0)
        {
            Namespace->PreferredUnmapGranularity = Data->NPDG + 1;
            Namespace->PreferredUnmapAlignment = Data->NPDA + 1;
        }

        DPRINT1("Namespace %lu DSM parameters:\n", NamespaceId);
        DPRINT1(" - DLFEAT.WriteZeros = %d\n", Data->DLFEAT.WriteZeroes);
        DPRINT1(" - DLFEAT.ReadBehavior = %d\n", Data->DLFEAT.ReadBehavior);
        DPRINT1(" - NPDG = %d\n", Data->NPDG);
        DPRINT1(" - NPDA = %d\n", Data->NPDA);
    }

    DPRINT1("Namespace %lu holds %I64u blocks of %lu bytes\n",
            NamespaceId, Namespace->BlockCount, Namespace->BlockSize);

    return TRUE;
}


/**
 * @brief Identifies NSID 1 through NN and records the active namespaces.
 */
VOID
NvmpEnumerateNamespaces(
    _In_ PNVME_ADAPTER_EXTENSION Adapter)
{
    ULONG NamespaceId;
    ULONG Found = 0;

    for (NamespaceId = 1; NamespaceId <= Adapter->NamespaceCount; NamespaceId++)
    {
        if (NvmpIdentifyNamespace(Adapter,
                                  NamespaceId,
                                  &Adapter->Namespaces[NamespaceId - 1]))
        {
            Found++;
        }
    }

    DPRINT1("Found %lu attached namespaces\n", Found);
}
