//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandFifoFetchThreshhold = 0x080,
    };
}

// cmdFifoThresh bits 4:0 contain the fetch threshold, used when the fifo freespace is below fetch threshold,
// then no new requests are made. Bits 8:5 contain the fifo high water mark, when fifo freespace is above
// the water mark, then fill requests will be generated. When the high water mark is qualified, then new
// requests are generated.

typedef uint32_t CommandFifoFetchThreshhold_t;
namespace CommandFifoFetchThreshhold
{
    enum
    {
        LowWaterMark = 0x0000001f,  // CMDFIFO 0 and 1’s fifo fetch threshold (low water mark) (Default value is 0).
        HighWaterMark = 0x000003e0, // CMDFIFO 0 and 1’s fifo high water mark (Default value is 7).
    };

    namespace Shift
    {
        enum
        {
            LowWaterMark = 0x00,
            HighWaterMark = 0x05
        };
    }

    inline CommandFifoFetchThreshhold_t Read(uint8_t* baseAddress)
    {
        return *((CommandFifoFetchThreshhold_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::CommandFifoFetchThreshhold));
    }
    
    inline void Write(uint8_t* baseAddress, CommandFifoFetchThreshhold_t value)
    {
        *((CommandFifoFetchThreshhold_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::CommandFifoFetchThreshhold)) = value;
    }

}

}