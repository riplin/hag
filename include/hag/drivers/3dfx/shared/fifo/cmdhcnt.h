//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandHoleCount0 = 0x048,
        CommandHoleCount1 = 0x078
    };
}

// cmdHoleCnt? contains the number of unwritten locations between cmdAMin and cmdAMax.

typedef uint32_t CommandHoleCount_t;
namespace CommandHoleCount
{
    enum
    {
        Value = 0x0000ffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandHoleCount_t Read(uint8_t* baseAddress)
    {
        return *((CommandHoleCount_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandHoleCount0 : Register::CommandHoleCount1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandHoleCount_t value)
    {
        *((CommandHoleCount_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandHoleCount0 : Register::CommandHoleCount1))) = value;
    }

}

}