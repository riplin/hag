//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        LaunchArea = 0x080
    };
}

typedef uint32_t LaunchCommand_t;


// Screen-to-screen Blt Mode
// Writing the launch area while in screen-to-screen blt mode results in a rectangle being copied from one
// area of display memory to another. The position of the source rectangle is given by the write to the launch
// area. The write to the launch area will be used to fill the srcXY register.

typedef LaunchCommand_t ScreenBlitLaunch_t;
namespace ScreenBlitLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

// Screen-to-screen Stretch Blt Mode
// Writing the launch area while in screen-to-screen blt mode results in pixels being copied from rectangle in
// display memory to another of a different size. The write to the launch area will be used to fill the srcXY
// register. The x and y direction bits do not apply to stretch blits. I.e., only top-down, left-to-right stretch
// blits can be done.

typedef LaunchCommand_t ScreenStretchBlitLaunch_t;
namespace ScreenStretchBlitLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

// Host-to-screen Blt Mode
// In host-to-screen blt mode, writes to the launch area should contain packed pixels to be used as source
// data. When performing a host-to-screen blt, the blt engine does not generate source addresses. However,
// it is still necessary for the driver to specify the srcFormat, in order for the blt engine to determine how
// the source data is packed. The driver must also write the srcXY register in order to specify the first byte
// or bit to use from the first dword. In monochrome source mode, the 5 lsbs will specify the initial bit. In all
// other modes, the 2 lsbs of srcXY will specify the initial byte of the initial span. The alignment of the first
// pixel of each span after the first is determined by adding the source stride (from the srcFormat register)
// to the alignment of the previous span.

// If more data is written to the launch area than is required for the host blt specified, the extra data will be
// discarded, or may be used in the following host blt, if it is requested while the 2D is operating on the first
// hblt. If too little data is written to the launch area, the hblt will be aborted, and pixels on an incomplete
// span at the end of the host blt may or may not be drawn.


// Host Blt Example 1

// In this example, the driver is drawing text to a 1024x768x16bpp screen using monochrome bitmaps of
// various widths. The monochrome data is packed, with each row byte aligned. First, it sets up the
// necessary registers before giving the data specific to the first blt:

// colorBack <= the background color
// colorFore <= the foreground color
// dstXY <= the starting position of the first character
// dstBaseAddr <= base address of the primary surface
// clip0Min <= 0x00000000
// clip0Max <= 0xFFFFFFFF
// command <= SRC_COPY || HOST_BLT_MODE = 0xCC000003
// dstFormat <= 0x00030800
// srcFormat <= 0x00400000

// The command mode is set to host-to-screen blt, with all other features disabled. Since colorkeying is
// disabled, only ROP0 is needed. The format register sets the host format to unswizzled monochrome,
// using byte-packing. This means that the stride will not have to be set for each blt, but will be set to the
// number of bytes required to store the number of pixels in the source width (Since this is not a stretch blt,
// the source width equals the destination width, as set later in the dstSize register). The clip registers are
// set such that the results will not be clipped. Although this is a host to screen blt, the srcXY register must
// be set in order to specify the initial alignment of the bitmask. For this example, the source data begins
// with the lsb of the first dword of host data, so the srcXY register is set to zero.

// Now, the driver is ready to start the first blt. It will blt a 11x7 pixel character.

// dstSize <= 0x0007000B
// srcXY <= 0x00000000
// launch <= 0xc0608020
// launch <= 0xC460C060
// launch <= 0x3B806EC0
// launch <= 0x00001100


// Host Blt Example 2

// In this example, the driver is drawing a pixel map

// colorBack <= the background color
// colorFore <= the foreground color
// dstXY <= the starting position of the first character
// clip0Min <= 0x00000000
// clip0Max <= 0xFFFFFFFF
// command <= SRC_COPY || HOST_BLT_MODE = 0xCC000003
// srcFormat <= 0x00240000

// The command mode is set to host-to-screen blt, with all other features disabled. Since colorkeying is
// disabled, only ROP0 is needed. The format register sets the host format to unswizzled monochrome,
// using byte-packing. This means that the stride will not have to be set for each blt, but will be set to the
// number of bytes required to store the number of pixels in the source width (Since this is not a stretch blt,
// the source width equals the destination width, as set later in the dstSize register). The clip registers are
// set such that the results will not be clipped. Although this is a host to screen blt, the srcXY register must
// be set in order to specify the initial alignment of the bitmask. For this example, the source data begins
// with the lsb of the first dword of host data, so the srcXY register is set to zero.

// Now, the driver is ready to start the first blt. It will blt a 11x7 pixel character.

// dstXY <= 0x0007000B
// srcXY <= 0x00000000
// launch <= 1st 2 rows
// launch <= 2nd 2 rows
// launch <= 3rd 2 rows
// launch <= last row

typedef LaunchCommand_t HostBlitLaunch_t;
namespace HostBlitLaunch
{

    enum
    {
        HostPixelData = 0xffffffff      // Source pixel data
    };

    namespace Shift
    {
        enum
        {
            HostPixelData = 0x00,
        };
    }
}

// Host-to-screen Stretch Blt Mode
// Writing the launch area while in host-to-screen blt mode results in the pixels written to the launch area
// being stretched onto the destination rectangle. Pixel data for Host-to-screen stretch blts is written just as
// for non-stretched host-to-screen blts, except when the destination height differs from the source height. In
// this case, the host must replicate or decimate the source spans to match the number of destinations spans
// required.

typedef LaunchCommand_t HostStretchBlitLaunch_t;
namespace HostStretchBlitLaunch
{

    enum
    {
        HostPixelData = 0xffffffff      // Source pixel data
    };

    namespace Shift
    {
        enum
        {
            HostPixelData = 0x00,
        };
    }
}

// Rectangle Fill Mode
// Rectangle fill mode is similar to screen-to-screen blt mode, but in this mode, the colorFore register is
// used as source data rather than data from display memory. The size of the rectangle is determined by the
// dstSize register. The write to the launch area gives the position of the destination rectangle, which is
// used to fill the dstXY register.

typedef LaunchCommand_t RectangleFillLaunch_t;
namespace RectangleFillLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

// Line Mode
// Writing the launch area while in line mode will write the launch data to the dstXY register and draw a
// line from srcXY to dstXY. After the line has been drawn, dstXY is copied to srcXY. In line mode, all
// pixels in the line will be drawn (as specified by the line style register), including both the start and
// endpoint.

// The ROP used for lines can use the pattern and the destination, but not source data. colorFore will be
// used in the ROP in place of source data. Source colorkeying must be turned off, destination colorkeying is
// allowed.

// Line drawing example

// srcXY <= 0x00020003                             // line start-point = (3, 2)
// lineStipple <= 0x00000006                       // bit mask is 110 binary
// lineStyle <= 0x02010202                         // start position = 2 1/3, repeat count = 2, bit-mask size=2
// colorBack <= BLACK
// colorFore <= GREY
// command <= LINE_MODE || OPAQUE
// launch <= 0x000c0016                            // line end-point = (22,12)

// The line drawn will appear as shown below:
// See documentation for image.

typedef LaunchCommand_t LineLaunch_t;
namespace LineLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

// Polyline Mode
// Writing the launch area while in line mode will write the launch data to the dstXY register and draw a
// line from srcXY to dstXY. After the line has been drawn, dstXY is copied to srcXY. In polyline mode,
// the endpoint of the line (the pixel at dstXY) will not be written. This ensures that each pixel in a non-
// overlapping polyline will be written only once.

// The ROP used for lines can use the pattern and the destination, but not source data. colorFore will be
// used in the ROP in place of source data. Source colorkeying must be turned off, destination colorkeying is
// allowed.

typedef LaunchCommand_t PolyLineLaunch_t;
namespace PolyLineLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

// Polygon Fill Mode
// The polygon fill mode can be used to draw simple polygons. A polygon may be drawn using the method
// described below if no horizontal span intersects more than two non-horizontal polygon edges. Polygons
// are drawn by first determining the top vertex - that is the vertex with the lowest y coordinate. The
// coordinates of this vertex should be written to the srcXY register. If multiple vertices share the lowest y
// coordinate, any vertex with the lowest y coordinate may be used as the starting point. If command[8] is
// set when the command register is written when command[3:0] indicates polygon mode, the value in the
// srcXY register will be copied to the dstXY register. The value in the srcXY register determines the
// starting point for the left side of the polygon, while the value in the dstXY register determines the starting
// point for the right side of the polygon. If bit[8] of the command register is not set, the starting position of
// the right side of the polygon can be set by writing to the dstXY register.

// Once the starting vertex is set, as well as the desired colors, ROP, pattern, and options for the polygon fill,
// the polygon can be drawn by writing polygon vertices to the launch area. When multiple vertices share
// the lowest y coordinate, the starting vertex chosen will determine which of those vertices are on the ‘right’
// edge of the polygon and which are on the ‘left’ edge. Pixels with the same y value as the starting point
// are on the left edge if they are to the left of the starting point.

// For optimum performance, software should determine the leftmost and rightmost of all vertices that share
// the lowest y coordinate. The coordinates of the leftmost vertex should be written to srcXY and the
// coordinates of the rightmost vertex should be written to dstXY. When the command register is written,
// command[8] (the ‘start command’ bit) should be low.

// In Polygon fill mode, polygon vertices should be written to the launch area in order of increasing y value.
// Whenever 2 vertices share the same y value, the leftmost vertex *must* be written first. The driver should
// keep track of the last y value sent for the left and right sides. If the y value for the last vertex sent for the
// left side is *less than or equal to* the last y value sent for the right side, the next vertex on the left side
// should be written to the launch area. Otherwise, the next vertex for the right side should be written to the
// launch area.

// The ROP used for filling polygons can use the pattern and the destination, but not source data. colorFore
// will be used in the ROP in place of source data. Source colorkeying must be turned off, destination
// colorkeying is allowed.

// Pixels that are on the line that forms the left edge of the polygon will be drawn. Pixels that fall on the line
// that forms the right edge of the polygon will not be drawn. For Horizontal edges, pixels on a horizontal
// polygon edge that is on the ‘top’ of the polygon (i.e. above the edge is outside the polygon and below the
// edge is inside the polygon) will be drawn, while pixels on a horizontal polygon edge that is on the bottom
// of the polygon will not be drawn.

// Polygon drawing example
// As an example of polygon drawing, say we are drawing the polygon shown in figure 2. Traversing the
// vertex list in counterclockwise order gives the following list of vertices:

// (4, 1) (2, 4) (3, 6) (1, 6) (2,8) (5, 11) (8,8) (13,8) (11,6) (11,3) (10,1)

// Figures 2a through 2m show the steps in drawing the polygon. Filled circles are vertices of the left
// polygon edge. Open circles are vertices of the right polygon edge. Pixels that are drawn at the end of
// each step are shaded in the figures.

// The polygon engine keeps track of four vertices at a time. The top vertex of the current left polygon edge
// (L0), the bottom vertex of the current left polygon edge (L1), the top vertex of the current right polygon
// edge (R0), and the bottom vertex of the current right polygon edge (R1). The values of these variables at
// each step in drawing the polygon are shown in the figures. The arrows in the figures indicate when a
// variable changes between the start of the step and the end of pixel filling for that step.

// Figure 2
// See documentation for image.


// First, all required registers must be written, including the dstFormat register to specify the drawing
// surface, color or pattern registers, and the command register. Write the coordinates of the starting vertex
// (4, 1) to the srcXY register:

// srcXY <= 0x00010004
// command <= POLYGON_MODE || INITIA TE_COMMAND

// Figure 2a
// See documentation for image.


// R1.y>=L1.y, so we have to write the next vertex for the left edge (2, 4):
// launch <= 0x00040002

// Figure 2b
// See documentation for image.


// R1.y<L1.y, so we write the next vertex for the right edge (10, 2). The drawing engine now has edges for
// both the left and right edges. So, it will draw all spans up to min(R1.y, L1.y). Because R1.y=R0.y, no
// pixels will be drawn, but R0 will be updated to vertex R1:

// launch <= 0x0001000a

// Figure 2c
// See documentation for image.


// R1.y<L1.y, so we again write the next vertex on the right polygon edge (11, 3). Pixels on all spans from
// max(L0.y, R0.y) to min(L1.y, R1.y)-1 will be drawn, as shown below. Because R1.y<L1.y, R0 is updated
// to R1.

// launch <= 0x0003000b

// Figure 2d
// See documentation for image.


// R1.y<L1.y, so we write the next vertex on the right edge (11, 6). Again, pixels on all spans from
// max(L0.y, R0.y) to min(L1.y, R1.y)-1 will be drawn. This time R1.y>L1.y, however, so L0 is updated to
// L1.

// launch <= 0x0006000b

// Figure 2e
// See documentation for image.


// R1.y>=L1.y, so we write the next vertex on the left edge (3, 6). L1.y=R1.y, so R0 is updated to R1 and L0
// is updated to L1.

// launch <= 0x00060003

// Figure 2f
// See documentation for image.


// R1.y>=L1.y, so we write the next vertex on the left edge (1, 6). L1.y=R1.y, so R0 is updated to R1 and L0
// is updated to L1. R1 did not change, so updating R0 to R1 has no effect.

// launch <= 0x00060001

// Figure 2g
// See documentation for image.


// R1.y>=L1.y, so we again write the next vertex on the left edge (2, 8). L1.y>R1.y, so R0 is updated to R1,
// again with no effect.

// launch <= 0x00080002

// Figure 2h
// See documentation for image.


// R1.y<L1.y, so we write the next vertex on the right edge (11, 8). L1.y=R1.y, so R0 is updated to R1, and
// L0 is updated to L1.

// launch <= 0x0008000b

// Figure 2i
// See documentation for image.


// R1.y>=L1.y, so we write the next vertex on the left edge (5, 11). L1.y>R1.y, so R0 is updated to R1.

// launch <= 0x000b0005

// Figure 2j
// See documentation for image.


// R1.y<L1.y, so we write the next vertex on the right edge (8, 8). L1.y>R1.y, so R0 is updated to R1, but no
// pixels are drawn.

// launch <= 0x00080008

// Figure 2k
// See documentation for image.


// R1.y<L1.y, so we write the next vertex on the right edge. This is the final vertex in the polygon, which
// doesn’t have a horizontal span at the bottom, so this vertex is the same as the last vertex for the left edge
// (5, 11). L1.y=R1.y, so R0 is updated to R1, and L0 is updated to L1. No pixels on the final span are
// drawn (this would be true even if L1.x did not equal R1.x). If the launch area is written again before any
// registers are written the polygon engine will begin a new polygon starting at (5,11).

// launch <= 0x000b0005

// Figure 2m
// See documentation for image.

typedef LaunchCommand_t PolygonLaunch_t;
namespace PolygonLaunch
{

    enum
    {
        X = 0x00001fff,     // X position of the source rectangle
        Y = 0x1fff0000      // Y position of the source rectangle
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

}