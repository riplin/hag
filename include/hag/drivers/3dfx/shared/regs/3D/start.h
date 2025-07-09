//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fp.h>
#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        StartR = 0x020,         // Starting Vertex-A Color information (fixed point two’s complement 12.12 format)
        StartG = 0x024,         // Starting Vertex-A Color information (fixed point two’s complement 12.12 format)
        StartB = 0x028,         // Starting Vertex-A Color information (fixed point two’s complement 12.12 format)
        StartZ = 0x02c,         // Starting Vertex-A Z information (fixed point two’s complement 20.12 format)
        StartA = 0x030,         // Starting Vertex-A Color information (fixed point two’s complement 12.12 format)
        StartS = 0x034,         // Starting Vertex-A Texture coordinates (fixed point two’s complement 14.18 format)
        StartT = 0x038,         // Starting Vertex-A Texture coordinates (fixed point two’s complement 14.18 format)
        StartW = 0x03c,         // Starting Vertex-A W information (fixed point two’s complement 2.30 format)
        FloatStartR = 0x0a0,    // Starting Vertex-A Color information (IEEE 32-bit single-precision floating point format)
        FloatStartG = 0x0a4,    // Starting Vertex-A Color information (IEEE 32-bit single-precision floating point format)
        FloatStartB = 0x0a8,    // Starting Vertex-A Color information (IEEE 32-bit single-precision floating point format)
        FloatStartZ = 0x0ac,    // Starting Vertex-A Z information (IEEE 32-bit single-precision floating point format)
        FloatStartA = 0x0b0,    // Starting Vertex-A Color information (IEEE 32-bit single-precision floating point format)
        FloatStartS = 0x0b4,    // Starting Vertex-A Texture coordinates (IEEE 32-bit single-precision floating point format)
        FloatStartT = 0x0b8,    // Starting Vertex-A Texture coordinates (IEEE 32-bit single-precision floating point format)
        FloatStartW = 0x0bc,    // Starting Vertex-A W information (IEEE 32-bit single-precision floating point format)
    };
}

namespace Alias
{
    enum
    {
        StartR = 0x020,
        StartG = 0x02c,
        StartB = 0x038,
        StartZ = 0x044,
        StartA = 0x050,
        StartS = 0x05c,
        StartT = 0x068,
        StartW = 0x074,
        FloatStartR = 0x020,
        FloatStartG = 0x02c,
        FloatStartB = 0x038,
        FloatStartZ = 0x044,
        FloatStartA = 0x050,
        FloatStartS = 0x05c,
        FloatStartT = 0x068,
        FloatStartW = 0x074
    };
}

// startR, startG, startB, startA, fstartR, fstartG, fstartB, and fstartA Registers
// The startR, startG, startB, startA, fstartR, fstartG, fstartB, and fstartA registers specify the starting
// color information (red, green, blue, and alpha) of a triangle to be rendered. The start registers must
// contain the color values associated with the A vertex of the triangle. The fstart registers are floating
// point equivalents of the start registers. Banshee automatically converts both the start and fstart registers
// into an internal fixed point notation used for rendering.

// startZ and fstartZ registers
// The startZ and fstartZ registers specify the starting Z information of a triangle to be rendered. The
// startZ registers must contain the Z values associated with the A vertex of the triangle. The fstartZ
// register is a floating point equivalent of the startZ registers. Banshee automatically converts both the
// startZ and fstartZ registers into an internal fixed point notation used for rendering.

// startS, startT, fstartS, and fstartT Registers
// The startS, startT, fstartS, and fstartT registers specify the starting S/W and T/W texture coordinate
// information of a triangle to be rendered. The start registers must contain the texture coordinates
// associated with the A vertex of the triangle. Note that the S and T coordinates used by Banshee for
// rendering must be divided by W prior to being sent to Banshee (i.e. Banshee iterates S/W and T/W prior
// to perspective correction). During rendering, the iterated S and T coordinates are (optionally) divided by
// the iterated W parameter to perform perspective correction. The fstart registers are floating point
// equivalents of the start registers. Banshee automatically converts both the start and fstart registers into
// an internal fixed point notation used for rendering.

// startW and fstartW registers
// The startW and fstartW registers specify the starting 1/W information of a triangle to be rendered. The
// startW registers must contain the W values associated with the A vertex of the triangle. Note that the W
// value used by Banshee for rendering is actually the reciprocal of the 3D-geometry-calculated W value (i.e.
// Banshee iterates 1/W prior to perspective correction). During rendering, the iterated S and T coordinates
// are (optionally) divided by the iterated W parameter to perform perspective correction. The fstartW
// register is a floating point equivalent of the startW registers. Banshee automatically converts both the
// startW and fstartW registers into an internal fixed point notation used for rendering.


typedef Fixed12p12_t Start12p12_t;
namespace Start12p12
{
    using namespace Fixed12p12;

    namespace Shift
    {
        using namespace Fixed12p12::Shift;
    }
}

typedef Fixed14p18_t Start14p18_t;
namespace Start14p18
{
    using namespace Fixed14p18;

    namespace Shift
    {
        using namespace Fixed14p18::Shift;
    }
}

typedef Fixed2p30_t Start2p30_t;
namespace Start2p30
{
    using namespace Fixed2p30;

    namespace Shift
    {
        using namespace Fixed2p30::Shift;
    }
}

}