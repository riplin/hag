//Copyright 2025-Present riplin

#pragma once

#include <hag/system/sysasm.h>
#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandBaseSize0 = 0x024,
        CommandBaseSize1 = 0x054
    };
}

// cmdBaseSize? contains the size of the command fifo in bits 7:0 in 4k byte pages, starting from 4k. Bit 8
// enables or disables command fifo ? operation. Bit 9 defines the location of command fifo ?, a value of 0
// locates the command fifo in frame buffer memory, and value of 1 locates the command fifo in AGP
// memory. Bit 10 disables the hole counter.

typedef uint32_t CommandBaseSize_t;
namespace CommandBaseSize
{
    enum
    {
        Size =          0x000000ff, // Size of CmdFifo in 4k byte pages. (0=4k, 1 = 8k, etc…). Default is 0x0.
        Enable =        0x00000100, // CMDFIFO_? enable (0=disable, 1=enable). Default is 0x0.
        Disable =       0x00000000,
        Location =      0x00000200, // CMDFIFO_0 resides in AGP (0=frame buffer memory, 1=AGP memory). Default is 0x0.
        Framebuffer =   0x00000000,
        AGP =           0x00000200,
        HoleCounter =   0x00000400, // Disable hole counter (0=enable, 1=disable). Default is 0x0.
        HoleEnable =    0x00000000,
        HoleDisable =   0x00000400
    };

    namespace Shift
    {
        enum
        {
            Size =          0x00,
            Enable =        0x08,
            Location =      0x09,
            HoleCounter =   0x0a
        };
    }

    template <int V>
    CommandBaseSize_t Read(uint8_t* baseAddress)
    {
        return *((CommandBaseSize_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandBaseSize0 : Register::CommandBaseSize1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandBaseSize_t value)
    {
        SYS_Barrier();
        *((CommandBaseSize_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandBaseSize0 : Register::CommandBaseSize1))) = value;
    }

}

}