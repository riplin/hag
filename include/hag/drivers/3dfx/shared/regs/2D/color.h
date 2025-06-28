//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        ForegroundColor = 0x064,
        BackgroundColor = 0x060
    };
}

// The colorBack and colorFore registers specify the foreground and background colors used in solid-fill
// and monochrome bitmap operations, and operations using a monochrome pattern. The color registers
// must be stored in the destination color format.

// The following tables shows the format of the color registers for each destination format.

// P = palette index
// R = red color channel
// G = green color channel
// B = blue color channel

// Dst Format      Bits stored

// 8 bpp           0000_0000_0000_0000_0000_0000_PPPP_PPPP
// 15 bpp          0000_0000_0000_0000_ARRR_RRGG_GGGB_BBBB
// 16 bpp          0000_0000_0000_0000_RRRR_RGGG_GGGB_BBBB
// 24 bpp          0000_0000_RRRR_RRRR_GGGG_GGGG_BBBB_BBBB
// 32 bpp          AAAA_AAAA_RRRR_RRRR_GGGG_GGGG_BBBB_BBBB

typedef uint32_t Color_t;
namespace Color
{

    enum
    {
        Value = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}