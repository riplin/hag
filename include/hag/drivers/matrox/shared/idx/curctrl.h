//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    CursorControl = 0x06
};

}
    
typedef uint8_t CursorControl_t;

namespace CursorControl
{
    enum //Mask
    {
        Mode = 0x03,            // Cursor mode select. This field is used to disable or select the cursor mode, as shown below:
        Disabled = 0x00,        // Cursor Disabled (default)
        ThreeColor = 0x01,      // Three-color Cursor
        XGA = 0x02,             // XGA Cursor
        XWindows = 0x03,        // X-Windows Cursor
                                // Since the cursor maps (located in the frame buffer at the location pointed to by
                                // CursorBaseAddressHigh and CursorBaseAddressLow) use two bits to represent each pixel of the 64 x 64
                                // cursor, there are four possible ways to display each pixel of the cursor. The following
                                // table shows how the encoded pixel data is decoded, based on the cursor mode (set by
                                // curmode):
                                //
                                //        RAM
                                //  Plane 1   Plane 0       Three-Color         XGA             X-Windows
                                //     0        0          Transparent(1)   Cursor Color 0     Transparent
                                //     0        1          Cursor Color 0   Cursor Color 1     Transparent
                                //     1        0          Cursor Color 1    Transparent      Cursor Color 0
                                //     1        1          Cursor Color 2   Complement(2)     Cursor Color 1
                                //
                                // (1) = Underlying color is displayed (that is, the cursor has no effect on the display)
                                // (2) = Each bit of the underlying pixel is inverted, then displayed.
    };

    namespace Shift
    {
        enum
        {
            Mode = 0x00
        };
    }
}

}