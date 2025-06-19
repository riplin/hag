//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    ColorKeyMaskLow = 0x40
};

}
    
typedef uint8_t ColorKeyMaskLow_t;

namespace ColorKeyMaskLow
{
    enum //Mask
    {
        MaskLow = 0xFF      // Color key mask bits 7 to 0. To prevent a particular bit plane from participating in a
                            // keying comparison, the corresponding color key mask bit should be set to 0b.
                            // The mask is also used in 32 bits/pixel single frame buffer modes (depth = ‘100’) for
                            // overlay enable/disable. See the ColorKeyMaskHigh register description for more information.
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