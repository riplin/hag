//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        Command = 0x070
    };
}

// The command register sets the command mode for the 2D engine, as well as selecting a number of
// options.

// Bits (3:0) set the command mode for the 2D drawing engine as shown in the table below. If bit(8) is set,
// the command will be initiated as soon as the command register is written. If bit(8) is cleared, drawing
// will be initiated by a write to the launch area. For descriptions and examples of each command, see the
// 2D launch area section.

// Command[3:0]     Command
//      0           Nop - wait for idle
//      1           Screen to screen blt
//      2           Screen to screen stretch blt
//      3           Host to screen blt
//      4           Host to screen stretch blt
//      5           Rectangle fill
//      6           Line
//      7           Polyline
//      8           Polygon fill
//     13           Write Sgram Mode register
//     14           Write Sgram Mask register
//     15           Write Sgram Color register

// Setting Bit(9) makes line drawing reversible. If this bit is set, drawing a line from point A to point B will
// result in the same pixels being drawn as drawing a line from point B to point A.

// Bits(11:10) control the value placed in dstXY after each blt or rectangle fill command is executed. If
// bit(10) is 0, dst_x is unchanged. If bit(10) is 1, dst_x gets dst_x + dst_width. If bit(11) is 0, dst_y is
// unchanged. If bit(11) is 1, dst_y gets dst_y + dst_height.

// Bit(12) controls whether lines are stippled or solid. If bit(12) is 0, lines will be a solid color. If bit(12) is
// 1, lines will either be made up of either a two color pattern using colorFore and colorBack or will be a
// transparent stipple using colorFore, as determined by the transparency bit - bit(16).

// Bit(13) controls the format of the pattern data. If bit(13) is set to 0, the pattern must be stored in the
// destination format. If it is set to 1, the pattern will be stored as a monochrome bitmap; Pattern registers 0
// and 1 will be used as an 8x8x1bpp pattern, which will be expanded into the destination format using the
// colorBack and colorFore registers. Note that if Bit(13) is set, and Bit(16) is set to indicate that
// monochrome data is transparent, the pattern will be used to determine pixel transparency without regard
// to the contents of the ROP register.

// Bits(15:14) control the direction of blting during screen-to-screen copies. Note that the corner of the
// source and destination rectangles passed in the srcXY and dstXY registers will change depending on the
// blting direction. Bit(15) also controls the direction of blting for host-to-screen copies. This can be used to
// flip a pixel map so that the top span in host memory is drawn as the bottom span on the screen. Note that
// the direction bits only apply to “pure” screen to screen blits, but not to stretch blits. Also, destination and
// source color keying, along with color conversions, cannot be used with right to left blits.

// Bit(16) controls whether monochrome source bitmaps, and monochrome patterns will be transparent or
// opaque. When bit(16) is 0, source bitmaps are opaque; a 0 in the bitmap will result in colorBack being
// written to the destination. When bit(16) is 1, source bitmaps and monochrome patterns are transparent.
// In this case, a 0 in the bitmap will result in the corresponding destination pixel being left unchanged.

// The X and Y pattern offsets give the coordinates within the pattern of the pixel which corresponds to the
// destination pixel pointed to by the destination base address register. In other words, if a pattern fill is
// performed which covers the origin, pixel (0,0) in the destination pixel map will be written with the color
// in pattern pixel (x_pat_offset, y_pat_offset).

// Bit(23) controls whether the clip0 or clip1 registers will be used for clipping. When bit(31) is 0, clipping
// values from clip0Min and clip0Max will be used, when bit(31) is 1, clipping values from clip1Min and
// clip1Max will be used.

// Bits(31:24) contain ROP0, the ternary ROP that is used when colorkeying is disabled. For more
// information on ROPs, see the description of the rop register.

typedef uint32_t Command_t;
namespace Command
{

    enum
    {
        Command =                               0x0000000f,
        CommandNop =                            0x00000000,
        CommandScreenToScreenBlit =             0x00000001,
        CommandScreenToScreenStretchBlit =      0x00000002,
        CommandHostToScreenBlit =               0x00000003,
        CommandHostToScreenStretchBlit =        0x00000004,
        CommandRectangleFill =                  0x00000005,
        CommandLine =                           0x00000006,
        CommandPolyLine =                       0x00000007,
        CommandPolygonFill =                    0x00000008,
        CommandWriteSGRAMModeRegister =         0x0000000d,
        CommandWriteSGRAMMaskRegister =         0x0000000e,
        CommandWriteSGRAMColorRegister =        0x0000000f,
        InitiateCommand =                       0x00000100, // (1=initiate command immediately, 0 = wait for launch write)
        ReversibleLines =                       0x00000200, // (1=reversible, 0=non-reversible)
        IncrementDestinationXStart =            0x00000400, // Increment destination x-start after blt or rectangle command (1=increment, 0=don’t)
        IncrementDestinationYStart =            0x00000800, // Increment destination y-start after blt or rectangle command (1=increment, 0=don’t)
        LineStyle =                             0x00001000, // Stipple line mode (1 = stippled lines, 0 = solid lines)
        LineSolid =                             0x00000000,
        LineStipple =                           0x00001000,
        PatternFormat =                         0x00002000, // Pattern Format (1 = monochrome, 0 = color)
        PatternColor =                          0x00000000,
        PatternMonochrome =                     0x00002000,
        XDirection =                            0x00004000, // X direction (0 = left to right, 1 = right to left)
        YDirection =                            0x00008000, // Y direction (0 = top to bottom, 1 = bottom to top)
        TransparentMonochrome =                 0x00010000, // Transparent monochrome (1 = transparent, 0 = opaque)
        XPatternOffset =                        0x000e0000, // X pattern offset
        YPatternOffset =                        0x00700000, // Y pattern offset
        ClipSelect =                            0x00800000, // Clip select (0=clip0 registers, 1 = clip1 registers)
        RasterOperation0 =                      0xff000000  // Raster Operation 0
    };

    namespace Shift
    {
        enum
        {
            Command = 0x00,
            InitiateCommand = 0x08,
            ReversibleLines = 0x09,
            IncrementDestinationXStart = 0x0a,
            IncrementDestinationYStart = 0x0b,
            Stipple = 0x0c,
            PatternFormat = 0x0d,
            XDirection = 0x0e,
            YDirection = 0x0f,
            TransparentMonochrome = 0x10,
            XPatternOffset = 0x11,
            YPatternOffset = 0x14,
            ClipSelect = 0x17,
            RasterOperation0 = 0x18
        };
    }
}

}