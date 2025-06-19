//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    ColorKeyLow = 0x42
};

}
    
typedef uint8_t ColorKeyLow_t;

namespace ColorKeyLow
{
    enum //Mask
    {
        MaskLow = 0xFF      // Color key bits 7 to 0. The color key is used to perform color keying between graphics
                            // and the video buffer (see ColorKeyHigh for more information on keying). It is also used
                            // in 32 bits/pixel single frame buffer mode (ColorDepth = ‘100’ / Bits32O8p) to specify the transparent
                            // color. See the ColorKeyHigh register description for more information.
    };

    namespace Shift
    {
        enum
        {
            MaskLow = 0x00
        };
    }
}

}