//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandReadPointerHigh0 = 0x030,
        CommandReadPointerHigh1 = 0x060
    };
}

// cmdRdPtrH? contains the upper 4-bits of the read pointer. This register is read write and has no default
// value. At initialization, this register should be set to cmdBaseAddr, expanded to a byte address.

typedef uint32_t CommandReadPointerHigh_t;
namespace CommandReadPointerHigh
{
    enum
    {
        Value = 0x0000000f  // Upper 4-bits of the CMDFIFO read pointer. Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandReadPointerHigh_t Read(uint8_t* baseAddress)
    {
        return *((CommandReadPointerHigh_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandReadPointerHigh0 : Register::CommandReadPointerHigh1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandReadPointerHigh_t value)
    {
        *((CommandReadPointerHigh_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandReadPointerHigh0 : Register::CommandReadPointerHigh1))) = value;
    }

}

}