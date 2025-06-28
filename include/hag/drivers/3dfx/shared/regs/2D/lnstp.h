//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        LineStipple = 0x03c
    };
}

// The line bit-mask register contains a mask that determines how lines will be drawn. Bits that are ones
// will be drawn with the color in the colorFore register. Bits that are zeros will be filled with the color in
// the colorBack register, or will not be filled, depending on the ‘transparent’ bit in the command register.
// The pattern in the bit mask can be set to repeat every 1-32 bits, as set by the bit-mask size part of the line
// style register. If the bit-mask size is set to less than 31, some of the bits of the line bit-mask will not be
// used, starting with the most-significant bit. For example, if the bit-mask size is set to 7, bits 0-7 of the
// lineStipple register will contain the line bit-mask.

typedef uint32_t LineStipple_t;
namespace LineStipple
{

    enum
    {
        BitMask = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            BitMask = 0x00
        };
    }
}

}