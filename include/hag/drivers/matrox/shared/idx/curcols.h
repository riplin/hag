//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{
    
enum
{
    CursorColor0Red = 0x08,
    CursorColor0Green = 0x09,
    CursorColor0Blue = 0x0A,
    CursorColor1Red = 0x0C,
    CursorColor1Green = 0x0D,
    CursorColor1Blue = 0x0E,
    CursorColor2Red = 0x10,
    CursorColor2Green = 0x11,
    CursorColor2Blue = 0x12,
};

}
    
typedef uint8_t CursorColor_t;

namespace CursorColor
{
    enum //Mask
    {
        Color = 0xFF        // Cursor color register. The desired color register (0, 1, or 2) is chosen according to both
                            // the cursor mode and cursor map information. (See the CursorControl register for more
                            // information.) Each color register is 24 bits wide and contains an 8-bit red, 8-bit green,
                            // and 8-bit blue field.
    };

    namespace Shift
    {
        enum
        {
            Color = 0x00
        };
    }

}

}}}}