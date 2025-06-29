//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandBump0 = 0x028,
        CommandBump1 = 0x058
    };
}

// cmdBump? defines the number of words to increment the amin pointer by, when managed by software.
// This register is write only and has no default.

typedef uint32_t CommandBump_t;
namespace CommandBump
{
    enum
    {
        Value = 0x0000ffff  // Number of words to bump CMDFIFO ?’s write pointer. Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    void Write(uint8_t* baseAddress, CommandBump_t value)
    {
        *((CommandBump_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandBump0 : Register::CommandBump1))) = value;
    }

}

}