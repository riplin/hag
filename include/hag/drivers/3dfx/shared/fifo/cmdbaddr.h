//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandBaseAddress0 = 0x020,
        CommandBaseAddress1 = 0x050
    };
}

// CMDFIFO ?’s base address pointer bits 23:0. CmdBaseAddr? contains either the entire frame buffer
// address of the start of CMDFIFO, or contains the AGP address of the start of CMDFIFO. This register is
// read write, and has no default.

typedef uint32_t CommandBaseAddress_t;
namespace CommandBaseAddress
{
    enum
    {
        Address = 0x00ffffff    // 24-bits of CMDFIFO address [23:0] in 4k byte pages. Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00
        };
    }

    template <int V>
    CommandBaseAddress_t Read(uint8_t* baseAddress)
    {
        return *((CommandBaseAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandBaseAddress0 : Register::CommandBaseAddress1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandBaseAddress_t value)
    {
        *((CommandBaseAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandBaseAddress0 : Register::CommandBaseAddress1))) = value;
    }

}

}