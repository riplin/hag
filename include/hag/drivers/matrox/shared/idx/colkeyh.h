//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    ColorKeyHigh = 0x43
};

}

typedef uint8_t ColorKeyHigh_t;

// The switching for keying is performed according to the following equation:
//
// if ((ALPHAEN & VP<15>) == VS)            ;show graphics buffer
// elsif ((COLKEYMSK & GP) == COLKEY)       ;show video buffer
// else                                     ;show graphics buffer
//
// where:
//
// VP<15>       is bit 15 of the video stream
// GP           is the graphics stream
// vs           is the video select polarity (see the GeneralControl register)
// alphaen      is the alpha plane enable bit (see the GeneralControl register)
// COLKEYMSK    is the 16-bit key mask from the ColorKeyMaskHigh and
//              ColorKeyMaskLow registers
//              COLKEY is the 16-bit key from the ColorKeyHigh and ColorKeyLow registers
//
// Note: To always choose the graphics stream when in split frame buffer mode,
// program vs = 0 and alphaen = 0. To always choose the video stream when
// in split frame buffer mode, program vs = 1, alphaen = 0, XCOLKEY = 0,
// and XCOLKEYMSK = 0.
//
// The color key is also used for overlay keying in 32 bits/pixel single frame buffer mode
// (ColorDepth = ‘100’ / Bits32O8p) to specify the transparent color. The equation is:
//
// if (COLKEYMSK & ALPHA == COLKEY)          ;show the 24-bit direct stream
// else                                      ;show the overlay color LUT(ALPHA)
//
// where:
//
// ALPHA is the address of the overlay register (in that mode, the palette is used as 256
// overlay registers) and LUT(ALPHA) is the overlay color.
//
// The overlay can be disabled by programming COLKEYMSK = 0 and COLKEY = 0.

namespace ColorKeyHigh
{
    enum //Mask
    {
        MaskHigh = 0xFF         // Color key bits 15 to 8. The color key is used to perform color keying between graphics
                                // and the video buffer.
                                // Note: In 2G8V16 and 32 bits/pixel single frame buffer modes (ColorDepth = ‘100’ / Bits32O8p), the
                                // contents of this register should be set to all zeroes. (The ColorDepth field is located in the
                                // MultiplexControl register.)
    };

    namespace Shift
    {
        enum
        {
            MaskHigh = 0x00
        };
    }
}

}