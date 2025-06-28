//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        Pattern0Alias = 0x044,
        Pattern1Alias = 0x048,
        Pattern = 0x100
    };
}

// The pattern registers contain an 8 pixel by 8 pixel pattern. The pixels must be either in the color format
// of the destination surface, or in 1bpp (monochrome) format. The pixels are to be written to the pattern
// registers in packed format. So, only registers 0 and 1 will be used for monochrome patterns, registers 0
// through 15 will be used when the destination is 8 bpp, registers 0 through 31 will be used when the
// destination is 16 bpp.

// Pixels should be written into the pattern registers starting with the upper left-hand corner of the pattern,
// proceeding horizontally left to right, and then vertically top to bottom. The least-significant bits of
// pattern[0] should always contain pixel(0,0) of a color pattern.

// The table below give the bit position of monochrome pixels within the pattern registers. The bits are
// numbered such that bit(0) represents the lsb of a register, and bit(31) represents the msb.

// Order of pixel storage in the pattern registers for a monochrome pattern

// pattern(0)
// Row 0           7   6   5   4   3   2   1   0
// Row 1           15  14  13  12  11  10  9   8
// Row 2           23  22  21  20  19  18  17  16
// Row 3           31  30  29  28  27  26  25  24
// pattern(1)
// Row 4           7   6   5   4   3   2   1   0
// Row 5           15  14  13  12  11  10  9   8
// Row 6           23  22  21  20  19  18  17  16
// Row 7           31  30  29  28  27  26  25  24

typedef uint32_t Pattern_t;
namespace Pattern
{
    enum
    {
        ColorData = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            ColorData = 0x00
        };
    }
}

}