//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        Color0 = 0x144,
        Color1 = 0x148
    };
}

// color0 Register
// The color0 register specifies constant color values which are used for certain rendering functions. In
// particular, bits(23:0) of color0 are optionally used as the c_local input in the color combine unit. In
// addition, bits(31:24) of color0 are optionally used as the c_local input in the alpha combine unit. See the
// fbzColorPath register description for more information.

// color1 Register
// The color1 register specifies constant color values which are used for certain rendering functions. In
// particular, bits(23:0) of color1 are optionally used as the c_other input in the color combine unit selected
// by bits(1:0) of fbzColorPath. The alpha component of color1(bits(31:24)) are optionally used as the
// a_other input in the alpha combine unit selected by bits(3:2) of fbzColorPath. The color1 register
// bits(23:0) are also used by the FASTFILL command as the constant color for screen clears. Also, for
// linear frame buffer write format 15(16-bit depth, 16-bit depth), the color for the pixel pipeline is taken
// from color1 if the pixel pipeline is enabled for linear frame buffer writes (lfbMode bit(8)=1).

typedef uint32_t Color_t;
namespace Color
{
    enum
    {
        Blue = 0x000000ff,
        Green = 0x0000ff00,
        Red = 0x00ff0000,
        Alpha = 0xff000000
    };

    namespace Shift
    {
        enum
        {
            Blue = 0x00,
            Green = 0x08,
            Red = 0x10,
            Alpha = 0x18
        };
    }
}

}