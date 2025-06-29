//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandFifoDepth0 = 0x044,
        CommandFifoDepth1 = 0x074
    };
}

// cmdFifoDepth? is a 20-bit register containing the current depth of CMDFIFO ?. Depth is the number of
// remaining unexecuted commands in off chip memory. The CMDFIFO is allowed to read upto, but not over
// the number of entries indicated by cmdFifoDepth register. This register is read write and has no default
// value.

typedef uint32_t CommandFifoDepth_t;
namespace CommandFifoDepth
{
    enum
    {
        Value = 0x000fffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandFifoDepth_t Read(uint8_t* baseAddress)
    {
        return *((CommandFifoDepth_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandFifoDepth0 : Register::CommandFifoDepth1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandFifoDepth_t value)
    {
        *((CommandFifoDepth_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandFifoDepth0 : Register::CommandFifoDepth1))) = value;
    }

}

}