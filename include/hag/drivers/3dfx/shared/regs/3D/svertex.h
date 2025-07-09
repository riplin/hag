//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        sVertexX = 0x264,           // Vertex coordinate information (IEEE 32 bit single-precision floating point format)
        sVertexY = 0x268,           // Vertex coordinate information (IEEE 32 bit single-precision floating point format)
        sVertexRed = 0x270,         // Red value at vertex (0.0 - 255.0). (IEEE 32 bit single-precision floating point format)
        sVertexGreen = 0x274,       // Green value at vertex (0.0 - 255.0). (IEEE 32 bit single-precision floating point format)
        sVertexBlue = 0x278,        // Blue value at vertex (0.0 - 255.0). (IEEE 32 bit single-precision floating point format)
        sVertexAlpha = 0x27c,       // Alpha value at vertex (0.0 - 255.0). (IEEE 32 bit single-precision floating point format)
        sVertexZ = 0x280,           // Vertex coordinate information (IEEE 32 bit single-precision floating point format)
        sVertexW = 0x284,           // Global 1/W. (IEEE 32 bit single-precision floating point format).
        sVertexWTmu0 = 0x288,       // Texture local 1/W. (IEEE 32 bit single-precision floating point format)
        sVertexSdW0 = 0x28c,        // Texture S coordinate (IEEE 32 bit single-precision floating point format)
        sVertexTdW0 = 0x290,        // Texture T coordinate (IEEE 32 bit single-precision floating point format)
        sVertexWTmu1 = 0x294,       // Texture local 1/W. (IEEE 32 bit single-precision floating point format)
        sVertexSdWTmu1 = 0x298,     // Texture local 1/W. (IEEE 32 bit single-precision floating point format)
        sVertexTdWTmu1 = 0x29c      // Texture local 1/W. (IEEE 32 bit single-precision floating point format)
    };
}

typedef float sVertex_t;
namespace sVertex
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
}

}