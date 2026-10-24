#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define SDPORT_API

#include <wdm.h>

/*
          2    1 000012D0 SdPortCompleteRequest
          3    2 00001330 SdPortGetPciConfigSpace
          4    3 000169A0 SdPortInitialize
          5    4 00001380 SdPortPoFxPowerControl
          6    5 00001460 SdPortReadRegisterBufferUchar
          7    6 000014A0 SdPortReadRegisterBufferUlong
          8    7 000014E0 SdPortReadRegisterBufferUshort
          9    8 00001520 SdPortReadRegisterUchar
         10    9 00001540 SdPortReadRegisterUlong
         11    A 00001560 SdPortReadRegisterUshort
         12    B 00001580 SdPortSetPciConfigSpace
         13    C 000015D0 SdPortWait
         14    D 000015F0 SdPortWriteRegisterBufferUchar
         15    E 00001630 SdPortWriteRegisterBufferUlong
         16    F 00001670 SdPortWriteRegisterBufferUshort
         17   10 000016B0 SdPortWriteRegisterUchar
         18   11 000016D0 SdPortWriteRegisterUlong
         19   12 000016F0 SdPortWriteRegisterUshort
 */

typedef struct _SD_MINIPORT SD_MINIPORT, *PSD_MINIPORT;
typedef struct _SDPORT_CAPABILITIES SDPORT_CAPABILITIES, *PSDPORT_CAPABILITIES;

#define SDPORT_ERROR_CMD_TIMEOUT                    0x00000001
#define SDPORT_ERROR_CMD_CRC_ERROR                  0x00000002
#define SDPORT_ERROR_CMD_END_BIT_ERROR              0x00000004
#define SDPORT_ERROR_CMD_INDEX_ERROR                0x00000008
#define SDPORT_ERROR_DATA_TIMEOUT                   0x00000010
#define SDPORT_ERROR_DATA_CRC_ERROR                 0x00000020
#define SDPORT_ERROR_DATA_END_BIT_ERROR             0x00000040
#define SDPORT_ERROR_BUS_POWER_ERROR                0x00000080
#define SDPORT_ERROR_AUTO_CMD12_ERROR               0x00000100
#define SDPORT_ERROR_ADMA_ERROR                     0x00000200

#define SDPORT_ERROR_ACMD12_NOT_EXECUTED            0x00010000
#define SDPORT_ERROR_ACMD12_RESPONSE_TIMEOUT        0x00020000
#define SDPORT_ERROR_ACMD12_RESPONSE_CRC_ERROR      0x00040000
#define SDPORT_ERROR_ACMD12_END_BIT_ERROR           0x00080000
#define SDPORT_ERROR_ACMD12_INDEX_ERROR             0x00100000
#define SDPORT_ERROR_ACMD12_CWODAT_NOT_EXECUTED     0x00800000

#define SDPORT_ERROR_SYSTEM_DMA_ERROR               0x20000000
#define SDPORT_GENERIC_IO_ERROR                     0x40000000
#define SDPORT_ERROR_UNKNOWN                        0x80000000

#define SDPORT_CMD_ERROR_GROUP                     (SDPORT_ERROR_CMD_TIMEOUT       | \
                                                    SDPORT_ERROR_CMD_CRC_ERROR     | \
                                                    SDPORT_ERROR_CMD_END_BIT_ERROR | \
                                                    SDPORT_ERROR_CMD_INDEX_ERROR   | \
                                                    SDPORT_ERROR_ADMA_ERROR)

#define SDPORT_DAT_ERROR_GROUP                     (SDPORT_ERROR_DATA_TIMEOUT       | \
                                                    SDPORT_ERROR_DATA_CRC_ERROR     | \
                                                    SDPORT_ERROR_DATA_END_BIT_ERROR | \
                                                    SDPORT_GENERIC_IO_ERROR         | \
                                                    SDPORT_ERROR_ADMA_ERROR)


#define SDPORT_EVENT_CARD_RESPONSE             0x00000001
#define SDPORT_EVENT_CARD_RW_END               0x00000002
#define SDPORT_EVENT_BLOCKGAP                  0x00000004
#define SDPORT_EVENT_DMA_COMPLETE              0x00000008
#define SDPORT_EVENT_BUFFER_EMPTY              0x00000010
#define SDPORT_EVENT_BUFFER_FULL               0x00000020
#define SDPORT_EVENT_CARD_CHANGE               0x000000C0
#define SDPORT_EVENT_CARD_INTERRUPT            0x00000100
#define SDPORT_EVENT_TUNING                    0x00001000
#define SDPORT_EVENT_ERROR                     0x00008000
#define SDPORT_EVENT_SG_LIST                   0x00010000
#define SDPORT_EVENT_DMA_RESOURCES_ALLOCATED   0x00020000
#define SDPORT_EVENT_DMA_TRANSFER_CANCELLED    0x00040000
#define SDPORT_EVENT_CLOCK_STABLE              0x00080000
#define SDPORT_EVENT_SIGNALING_VOLTAGE_STABLE  0x00100000
#define SDPORT_EVENT_VCORE_STABLE              0x00200000
#define SDPORT_EVENT_SYSTEM_DMA_COMPLETE       0x00400000
#define SDPORT_EVENT_HPI                       0x00800000
#define SDPORT_EVENT_PSTATE_CHANGED            0x01000000
#define SDPORT_EVENT_ALL                       0xFFFFFFFF

#define SDPORT_DEVICE_CONTROLLER_EVENTS       (SDPORT_EVENT_CARD_CHANGE   | \
                                               SDPORT_EVENT_CARD_RESPONSE | \
                                               SDPORT_EVENT_CARD_RW_END   | \
                                               SDPORT_EVENT_BUFFER_EMPTY  | \
                                               SDPORT_EVENT_BUFFER_FULL   | \
                                               SDPORT_EVENT_DMA_COMPLETE  | \
                                               SDPORT_EVENT_ERROR)

#define SDPORT_DEVICE_ASYNC_EVENTS            (SDPORT_EVENT_CARD_CHANGE    | \
                                               SDPORT_EVENT_CARD_INTERRUPT | \
                                               SDPORT_EVENT_TUNING)

#define SDPORT_DEVICE_SOFTWARE_EVENTS         (SDPORT_EVENT_SG_LIST                   | \
                                               SDPORT_EVENT_DMA_RESOURCES_ALLOCATED   | \
                                               SDPORT_EVENT_DMA_TRANSFER_CANCELLED    | \
                                               SDPORT_EVENT_CLOCK_STABLE              | \
                                               SDPORT_EVENT_SIGNALING_VOLTAGE_STABLE)


typedef enum _SDPORT_BUS_TYPE {
    SdBusTypeUndefined = -1,
    SdBusTypeAcpi = 0,
    SdBusTypePci = 1
} SDPORT_BUS_TYPE, *PSDPORT_BUS_TYPE;

typedef UCHAR SDPORT_COMMAND_INDEX, *PSDPORT_COMMAND_INDEX;

typedef enum {
    SdCommandTypeUndefined = 0,
    SdCommandTypeSuspend = 1,
    SdCommandTypeResume = 2,
    SdCommandTypeAbort = 3
} SDPORT_COMMAND_TYPE, *PSDPORT_COMMAND_TYPE;

typedef enum {
    SdCommandClassUndefined = 0,
    SdCommandClassStandard = 1,
    SdCommandClassApp = 2
} SDPORT_COMMAND_CLASS, *PSDPORT_COMMAND_CLASS;

typedef enum {
    SdResponseTypeUndefined = 0,
    SdResponseTypeNone = 1,
    SdResponseTypeR1 = 2,
    SdResponseTypeR1B = 3,
    SdResponseTypeR2 = 4,
    SdResponseTypeR3 = 5,
    SdResponseTypeR4 = 6,
    SdResponseTypeR5 = 7,
    SdResponseTypeR5B = 8,
    SdResponseTypeR6 = 9
} SDPORT_RESPONSE_TYPE, *PSDPORT_RESPONSE_TYPE;

typedef enum {
    SdTransferTypeUndefined = 0,
    SdTransferTypeNone = 1,
    SdTransferTypeSingleBlock = 2,
    SdTransferTypeMultiBlock = 3,
    SdTransferTypeMultiBlockNoStop =4
} SDPORT_TRANSFER_TYPE, *PSDPORT_TRANSFER_TYPE;

typedef enum {
    SdTransferDirectionUndefined = 0,
    SdTransferDirectionRead = 1,
    SdTransferDirectionWrite = 2
} SDPORT_TRANSFER_DIRECTION, *PSDPORT_TRANSFER_DIRECTION;

typedef enum {
    SdTransferMethodUndefined = 0,
    SdTransferMethodPio = 1,
    SdTransferMethodSgDma = 2,
} SDPORT_TRANSFER_METHOD, *PSDPORT_TRANSFER_METHOD;

#define SDPORT_MAX_RESPONSE_LENGTH 16

typedef struct _SDPORT_COMMAND {
    SDPORT_COMMAND_INDEX Index;
    SDPORT_COMMAND_TYPE Type;
    SDPORT_COMMAND_CLASS Class;
    SDPORT_RESPONSE_TYPE ResponseType;
    SDPORT_TRANSFER_TYPE TransferType;
    SDPORT_TRANSFER_DIRECTION TransferDirection;
    SDPORT_TRANSFER_METHOD TransferMethod;
    ULONG Argument;

    BOOLEAN UseAutoCmd12;
    BOOLEAN UseAutoCmd23;
    USHORT BlockSize;
    USHORT BlockCount;
    ULONG Length;
    PUCHAR DataBuffer;
    PVOID DmaVirtualAddress;
    PHYSICAL_ADDRESS DmaPhysicalAddress;
    PSCATTER_GATHER_LIST ScatterGatherList;
    ULONG ScatterGatherListSize;
} SDPORT_COMMAND, *PSDPORT_COMMAND;

typedef enum _SDPORT_REQUEST_TYPE {
    SdRequestTypeUnspecified = 0,
    SdRequestTypeCommandNoTransfer = 1,
    SdRequestTypeCommandWithTransfer = 2,
    SdRequestTypeStartTransfer = 3
} SDPORT_REQUEST_TYPE, *PSDPORT_REQUEST_TYPE;

typedef struct _SDPORT_REQUEST {
    SDPORT_REQUEST_TYPE Type;
    SDPORT_COMMAND Command;
    ULONG RequiredEvents;
    NTSTATUS Status;
} SDPORT_REQUEST, *PSDPORT_REQUEST;

typedef enum _SDPORT_RESET_TYPE {
    SdResetTypeUndefined = 0,
    SdResetTypeAll = 1,
    SdResetTypeCmd = 2,
    SdResetTypeDat = 3
} SDPORT_RESET_TYPE;

typedef enum _SDPORT_BUS_VOLTAGE {
    SdBusVoltageUndefined = 0,
    SdBusVoltage33 = 1,
    SdBusVoltage30 = 2,
    SdBusVoltage18 = 3,
    SdBusVoltageOff = 4
} SDPORT_BUS_VOLTAGE;

typedef enum _SDPORT_BUS_WIDTH {
    SdBusWidthUndefined = 0,
    SdBusWidth1Bit = 1,
    SdBusWidth4Bit = 4,
    SdBusWidth8Bit = 8
} SDPORT_BUS_WIDTH;

typedef enum _SDPORT_BUS_SPEED {
    SdBusSpeedUndefined = 0,
    SdBusSpeedNormal = 1,
    SdBusSpeedHigh = 2,
    SdBusSpeedSDR12 = 3,
    SdBusSpeedSDR25 = 4,
    SdBusSpeedSDR50 = 5,
    SdBusSpeedDDR50 = 6,
    SdBusSpeedSDR104 = 7,
    SdBusSpeedHS200 = 8,
    SdBusSpeedHS400 = 9
} SDPORT_BUS_SPEED;

typedef enum _SDPORT_SIGNALING_VOLTAGE {
    SdignalingVoltageUndefined = 0,
    SdSignalingVoltage33 = 1,
    SdSignalingVoltage18 = 2
} SDPORT_SIGNALING_VOLTAGE;

typedef enum _SDPORT_DRIVE_STRENGTH {
    SdDriveStrengthUndefined = 0,
    SdDriveStrength200mA = 1,
    SdDriveStrength400mA = 2,
    SdDriveStrength600mA = 3,
    SdDriveStrength800mA = 4
} SDPORT_DRIVE_STRENGTH;

typedef enum _SDPORT_DRIVER_TYPE {
    SdDriverTypeUndefined = 0,
    SdDriverTypeB = 1,
    SdDriverTypeA = 2,
    SdDriverTypeC = 3,
    SdDriverTypeD = 4
} SDPORT_DRIVER_TYPE;

typedef enum _SDPORT_BUS_OPERATION_TYPE {
    SdBusOperationUndefined = 0,
    SdResetHw = 1,
    SdResetHost = 2,
    SdSetClock = 3,
    SdSetVoltage = 4,
    SdSetPower = 5,
    SdSetBusWidth = 6,
    SdSetBusSpeed = 7,
    SdSetSignalingVoltage = 8,
    SdSetDriveStrength = 9,
    SdSetDriverType = 10,
    SdSetPresetValue = 11,
    SdSetBlockGapInterrupt = 12,
    SdExecuteTuning = 13
} SDPORT_BUS_OPERATION_TYPE;

typedef struct _SDPORT_BUS_OPERATION {
    SDPORT_BUS_OPERATION_TYPE Type;
    union {
        SDPORT_RESET_TYPE ResetType;
        ULONG FrequencyKhz;
        SDPORT_BUS_VOLTAGE Voltage;
        BOOLEAN PowerEnabled;
        SDPORT_BUS_WIDTH BusWidth;
        SDPORT_BUS_SPEED BusSpeed;
        SDPORT_SIGNALING_VOLTAGE SignalingVoltage;
        UCHAR DriveStrength;
        SDPORT_DRIVER_TYPE DriverType;
        BOOLEAN PresetValueEnabled;
        BOOLEAN BlockGapIntEnabled;
    } Parameters;
} SDPORT_BUS_OPERATION, *PSDPORT_BUS_OPERATION;

typedef
NTSTATUS
(NTAPI *PSDPORT_GET_SLOT_COUNT)(
    _In_  PSD_MINIPORT Miniport,
    _Out_ PUCHAR SlotCount);

typedef
VOID
(NTAPI *PSDPORT_GET_SLOT_CAPABILITIES)(
    _In_ PVOID PrivateExtension,
    _Out_ PSDPORT_CAPABILITIES Capabilities);

typedef
BOOLEAN
(NTAPI *PSDPORT_INTERRUPT)(
    _In_ PVOID PrivateExtension,
    _Out_ PULONG Events,
    _Out_ PULONG Errors,
    _Out_ PBOOLEAN NotifyCardChange,
    _Out_ PBOOLEAN NotifySdioInterrupt,
    _Out_ PBOOLEAN NotifyTuning);

typedef
NTSTATUS
(NTAPI *PSDPORT_ISSUE_REQUEST)(
    _In_ PVOID PrivateExtension,
    _In_ struct _SDPORT_REQUEST *Request);

typedef
VOID
(NTAPI *PSDPORT_GET_RESPONSE)(
    _In_ PVOID PrivateExtension,
    _In_ struct _SDPORT_COMMAND *Command,
    _Out_ PVOID ResponseBuffer);

typedef
VOID
(NTAPI *PSDPORT_REQUEST_DPC)(
    _In_ PVOID PrivateExtension,
    _Inout_ struct _SDPORT_REQUEST *Request,
    _In_ ULONG Events,
    _In_ ULONG Errors);

typedef
VOID
(NTAPI *PSDPORT_TOGGLE_EVENTS)(
    _In_ PVOID PrivateExtension,
    _In_ ULONG EventMask,
    _In_ BOOLEAN Enable);

typedef
VOID
(NTAPI *PSDPORT_CLEAR_EVENTS)(
    _In_ PVOID PrivateExtension,
    _In_ ULONG EventMask);

typedef
VOID
(NTAPI *PSDPORT_SAVE_CONTEXT)(
    _In_ PVOID PrivateExtension);

typedef
VOID
(NTAPI *PSDPORT_RESTORE_CONTEXT)(
    _In_ PVOID PrivateExtension);

typedef
NTSTATUS
(NTAPI *PSDPORT_INITIALIZE)(
    _In_ PVOID PrivateExtension,
    _In_ PHYSICAL_ADDRESS PhysicalBase,
    _In_ PVOID VirtualBase,
    _In_ ULONG Length,
    _In_ BOOLEAN CrashdumpMode);

typedef
NTSTATUS
(NTAPI *PSDPORT_ISSUE_BUS_OPERATION)(
    _In_ PVOID PrivateExtension,
    _In_ struct _SDPORT_BUS_OPERATION *BusOperation);

typedef
BOOLEAN
(NTAPI *PSDPORT_GET_CARD_DETECT_STATE)(
    _In_ PVOID PrivateExtension);

typedef
BOOLEAN
(NTAPI *PSDPORT_GET_WRITE_PROTECT_STATE)(
    _In_ PVOID PrivateExtension);

typedef
NTSTATUS
(NTAPI *PSDPORT_PO_FX_POWER_CONTROL_CALLBACK)(
    _In_ struct _SD_MINIPORT *Miniport,
    _In_ LPCGUID PowerControlCode,
    _In_reads_bytes_opt_(InputBufferSize) PVOID InputBuffer,
    _In_ SIZE_T InputBufferSize,
    _Out_writes_bytes_opt_(OutputBufferSize) PVOID OutputBuffer,
    _In_ SIZE_T OutputBufferSize,
    _Out_opt_ PSIZE_T BytesReturned);

typedef
VOID
(NTAPI *PSDPORT_CLEANUP)(
    _In_ struct _SD_MINIPORT *Miniport);

typedef struct _SDPORT_INITIALIZATION_DATA {
    ULONG StructureSize;

    PSDPORT_GET_SLOT_COUNT GetSlotCount;
    PSDPORT_GET_SLOT_CAPABILITIES GetSlotCapabilities;
    PSDPORT_INTERRUPT Interrupt;
    PSDPORT_ISSUE_REQUEST IssueRequest;
    PSDPORT_GET_RESPONSE GetResponse;
    PSDPORT_REQUEST_DPC RequestDpc;
    PSDPORT_TOGGLE_EVENTS ToggleEvents;
    PSDPORT_CLEAR_EVENTS ClearEvents;
    PSDPORT_SAVE_CONTEXT SaveContext;
    PSDPORT_RESTORE_CONTEXT RestoreContext;
    PSDPORT_INITIALIZE Initialize;
    PSDPORT_ISSUE_BUS_OPERATION IssueBusOperation;
    PSDPORT_GET_CARD_DETECT_STATE GetCardDetectState;
    PSDPORT_GET_WRITE_PROTECT_STATE GetWriteProtectState;
    PSDPORT_PO_FX_POWER_CONTROL_CALLBACK PowerControlCallback;
    PSDPORT_CLEANUP Cleanup;

    ULONG PrivateExtensionSize;
    BOOLEAN CrashdumpSupported;
} SDPORT_INITIALIZATION_DATA, *PSDPORT_INITIALIZATION_DATA;

#define MAX_SD_SLOTS 8

typedef struct _SDPORT_SLOT_EXTENSION {
    PSD_MINIPORT Miniport;

    DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) UCHAR PrivateExtension[0];
} SDPORT_SLOT_EXTENSION, *PSDPORT_SLOT_EXTENSION;

typedef struct _SDPORT_PCI_CONFIG_INFO {
    ULONG VendorID;
    ULONG DeviceID;
    ULONG RevisionID;
    ULONG SubsysID;
} SDPORT_PCI_CONFIG_INFO, *PSDPORT_PCI_CONFIG_INFO;

typedef struct _SDPORT_CAPABILITIES {
    UCHAR SpecVersion;
    UCHAR MaximumOutstandingRequests;
    USHORT MaximumBlockSize;
    USHORT MaximumBlockCount;
    ULONG BaseClockFrequencyKhz;
    ULONG TuningTimerCountInSeconds;
    ULONG DmaDescriptorSize;
    ULONG AlignmentRequirement;
    ULONG PioTransferMaxThreshold;
    struct {
        ULONG ScatterGatherDma:1;
        ULONG Address64Bit:1;
        ULONG BusWidth8Bit:1;
        ULONG HighSpeed:1;
        ULONG SignalingVoltage18V:1;
        ULONG SDR50:1;
        ULONG DDR50:1;
        ULONG SDR104:1;
        ULONG HS200:1;
        ULONG HS400:1;
        ULONG Reserved:5;
        ULONG DriverTypeA:1;
        ULONG DriverTypeB:1;
        ULONG DriverTypeC:1;
        ULONG DriverTypeD:1;
        ULONG TuningForSDR50:1;
        ULONG SoftwareTuning:1;
        ULONG AutoCmd12:1;
        ULONG AutoCmd23:1;
        ULONG Voltage18V:1;
        ULONG Voltage30V:1;
        ULONG Voltage33V:1;
        ULONG Limit200mA:1;
        ULONG Limit400mA:1;
        ULONG Limit600mA:1;
        ULONG Limit800mA:1;
        ULONG SaveContext:1;
        ULONG Reserved1:3;
    } Supported;
    struct {
        ULONG UsePioForRead:1;
        ULONG UsePioForWrite:1;
        ULONG Reserved:30;
    } Flags;
} SDPORT_CAPABILITIES, *PSDPORT_CAPABILITIES;

typedef struct _SDPORT_PORT_CONFIG_INFO {
    PVOID DeviceObject;
    SDPORT_BUS_TYPE BusType;
    union {
        SDPORT_PCI_CONFIG_INFO PciConfigInfo;
    } BusInfo;
} SDPORT_PORT_CONFIG_INFO, *PSDPORT_PORT_CONFIG_INFO;

typedef struct _SD_MINIPORT {
    PSDPORT_INITIALIZATION_DATA InitializationData;
    SDPORT_PORT_CONFIG_INFO ConfigurationInfo;
    UCHAR SlotCount;
    PSDPORT_SLOT_EXTENSION SlotExtensionList[MAX_SD_SLOTS];
} SD_MINIPORT, *PSD_MINIPORT;


SDPORT_API
NTSTATUS
NTAPI
SdPortInitialize(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath);

SDPORT_API
VOID
NTAPI
SdPortCompleteRequest(
    _In_ PSDPORT_REQUEST Request,
    _In_ NTSTATUS Status);

SDPORT_API
VOID
NTAPI
SdPortCompleteRequest(
    _In_ PSDPORT_REQUEST Request,
    _In_ NTSTATUS Status);

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
    _Out_opt_ PSIZE_T BytesReturned);

SDPORT_API
NTSTATUS
NTAPI
SdPortGetPciConfigSpace(
    _In_ PSD_MINIPORT Miniport,
    _In_ UCHAR Offset,
    _Out_writes_bytes_(Length) PUCHAR Buffer,
    _In_ ULONG Length);

SDPORT_API
NTSTATUS
NTAPI
SdPortSetPciConfigSpace(
    _In_ PSD_MINIPORT Miniport,
    _In_ UCHAR Offset,
    _In_reads_bytes_(Length) PUCHAR Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortWait(
    _In_ ULONG Microseconds);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ ULONG Data);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ USHORT Data);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_ UCHAR Data);

SDPORT_API
ULONG
NTAPI
SdPortReadRegisterUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register);

SDPORT_API
USHORT
NTAPI
SdPortReadRegisterUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register);

SDPORT_API
UCHAR
NTAPI
SdPortReadRegisterUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register);

SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PULONG Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PUSHORT Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortReadRegisterBufferUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _Out_writes_all_(Length) PUCHAR Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUlong(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PULONG Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUshort(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PUSHORT Buffer,
    _In_ ULONG Length);

SDPORT_API
VOID
NTAPI
SdPortWriteRegisterBufferUchar(
    _In_ PVOID BaseAddress,
    _In_ ULONG Register,
    _In_reads_(Length) PUCHAR Buffer,
    _In_ ULONG Length);

#ifdef __cplusplus
}
#endif
