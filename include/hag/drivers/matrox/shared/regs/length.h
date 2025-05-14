//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Register
{
    
enum
{
    Length = 0x1C5C,
    LengthCommit = 0x1D5C
};

}

typedef uint32_t Length_t;

namespace Length
{

    enum //Mask
    {
        Length = 0x0000FFFF,        // Length. The length field is a 16-bit unsigned value.
                                    // - The length field does not require initialization for auto-init vectors.
                                    // - For a vector draw, length is programmed with the number of pixels to be drawn.
                                    // - For blits and trapezoid fills, length is programmed with the number of lines to be
                                    //   filled or blitted.
                                    // - To load the texture color palette, length is programmed with the number of locations
                                    //   in the LUT to be filled.
        Beta = 0xF0000000,          // Beta factor. This field is used to drive the vertical scaling in ILOAD_HIQHV (it is not
                                    // used for other opcodes). The beta field represents the four least significant bits of a
                                    // value between 1 and 16 (16 is 0000b), which represents a beta factor of 1/16 through
                                    // 16/16.
    };

    namespace Shift
    {
        enum
        {
            Length = 0x00,
            Beta = 0x1C
        };
    }
}

}}}
