//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        Status = 0x000
    };
}

// The status register provides a way for the CPU to interrogate the graphics processor about its current state
// and FIFO availability. The status register is read only, but writing to status clears any Banshee generated
// PCI interrupts.

// Bits(5:0) show the number of entries available in the internal host FIFO. The internal host FIFO is 64
// entries deep. The FIFO is empty when bits(5:0)=0x3f. Bit(6) is the state of the monitor vertical retrace
// signal, and is used to determine when the monitor is being refreshed. Bit(7) of status is used to determine
// if the graphics engine of FBI is active. Note that bit(7) only determines if the graphics engine of FBI is
// busy -- it does not include information as to the status of the internal PCI FIFOs. Bit(8) of status is used
// to determine if TREX is busy. Note that bit(8) of status is set if any unit in TREX is not idle -- this
// includes the graphics engine and all internal TREX FIFOs. Bit(9) of status determines if all units in the
// Banshee system (including graphics engines, FIFOs, etc.) are idle. Bit(9) is set when any internal unit in
// Banshee is active (e.g. graphics is being rendered or any FIFO is not empty). When the Memory FIFO is
// enabled, bits(27:12) show the number of entries available in the Memory FIFO. Depending upon the
// amount of frame buffer memory available, a maximum of 65,536 entries may be stored in the Memory
// FIFO. The Memory FIFO is empty when bits(27:12)=0xffff. Bits (30:28) of status track the number of
// outstanding SWAPBUFFER commands. When a SWAPBUFFER command is received from the host
// cpu, bits (30:28) are incremented -- when a SWAPBUFFER command completes, bits (30:28) are
// decremented. Bit(31) of status is used to monitor the status of the PCI interrupt signal. If Banshee
// generates a vertical retrace interrupt (as defined in pciInterrupt), bit(31) is set and the PCI interrupt
// signal line is activated to generate a hardware interrupt. An interrupt is cleared by writing to status with
// “dont-care” data. NOTE THAT BIT(31) IS CURRENTLY NOT IMPLEMENTED IN HARDWARE, AND WILL ALWAYS
// RETURN 0X0.

typedef uint32_t Status_t;
namespace Status
{

    enum
    {
        FIFOFreespace =             0x0000003F, // 5:0   PCI FIFO freespace (0x3f=FIFO empty). Default is 0x3f.
        VerticalRetrace =           0x00000040, // 6     Vertical retrace (0=Vertical retrace active, 1=Vertical retrace inactive). Default is 1.
        FBIGraphicsEngineBusy =     0x00000080, // 7     FBI graphics engine busy (0=engine idle, 1=engine busy). Default is 0.
        TREXBusy =                  0x00000100, // 8     TREX busy (0=engine idle, 1=engine busy). Default is 0.
        BansheeBusy =               0x00000200, // 9     Banshee busy (0=idle, 1=busy). Default is 0.
        TwoDBusy =                  0x00000400, // 10    2D busy (0=idle, 1=busy). Default is 0.
                                                // 11    Reserved
        MemoryFIFO =                0x0FFFF000, // 27:12 Memory FIFO (marked reserved in docs)
        SwapBuffersPending =        0x70000000, // 30:28 Swap Buffers Pending. Default is 0x0.
        PCIInterrupt =              0x80000000  // 31    PCI Interrupt Generated. Default is 0x0. (not currently implemented).        
    };

    namespace Shift
    {
        enum
        {
            FIFOFreespace = 0x00,
            VerticalRetrace = 0x06,
            FBIGraphicsEngineBusy = 0x07,
            TREXBusy = 0x08,
            BansheeBusy = 0x09,
            TwoDBusy = 0x0a,
            MemoryFIFO = 0x0b,
            SwapBuffersPending = 0x0c,
            PCIInterrupt = 0x1f
        };
    }
}

}