//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        InterruptControl = 0x004
    };
}

// The intrCtrl register controls the interrupt capabilities of Banshee. Bits 1:0 enable video horizontal sync
// signal generation of interrupts. Generated horizontal sync interrupts are detected by the CPU by reading
// bits 7:6 of intrCtrl. Bits 3:2 enable video vertical sync signal generation of interrupts. Generated vertical
// sync interrupts are detected by the CPU by reading bits 9:8 of intrCtrl. Bit 4 of intrCtrl enables
// generation of interrupts when the frontend PCI FIFO is full. Generated PCI FIFO Full interrupts are
// detected by the CPU by reading bit 10 of intrCtrl. PCI FIFO full interrupts are genered when intrCtrl bit
// 4 is set and the number of free entries in the frontend PCI FIFO drops below the value specified in
// fbiInit0 bits(10:6). Bit 5 of intrCtrl enables the user interrupt command USERINTERRUPT generation
// of interrupts. Generated user interrupts are detected by the CPU by reading bit 11 of intrCtrl. The tag
// associated with a generated user interrupt is stored in bits 19:12 of intrCtrl.
// Generated interrupts are cleared by writing a 0 to the bit signaling a particular interrupt was generated
// and writing a 1 to interCtrl bit(31). For example, a PCI FIFO full generated interrupt is cleared by
// writing a 0 to bit 10 of intrCtrl, and a generated user interrupt is cleared by writing a 0 to bit 11 of
// intrCtrl. For both cases, bit 31 of intrCtrl must be written with the value 1 to clear the external PCI
// interrupt. Care must be taken when clearing interrupts not to accidentally overwrite the interrupt mask
// bits (bits 5:0) of intrCtrl) which enable generation of particular interrupts.
// Note that writes to the intrCtrl register are not pushed on the PCI frontend FIFO, so writes to intrCtrl
// are processed immediately. Since intrCtrl is not FIFO’ed, writes to intrCtrl may be processed out-of-
// order with respect to other queued writes in the PCI and memory-backed FIFOs.

typedef uint32_t InterruptControl_t;
namespace InterruptControl
{

    enum
    {
        HorizontalSyncRisingEdge =          0x00000001, // 0 Horizontal Sync (rising edge) interrupts enable (1=enable). Default is 0.
        horizontalSyncFallingEdge =         0x00000002, // 1 Horizontal Sync (falling edge) interrupts enable (1=enable). Default is 0.
        VerticalSyncRisingEdge =            0x00000004, // 2 Vertical Sync (rising edge) interrupts enable (1=enable). Default is 0.
        VerticalSyncFallingEdge =           0x00000008, // 3 Vertical Sync (falling edge) interrupts enable (1=enable). Default is 0.
        PCIFifoInterruptEnable =            0x00000010, // 4 PCI FIFO Full interrupts enable (1=enable). Default is 0.
        UserInterruptCommandEnable =        0x00000020, // 5 User Interrupt Command interrupts enable (1=enable). Default is 0.
        HorSyncRisingInterruptGenerated =   0x00000040, // 6 Horizontal Sync (rising edge) interrupt generated (1=interrupt generated).
        HorSyncFallingInterruptGenerated =  0x00000080, // 7 Horizontal Sync (falling edge) interrupt generated (1=interrupt generated).
        VerSyncRisingInterruptGenerated =   0x00000100, // 8 Vertical Sync (rising edge) interrupt generated (1=interrupt generated).
        VerSyncFallingInterruptGenerated =  0x00000200, // 9 Vertical Sync (falling edge) interrupt generated (1=interrupt generated).
        PCIFifoFullInterruptGenerated =     0x00000400, // 10 PCI FIFO Full interrupt generated (1=interrupt generated).
        UserInterruptCommandGenerated =     0x00000800, // 11 User Interrupt Command interrupt generated (1=interrupt generated).
        UserInterruptCommand =              0x000ff000, // 19:12 User Interrupt Command Tag. Read only.
        HoleCountingInterruptsEnable =      0x00100000, // 20 Hole counting interupts enable (1=enable). Default is 0.
        VMIInterruptsEnable =               0x00200000, // 21 VMI interrupts enable. (1=enable). Default is 0.
        HoleCountingInterruptGenerated =    0x00400000, // 22 Hole counting interrupt generated (1=interrupt generated).
        VMIInterruptGenerated =             0x00800000, // 23 VMI interrupt generated (1=interrupt generated).
                                                        // 29:24 reserved
        VGAInterruptGenerated =             0x40000000, // 30 VGA Interrupt generated (1=interrupt generated).
        ExternalPin =                       0x80000000  // 31 External pin pci_inta value, active low (0=PCI interrupt is active, 1=PCI interrupt is inactive)
    };

    namespace Shift
    {
        enum
        {
            HorizontalSyncRisingEdge = 0x00,
            horizontalSyncFallingEdge = 0x01,
            VerticalSyncRisingEdge = 0x02,
            VerticalSyncFallingEdge = 0x03,
            PCIFifoInterruptEnable = 0x04,
            UserInterruptCommandEnable = 0x05,
            HorSyncRisingInterruptGenerated = 0x06,
            HorSyncFallingInterruptGenerated = 0x07,
            VerSyncRisingInterruptGenerated = 0x08,
            VerSyncFallingInterruptGenerated = 0x09,
            PCIFifoFullInterruptGenerated = 0x0a,
            UserInterruptCommandGenerated = 0x0b,
            UserInterruptCommand = 0x0c,
            HoleCountingInterruptsEnable = 0x14,
            VMIInterruptsEnable = 0x15,
            HoleCountingInterruptGenerated = 0x16,
            VMIInterruptGenerated = 0x17,
            VGAInterruptGenerated = 0x1e,
            ExternalPin = 0x1f
        };
    }
}

}