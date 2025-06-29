//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPGraphicsStride = 0x010
    };
}

// agpGraphicsStride defines the destination stride in bytes of the AGP transfer. Stride is in multiples of
// bytes. This register is read write, and defaults to 0.

typedef uint32_t AGPGraphicsStride_t;
namespace AGPGraphicsStride
{
    enum
    {
        FramebufferStride = 0x00007fff
    };

    namespace Shift
    {
        enum
        {
            FramebufferStride = 0x0
        };
    }

    inline AGPGraphicsStride_t Read(uint8_t* baseAddress)
    {
        return *((AGPGraphicsStride_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPGraphicsStride));
    }

    inline void Write(uint8_t* baseAddress, AGPGraphicsStride_t value)
    {
        *((AGPGraphicsStride_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPGraphicsStride)) = value;
    }

}

}