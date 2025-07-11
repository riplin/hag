//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        LineStyle = 0x040
    };
}

// The lineStyle register specifies how lines will be drawn.

// The bit pattern used for line stippling can be set to repeat every 1-32 bits, as set by the bit-mask size part
// of this register. The bit-mask size entry gives the number of bits *minus one* that will be used from the
// lineStipple register. Thus, if you want to use 2 bits to represent a dashed line, you would set the bit-mask
// size to 1.

// Each bit from the lineStipple register will determine the color or transparency of from 1-256 pixels. The
// repeat count determines the number of pixels along the line that will be drawn (or skipped) for each bit in
// the line pattern register. The number of pixels associated with each bit of the line pattern *minus one*
// must be written to the repeat count entry.

// The start position give the offset within the line pattern register for the first pixel drawn in a line. It
// consists of an integer index of the current bit in the line pattern, and a fractional offset that will determine
// the number of pixels that will be drawn using that bit of the pattern. The number of pixels drawn using
// the initial bit in the line pattern will equal the repeat count (i.e. the repeat count entry+1) minus the
// fractional part of the start position. The bit positions within the lineStipple registers are numbered
// starting with the lsb at 0, going up to the msb at 31.

// It is illegal to set the integer part of the stipple position to be greater than the bit-mask size. It is illegal to
// set the fractional part to be greater than the repeat count. If either part of the stipple position is too large,
// the behavior of the line drawing engine is undefined.

// Writing the lineStyle register will cause the stipple position to be loaded from the register. If the
// lineStyle register is not written to between the execution of two line commands, the stipple position at the
// start of the new line will be whatever if was after the completion of the last line. If the lineStyle register
// is read while the 2D engine is idle, the stipple position read will always be that which will be used in the
// next line operation - thus, if the lineStyle register has been written since the last stippled line was drawn
// the value written will be returned, otherwise the value that remained after the last stippled line will
// returned. Reading the lineStyle register while the 2D engine is not idle will return an indeterminable
// value for the stipple position.

// In the following examples,. ‘x’ represents a pixel colored with colorFore, ‘o’ represents a pixel colored
// with colorBack or that is transparent. ‘_S_’ Shows that the line engine is starting at bit 0 in the
// lineStipple register. ‘_’ shows that the line engine is using a new bit from the lineStipple register.

// Example
// Say the bit-mask size is set to 6 (thus, the entry in the register is 5) and the line pattern is:
// lineStipple <= 010111b

// The pixel pattern that will be repeated is:

// repeat_count        repeating pixel pattern
// 1                   x_x_x_o_x_o_S_x_x_x_o_x_o
// 2                   xx_xx_xx_oo_xx_oo_S_xx_xx_xx_oo_xx_oo
// 3                   xxx_xxx_xxx_ooo_xxx_ooo_S_xxx_xxx_xxx_ooo_xxx_ooo

// Example
// Say the repeat count is 5 (the register entry is 4), the integer part of the start position is 7, and the
// fractional part of the start position is 2. The color of the first 3 pixels drawn for the line will be
// determined by bit 7 in the line pattern register, the next 5 pixels will be determined by bit 8, and so on.

// lineStyle <= 07020904h
// lineStipple <= 1010110111b

// pixels generated, where x=colorFore and o=colorBack:

// xxx_ooooo_xxxxx_S_xxxxx_xxxxx_xxxxx_ooooo_xxxxx_xxxxx_ooooo_xxxxx_ooooo_xxxxx_S

// Pseudo code for line pixel generation
// Here is the pseudo-code for determining the color of pixels generated by the line engine:

// <bit_position> = <start_position_integer>
// <pixel_position> = <start_position_fraction>

// while (<need_another_pixel>)
// {
//     if ( <line_pattern> & (1 << <bit_position>) )
//     {
//         <new_pixel_color> = <colorFore>
//     }
//     else
//     {
//         if (<transparent>)
//         {
//             <new_pixel_color> = <transparent>
//         }
//         else
//         {
//             <new_pixel_color> = <colorBack>
//         }
//     }
//     if ( <pixel_position> == <repeat_count> )
//     {
//         <pixel_position> = 0
//         if (<bit_position> == <bit_mask_size>)
//         {
//             <bit_position> = 0;
//         }
//         else
//         {
//             <bit_position> = <bit_position> + 1
//         }
//     }
//     else
//     {
//         <pixel_position> = <pixel_position> +1
//     }
// }

typedef uint32_t LineStyle_t;
namespace LineStyle
{

    enum
    {
        RepeatCount =       0x000000ff,
        StippleSize =       0x00001f00,
        StartFrac =         0x00ff0000,// 23:16 Start position - fractional part
        StartInt =          0x1f000000,// 28:24 Start position - integer part
    };

    namespace Shift
    {
        enum
        {
            RepeatCount = 0x00,
            StippleSize = 0x08,
            StartFrac = 0x10,
            StartInt = 0x18
        };
    }
}

}