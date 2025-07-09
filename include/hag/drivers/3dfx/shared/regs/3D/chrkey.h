//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        ChromaKey = 0x134
    };
}

// chromaKey Register
// The chromaKey register specifies a color which is compared with all pixels to be written into the RGB
// buffer. If a color match is detected between an outgoing pixel and the chromaKey register, and chroma-
// keying is enabled (bit(1)=1 in the fbzMode register), then the pixel is not written into the frame buffer.
// An outgoing pixel will still be written into the RGB buffer if chroma-keying is disabled or the
// chromaKey color does not equal the outgoing pixel color. Note that the alpha color component of an
// outgoing pixel is ignored in the chroma-key color match circuitry. The chroma-key comparison is
// performed immediately after texture lookup, but before lighting, fog, or alpha blending. See the
// description of the fbzColorPath register for further information on the location of the chroma-key
// comparison circuitry. The format of chromaKey is a 24-bit RGB color.

typedef uint32_t ChromaKey_t;
namespace ChromaKey
{
    enum
    {
        Blue = 0x000000ff,
        Green = 0x0000ff00,
        Red = 0x00ff0000
     };

    namespace Shift
    {
        enum
        {
            Blue = 0x00,
            Green = 0x08,
            Red = 0x010
        };
    }
}

}