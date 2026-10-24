/*
 * PROJECT:     ReactOS SD Host Controller Port Driver
 * LICENSE:     GPL-2.0+ (https://spdx.org/licenses/GPL-2.0+)
 * PURPOSE:     Main sources for sdport
 * COPYRIGHT:   Copyright 2026 Natalia Cholewa <natanalt@proton.me>
 */

#include "pch.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    DPRINT1("DriverEntry(%p %p)\n", DriverObject, RegistryPath);
    return STATUS_SUCCESS;
}

/**
 * @unimplemented
 */
SDPORT_API
NTSTATUS
NTAPI
SdPortInitialize(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}


/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortCompleteRequest(
    _In_ PSDPORT_REQUEST Request,
    _In_ NTSTATUS Status)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
NTSTATUS
NTAPI
SdPortPoFxPowerControl(
    _In_ PVOID PrivateExtension,
    _In_ LPCGUID PowerControlCode,
    _In_opt_ PVOID InBuffer,
    _In_ SIZE_T InBufferSize,
    _Out_opt_ PVOID OutBuffer,
    _In_ SIZE_T OutBufferSize,
    _Out_opt_ PSIZE_T BytesReturned)
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
NTSTATUS
NTAPI
SdPortGetPciConfigSpace(
    _In_ PSD_MINIPORT Miniport,
    _In_ UCHAR Offset,
    _Out_writes_bytes_(Length) PUCHAR Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
NTSTATUS
NTAPI
SdPortSetPciConfigSpace(
    _In_ PSD_MINIPORT Miniport,
    _In_ UCHAR Offset,
    _In_reads_bytes_(Length) PUCHAR Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWait(
    _In_ ULONG Microseconds)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ ULONG Data)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ USHORT Data)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ UCHAR Data)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
ULONG
NTAPI
SdPortReadRegisterUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register)
{
    UNIMPLEMENTED;
    return 0;
}

/**
 * @unimplemented
 */
SDPORT_API
USHORT
NTAPI
SdPortReadRegisterUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register)
{
    UNIMPLEMENTED;
    return 0;
}

/**
 * @unimplemented
 */
SDPORT_API
UCHAR
NTAPI
SdPortReadRegisterUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register)
{
    UNIMPLEMENTED;
    return 0;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PULONG Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PUSHORT Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PUCHAR Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PULONG Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PUSHORT Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}

/**
 * @unimplemented
 */
SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PUCHAR Buffer,
    _In_ ULONG Length)
{
    UNIMPLEMENTED;
}
