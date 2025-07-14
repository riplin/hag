//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    ColorKeyMaskHigh = 0x41
};

}
    
typedef uint8_t ColorKeyMaskHigh_t;

namespace ColorKeyMaskHigh
{
    enum //Mask
    {
        MaskHigh = 0xFF     // Color key mask bits 15 to 8. To prevent a particular bit plane from participating in a
                            // keying comparison, the corresponding color key mask bit should be set to 0b.
                            // Note: In 2G8V16 and 32 bits/pixel single frame buffer modes (ColorDepth =
                            // ‘100’ / Bits32O8p), this register must be set to all zeroes.
    };

    namespace Shift
    {
        enum
        {

        };
    }
}

}