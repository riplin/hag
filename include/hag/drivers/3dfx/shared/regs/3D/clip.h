//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        ClipLeftRight = 0x118,
        ClipTopBottom = 0x11c,
        ClipLeftRight1 = 0x200,
        ClipTopBottom1 = 0x204
    };
}

// clipLeftRight and clipLowYHighY Registers
// The clip registers specify a rectangle within which all drawing operations are confined. If a pixel is to be
// drawn outside the clip rectangle, it will not be written into the RGB or depth buffers. Note that the
// specified clipping rectangle defines a valid drawing area in both the RGB and depth/alpha buffers. The
// values in the clipping registers are given in pixel units, and the valid drawing rectangle is inclusive of the
// clipleft and clipLowY register values, but exclusive of the clipRight and clipHighY register values.
// clipLowY must be less than clipHighY, and clipLeft must be less than clipRight. The clip registers can
// be enabled by setting bit(0) in the fbzMode register. When clipping is enabled, the bounding clipping
// rectangle must always be less than or equal to the screen resolution in order to clip to screen coordinates.
// Also note that if clipping is not enabled, rendering must not be specified to occur outside of the screen
// resolution.

// Important Note: The clipLowYHighY register is defined such that y=0 always resides at the top of the
// monitor screen. Changing the value of the Y origin bits (fbzMode bit(17) or lfbMode bit(13)) has no
// effect on the clipLowYHighY register orientation. As a result, if the Y origin is defined to be at the
// bottom of the screen (by setting one of the Y origin bits), care must be taken in setting the
// clipLowYHighY register to ensure proper functionality. In the case where the Y origin is defined to be at
// the bottom of the screen, the value of clipLowYHighY is usually set as the number of scan lines in the
// monitor resolution minus the desired Y clipping values.

// The clip registers are also used to define a rectangular region to be drawn during a FASTFILL command.
// Note that when clipLowYHighY is used to specify a rectangular region for the FASTFILL command, the
// orientation of the Y origin (top or bottom of the screen) is defined by the status of fbzMode bit(17). See
// section 7 and the fastfillCMD register description for more information on the FASTFILL command.

typedef uint32_t ClipLeftRight_t;
namespace ClipLeftRight
{
    enum
    {
        Right = 0x00000fff,
        Left = 0x0fff0000
    };

    namespace Shift
    {
        enum
        {
            Right = 0x00,
            Bottom = 0x10
        };
    }
}

typedef uint32_t ClipTopBottom_t;
namespace ClipTopBottom
{
    enum
    {
        Bottom = 0x00000fff,
        Top = 0x0fff0000
    };

    namespace Shift
    {
        enum
        {
            Top = 0x00,
            Bottom = 0x10
        };
    }
}

// clipLeftRight1, clipTopBottom1 Registers
// The clip0 and clip1 registers specify two rectangular regions which restrict drawing operation. The
// secondary clip rectangles may be defined as inclusive or exclusive through the clipMode field of the
// clipTopBottom register. An inclusive rectangle allows drawing within the rectangle and an exclusive
// rectangle disallows drawing within the rectangle. Drawing within an excluded region of either of the clip
// rectangles circumvents the write of pixels into both the color and auxiliary buffers.
// The clip registers define the four corners of a rectangular region in window relative pixel coordinates
// (native x/y rendering coordinates). The value of clipTop must be less than clipBottom and the value of
// clipLeft must be less than clipRight. This programming results in a rectangular region including the
// clipLeft and clipTop register values, but excluding the clipRight and clipBottom register values.

typedef uint32_t ClipLeftRight1_t;
namespace ClipLeftRight1
{
    enum
    {
        Right = 0x00000fff,
        Left = 0x0fff0000,
        Enable = 0x80000000
    };

    namespace Shift
    {
        enum
        {
            Right = 0x00,
            Bottom = 0x10,
            Enable = 0x1f
        };
    }
}

typedef uint32_t ClipTopBottom1_t;
namespace ClipTopBottom1
{
    enum
    {
        Bottom = 0x00000fff,
        Top = 0x0fff0000,
        Mode = 0x80000000,
        Inclusive = 0x00000000,
        Exclusive = 0x80000000
    };

    namespace Shift
    {
        enum
        {
            Top = 0x00,
            Bottom = 0x10,
            Mode = 0x1f
        };
    }
}

}