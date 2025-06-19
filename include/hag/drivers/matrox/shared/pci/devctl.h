//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    DeviceControl = 0x04
};

}

typedef System::PCI::Register_t DeviceControl_t;

namespace DeviceControl
{
    enum //Mask
    {
        IOSpaceEnable = 0x00000001,                 // R/W I/O space. Controls device response to I/O SPACE accesses (VGA registers).
                                                    // 0 = disable the device response
                                                    // 1 = enable the device response
        MemorySpaceEnable = 0x00000002,             // R/W Memory space. Controls device response to memory accesses (EPROM, VGA frame
                                                    // buffer, MGA control aperture, MGA direct access aperture, and 8 MByte
                                                    // Pseudo-DMA window).
                                                    // 0 = disable the device response
                                                    // 1 = enable the device response
        BusMasterEnable = 0x00000004,               // R/W Bus master. Controls a device’s ability to act as a master on the
                                                    // PCI bus (used to access system memory):
                                                    // 0 = prevents the device from generating PCI accesses
                                                    // 1 = allows the device to behave as a bus master
        SpecialCycle = 0x00000008,                  // RO The hard coded ‘0’ indicates that the MGA will not respond to a special cycle.
        MemoryWriteAndInvalidate = 0x00000010,      // RO The hard coded ‘0’ indicates that an MGA acting as a bus master will never generate
                                                    // the write and invalidate command.
        VGASnoop = 0x00000020,                      // Controls how the chip handles I/O accesses to the VGA DAC locations.
                                                    // R/W The vgasnoop field is only used when vgaioen (see OPTION on page 4-14) is ‘1’.
                                                    // 0 = The chip will reply to read and write accesses at VGA locations 3C6h, 3C7h, 3C8h, and 3C9h.
                                                    // 1 = The chip will snoop writes to VGA DAC locations. It will not assert
                                                    //     PTRDY/, PSTOP/, and PDEVSEL/, but will internally decode the access and pro-
                                                    //     gram the on-board DAC. In situations where the chip is not ready to snoop the
                                                    //     access, it will acknowledge the cycle by asserting PDEVSEL/, and force a retry
                                                    //     cycle by asserting PSTOP/. Read accesses to VGA DAC locations are not affected
                                                    //     by vgasnoop.
        RespondToParityErrors = 0x00000040,         // RO The hard coded ‘0’ indicates that the MGA will not detect and signal parity errors
                                                    // (MGA does generate parity information as per the PCI specification requirement).
                                                    // Writing has no effect.
        WaitCycle = 0x00000080,                     // RO This bit reads as ‘1’, indicating that continuous address/data stepping is performed for
                                                    // read accesses in the target (data stepping) and the master (address stepping). Stepping
                                                    // lasts one pclk. Writing has no effect.
        SERREnable = 0x00000100,                    // RO This hard coded ‘0’ indicates that MGA does not generate SERR interrupts.
                                                    // Writing has no effect.
        CapList = 0x00100000,                       // RO Capability list availability.
                                                    // 0 = Capability list not available.
                                                    // 1 = The list is located at the offset in the CAP_PTR register.
        Cap66MHz = 0x00200000,                      // RO The hard coded ‘0’ indicates that the MGA is running at 33 MHz or lower clock rates.
        UserDefinableFeatureSupport = 0x00400000,   // RO The hard coded ‘0’ indicates that the MGA does not support user-definable features.
        FastBackToBackTransactions = 0x00800000,    // RO The hard coded ‘1’ indicates that the MGA supports fast back-to-back transactions
                                                    // when part of the transaction targets a different agent. Writing has no effect.
        DeviceSelectTiming = 0x06000000,            // RO Device select timing. Specifies the timing of devsel. It is read as ‘01’.
        SignaledTargetAbort = 0x08000000,           // R/W Signaled target abort. Set to ‘1’ when the MGA terminates a transaction in target mode
                                                    // with target-abort. This bit is cleared to ‘0’ when written with ‘1’.
        ReceivedTargetAbort = 0x10000000,           // R/W Received target abort. Set to ‘1’ when the MGA is a master and a transaction is
                                                    // terminated with target-abort. This bit is cleared to ‘0’ when written with ‘1’.
        ReceivedMasterAbort = 0x20000000,           // R/W Received master abort. Set to ‘1’ when a transaction is terminated with master-abort
                                                    // by the MGA. This bit is cleared to ‘0’ when written with ‘1’.
        SignalSERR = 0x40000000,                    // RO MGA does not assert SERR/. Writing has no effect. Reading will give ‘0’s.
        DetectParityErrors = 0x80000000             // RO MGA does not detect parity errors. Writing has no effect. Reading will give ‘0’s.
    };

    namespace Shift
    {
        enum
        {
            IOSpaceEnable = 0x00,
            MemorySpaceEnable = 0x01,
            BusMasterEnable = 0x02,
            SpecialCycle = 0x03,
            MemoryWriteAndInvalidate = 0x04,
            VGASnoop = 0x05,
            RespondToParityErrors = 0x06,
            WaitCycle = 0x07,
            SERREnable = 0x08,
            Cap66MHz = 0x15,
            UserDefinableFeatureSupport = 0x16,
            FastBackToBackTransactions = 0x17,
            DeviceSelectTiming = 0x19,
            SignaledTargetAbort = 0x1b,
            ReceivedTargetAbort = 0x1c,
            ReceivedMasterAbort = 0x1d,
            SignalSERR = 0x1e,
            DetectParityErrors = 0x1f,
        };
    }

    inline DeviceControl_t Read(System::PCI::Device_t device) { return System::PCI::Read32(device, Register::DeviceControl); }
}

}