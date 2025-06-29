//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        YUVStride = 0x104,
    };
}

// yuvStride register contains the destination stride value of the U and V planes.

typedef uint32_t YUVStride_t;
namespace YUVStride
{
    enum
    {
        Stride = 0x00003fff,    // Y, U and V stride register. Default is 0x0.
        Tiled = 0x80000000      // Destination is tiled (0 = linear, 1 = tiled) . Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Stride = 0x00,
            Tiled = 0x1f
        };
    }

    inline YUVStride_t Read(uint8_t* baseAddress)
    {
        return *((YUVStride_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::YUVStride));
    }
    
    inline void Write(uint8_t* baseAddress, YUVStride_t value)
    {
        *((YUVStride_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::YUVStride)) = value;
    }

}

}