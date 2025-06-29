//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandReadPointerLow0 = 0x02c,
        CommandReadPointerLow1 = 0x05c
    };
}

// cmdRdPtrL? contains the lower 32-bits of the read pointer. This register is read / write and allows
// software to monitor the progress of the CMDFIFO. This register is read write and has no default value.
// At initialization, this register should be set to cmdBaseAddr ,expanded to a byte address.

typedef uint32_t CommandReadPointerLow_t;
namespace CommandReadPointerLow
{
    enum
    {
        Value = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandReadPointerLow_t Read(uint8_t* baseAddress)
    {
        return *((CommandReadPointerLow_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandReadPointerLow0 : Register::CommandReadPointerLow1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandReadPointerLow_t value)
    {
        *((CommandReadPointerLow_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandReadPointerLow0 : Register::CommandReadPointerLow1))) = value;
    }

}

}