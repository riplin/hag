//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        BresenhamError0 = 0x028,
        BresenhamError1 = 0x02c
    };
}

// These registers allows the user to specify the initial Bresenham error terms used when performing line
// drawing, polygon drawing, and stretch blts. The Bresenham error terms are signed values.

// Bit 31 of each registers determines whether or not the error term given in the lower bits will be used. If
// this bit is 0, the line and stretch blt engines will generate the initial error term automatically. If the bit is
// set to 1, the error term given in bits 16-0 will be used. If a bresenham error register is used, the register
// should be written with bit[31] set to 0 after completion of the operation, so that subsequent operations will
// not be affected.

// bresError0 can be used to set the initial error value for lines, for the left edge of a polygon, and for blt
// stretching along the y-axis.

// bresError1 can be used to set the initial error value for the right edge of a polygon, and for blt stretching
// along the x-axis.

typedef uint32_t BresenhamError_t;
namespace BresenhamError
{

    enum
    {
        ErrorTerm = 0x0001ffff,
        Enable = 0x80000000
    };

    namespace Shift
    {
        enum
        {
            ErrorTerm = 0x00,
            Enable = 0x1f
        };
    }
}

}