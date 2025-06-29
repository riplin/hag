//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPRequestSize = 0x000
    };
}

// agpReqSize defines the AGP packet transfer size. The maximum transfer size is 4-Mbyte block of data.
// This register is read write and has no default value.

typedef uint32_t AGPRequestSize_t;
namespace AGPRequestSize
{
    enum
    {
        Size = 0x000fffff
    };

    namespace Shift
    {
        enum
        {
            Size = 0x00
        };
    }

    inline AGPRequestSize_t Read(uint8_t* baseAddress)
    {
        return *((AGPRequestSize_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPRequestSize));
    }

    inline void Write(uint8_t* baseAddress, AGPRequestSize_t value)
    {
        *((AGPRequestSize_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPRequestSize)) = value;
    }

}

}