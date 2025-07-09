//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        ChromaRange = 0x138
    };
}

// chromaRange Register
// The chromaRange register specifies a 24-bit RGB color value which is comared to all pixels to be written
// to the color buffer. If chroma-keying is enabled (fbzMode[1]) and chroma-ranging is enabled
// (chromaRange[28]), the outgoing pixel color is compared to a color range formed by the colors of the
// chromaKey and chromaRange registers.

// Each RGB color component of the chromaKey and chromaRange registers defines a chroma range for
// the color component The color component range includes the lower limit color from the chromaKey
// register and the upper limit color from the chromaRange register. Software must program the lower
// limits less-than or equal to the upper limits.

// Each RGB color component chromaRange mode defines the color component range as inclusive or
// exclusive. Inclusive ranges prohibit colors within the range and exclusive ranges prohibit colors outside
// of the range.

// Prohibited colors are blocked from the frame buffer based on the chromaRange mode. This mode may be
// set to “intersection” or “union”. The intersection mode blocks pixels prohibited by all of the color
// components and the union mode blocks pixels prohibited by any of the color components

typedef uint32_t ChromaRange_t;
namespace ChromaRange
{
    enum
    {
        BlueUpperLimit = 0x000000ff, // 7:0 Chroma-Range Blue Upper Limit
        GreenUpperLimit = 0x0000ff00, // 15:8 Chroma-Range Green Upper Limit
        RedUpperLimit = 0x00ff0000, // 23:16 Chroma-Range Red Upper Limit
        BlueMode = 0x01000000, // 24 Chroma-Range Blue Mode (0=inclusive; 1=exclusive)
        BlueInclusive = 0x00000000,
        BlueExclusive = 0x01000000,
        GreenMode = 0x02000000, // 25 Chroma-Range Green Mode (0=inclusive; 1=exclusive)
        GreenInclusive = 0x00000000,
        GreenExclusive = 0x02000000,
        RedMode = 0x04000000, // 26 Chroma-Range Red Mode (0=inclusive; 1=exclusive)
        RedInclusive = 0x00000000,
        RedExclusive = 0x08000000,
        BlockMode = 0x10000000, // 27 Chroma-Range Block Mode (0=intersection; 1=union)
        BlockModeIntersection = 0x00000000,
        BlockModeUnion = 0x10000000,
        Enable = 0x20000000 // 28 Chroma-Range Enable (0=disable; 1=enable)
    };

    namespace Shift
    {
        enum
        {
            BlueUpperLimit = 0x00,
            GreenUpperLimit = 0x08,
            RedUpperLimit = 0x10,
            BlueMode = 0x18,
            GreenMode = 0x19,
            RedMode = 0x1a,
            BlockMode = 0x1b,
            Enable = 0x1c
        };
    }
}

}