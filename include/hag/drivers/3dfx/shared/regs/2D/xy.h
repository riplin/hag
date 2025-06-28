//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        SourceXY = 0x05c,
        DestinationXY = 0x06c
    };
}

// During screen-to-screen blts, the srcXY registers sets the position from which blt data will be read. Note
// that the starting position for a blit depends on the direction of the blt as shown in the table below. For
// lines and polylines, srcXY is the starting point of the first line segment. For polygons, the srcXY should
// be the topmost vertex of the polygon - that is, the vertex with the lowest y value. If there are multiple
// vertices sharing the lowest y value, the srcXY should be set to the leftmost vertex with that y value.
// Reading the srcXY register while in polygon mode will always return the last polygon vertex that the host
// sent for the left side of the polygon.

// The values in the srcXY are signed, however for blts srcXY must contain only positive values.

// During host-to-screen blts, only the x entry of the srcXY register is used. This entry determines the
// alignment of the initial pixel in the blt within the first dword sent from the host. For monochrome
// bitmaps, bits[4:0] are used to determine the bit position within the dword of the initial pixel. For color
// bitmaps, bits[1:0] are give the position within the dword of the first byte of pixel data. Host blts are
// always performed left-to-right (the x-direction bit in the command register is ignored), so the offset given
// will always be that of the leftmost pixel in the first span. The alignment of the initial pixel of all spans
// after the first is determined by adding the source stride (from the srcFormat register) to the alignment of
// the previous span.

// For blts, the dstXY should be the starting pixel of destination rectangle as shown in the table below. For
// line and polyline modes, the dstXY will be the endpoint of the first line segment.

// In polygon mode, the dstXY register is used to store the last vertex sent for the right side of the polygon.
// If command[8] is set when the command register is written in polygon mode, the value from srcXY will
// be copied to dstXY. If command[8] is cleared, dstXY can be written with the rightmost pixel in the top
// span of the polygon.

typedef uint32_t XY_t;
namespace XY
{

    enum
    {
        X =                     0x00000fff,
        Command =               0x0000c000,
        UpperLeft =             0x00000000,
        UpperRight =            0x00004000,
        LowerLeft =             0x00008000,
        LowerRight =            0x0000c000,
        Y =                     0x0fff0000,
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Command = 0x0e,
            Y = 0x10,
        };
    }

}

}