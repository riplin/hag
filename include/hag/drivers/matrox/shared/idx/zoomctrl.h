//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{

enum
{
    ZoomControl = 0x38
};

}
    
typedef uint8_t ZoomControl_t;

namespace ZoomControl
{
    enum //Mask
    {
        HorizontalZoom = 0x03,      // Horizontal zoom factor. Specifies the (zoom) factor used to replicate pixels in the
                                    // horizontal display line.
                                    // Note: The cursor is not affected by the hzoom bits (the cursor is never zoomed).
                                    // The following factors are supported:
        Zoom1x = 0x00,              // 00 = 1x (default)
        Zoom2x = 0x01,              // 01 = 2x
        Zoom4x = 0x03               // 11 = 4x
    };

    namespace Shift
    {
        enum
        {
            HorizontalZoom = 0x00
        };
    }
}

}}}}