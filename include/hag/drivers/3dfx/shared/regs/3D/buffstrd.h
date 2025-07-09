//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        ColorBufferStride = 0x1f0,
        AuxBufferStride = 0x1f8
    };
}

// colBufferStride
// If the color buffer is linear (colBufferStride[15]=0) then colBufferStride[13:0] defines the linear stride of
// the color buffer in bytes. Linear stride must be 16-byte aligned. If the color buffer is tiled
// (colBufferStride[15]=1) then colBufferStride[6:0] defines the tile stride for the color buffer in tiles.

// auxBufferStride
// If the aux buffer is linear (auxBufferStride[15]=0) then auxBufferStride[13:0] defines the linear stride of
// the aux buffer in bytes. Linear stride must be 16-byte aligned. If the aux buffer is tiled
// (auxBufferStride[15]=1) then auxBufferStride[6:0] defines the tile stride for the aux buffer in tiles.

typedef uint32_t BufferStride_t;
namespace BufferStride
{
    enum
    {
        StrideLin = 0x00001fff,
        StrideTile = 0x0000007f,
        MemoryType = 0x00008000,
        MemoryLinear = 0x00000000,
        MemoryTiled = 0x00008000
    };

    namespace Shift
    {
        enum
        {
            StrideLin = 0x00,
            StrideTile = 0x00,
            MemoryType = 0x0f,
        };
    }
}

}