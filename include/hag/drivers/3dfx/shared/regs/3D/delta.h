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
        DeltaRDeltaX = 0x040,       // Change in color with respect to X (fixed point two’s complement 12.12 format)
        DeltaGDeltaX = 0x044,       // Change in color with respect to X (fixed point two’s complement 12.12 format)
        DeltaBDeltaX = 0x048,       // Change in color with respect to X (fixed point two’s complement 12.12 format)
        DeltaZDeltaX = 0x04c,       // Change in Z with respect to X (fixed point two’s complement 20.12 format)
        DeltaADeltaX = 0x050,       // Change in color with respect to X (fixed point two’s complement 12.12 format)
        DeltaSDeltaX = 0x054,       // Change in S with respect to X (fixed point two’s complement 14.18 format)
        DeltaTDeltaX = 0x058,       // Change in T with respect to X (fixed point two’s complement 14.18 format)
        DeltaWDeltaX = 0x05c,       // Change in W with respect to X (fixed point two’s complement 2.30 format)
        DeltaRDeltaY = 0x060,       // Change in color with respect to Y (fixed point two’s complement 12.12 format)
        DeltaGDeltaY = 0x064,       // Change in color with respect to Y (fixed point two’s complement 12.12 format)
        DeltaBDeltaY = 0x068,       // Change in color with respect to Y (fixed point two’s complement 12.12 format)
        DeltaZDeltaY = 0x06c,       // Change in Z with respect to Y (fixed point two’s complement 20.12 format)
        DeltaADeltaY = 0x070,       // Change in color with respect to Y (fixed point two’s complement 12.12 format)
        DeltaSDeltaY = 0x074,       // Change in S with respect to Y (fixed point two’s complement 14.18 format)
        DeltaTDeltaY = 0x078,       // Change in T with respect to Y (fixed point two’s complement 14.18 format)
        DeltaWDeltaY = 0x07c,       // Change in W with respect to Y (fixed point two’s complement 2.30 format)
        FloatDeltaRDeltaX = 0x0c0,  // Change in color with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaGDeltaX = 0x0c4,  // Change in color with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaBDeltaX = 0x0c8,  // Change in color with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaZDeltaX = 0x0cc,  // Change in Z with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaADeltaX = 0x0d0,  // Change in color with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaSDeltaX = 0x0d4,  // Change in S with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaTDeltaX = 0x0d8,  // Change in T with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaWDeltaX = 0x0dc,  // Change in W with respect to X (IEEE 32-bit single-precision floating point format)
        FloatDeltaRDeltaY = 0x0e0,  // Change in color with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaGDeltaY = 0x0e4,  // Change in color with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaBDeltaY = 0x0e8,  // Change in color with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaZDeltaY = 0x0ec,  // Change in Z with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaADeltaY = 0x0f0,  // Change in color with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaSDeltaY = 0x0f4,  // Change in S with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaTDeltaY = 0x0f8,  // Change in T with respect to Y (IEEE 32-bit single-precision floating point format)
        FloatDeltaWDeltaY = 0x0fc,  // Change in W with respect to Y (IEEE 32-bit single-precision floating point format)
    };
}

namespace Alias
{
    enum
    {
        DeltaRDeltaX = 0x024,
        DeltaGDeltaX = 0x030,
        DeltaBDeltaX = 0x03c,
        DeltaZDeltaX = 0x048,
        DeltaADeltaX = 0x054,
        DeltaSDeltaX = 0x060,
        DeltaTDeltaX = 0x06c,
        DeltaWDeltaX = 0x078,
        DeltaRDeltaY = 0x028,
        DeltaGDeltaY = 0x034,
        DeltaBDeltaY = 0x040,
        DeltaZDeltaY = 0x04c,
        DeltaADeltaY = 0x058,
        DeltaSDeltaY = 0x064,
        DeltaTDeltaY = 0x070,
        DeltaWDeltaY = 0x07c,
        FloatDeltaRDeltaX = 0x0a4,
        FloatDeltaGDeltaX = 0x0b0,
        FloatDeltaBDeltaX = 0x0bc,
        FloatDeltaZDeltaX = 0x0c8,
        FloatDeltaADeltaX = 0x0d4,
        FloatDeltaSDeltaX = 0x0e0,
        FloatDeltaTDeltaX = 0x0ec,
        FloatDeltaWDeltaX = 0x0f8,
        FloatDeltaRDeltaY = 0x0a8,
        FloatDeltaGDeltaY = 0x0b4,
        FloatDeltaBDeltaY = 0x0c0,
        FloatDeltaZDeltaY = 0x0cc,
        FloatDeltaADeltaY = 0x0d8,
        FloatDeltaSDeltaY = 0x0e4,
        FloatDeltaTDeltaY = 0x0f0,
        FloatDeltaWDeltaY = 0x100,
    };
}

// dRdX, dGdX, dBdX, dAdX, fdRdX, fdGdX, fdBdX, and fdAdX Registers
// The dRdX, dGdX, dBdX, dAdX, fdRdX, fdGdX, fdBdX, and fdAdX registers specify the change in the
// color information (red, green, blue, and alpha) with respect to X of a triangle to be rendered. As a
// triangle is rendered, the d?dX registers are added to the the internal color component registers when the
// pixel drawn moves from left-to-right, and are subtracted from the internal color component registers when
// the pixel drawn moves from right-to-left. The fd?dX registers are floating point equivalents of the d?dX
// registers. Banshee automatically converts both the d?dX and fd?dX registers into an internal fixed point
// notation used for rendering.

// dZdX and fdZdX Registers
// The dZdX and fdZdX registers specify the change in Z with respect to X of a triangle to be rendered. As
// a triangle is rendered, the dZdX register is added to the the internal Z register when the pixel drawn
// moves from left-to-right, and is subtracted from the internal Z register when the pixel drawn moves from
// right-to-left. The fdZdX registers are floating point equivalents of the dZdX registers. Banshee
// automatically converts both the dZdX and fdZdX registers into an internal fixed point notation used for
// rendering.

// dSdX, dTdX, fdSdX, and fdTdX Registers
// The dXdX, dTdX, fdSdX, and fdTdX registers specify the change in the S/W and T/W texture
// coordinates with respect to X of a triangle to be rendered. As a triangle is rendered, the d?dX registers
// are added to the the internal S and T registers when the pixel drawn moves from left-to-right, and are
// subtracted from the internal S/W and T/W registers when the pixel drawn moves from right-to-left. Note
// that the delta S/W and T/W values used by Banshee for rendering must be divided by W prior to being
// sent to Banshee (i.e. Banshee uses ∆S/W and ∆T/W ). The d?dX registers are floating point equivalents
// of the fd?dX registers. Banshee automatically converts both the d?dX and fd?dX registers into an
// internal fixed point notation used for rendering.

// dWdX and fdWdX Registers
// The dWdX and fdWdX registers specify the change in 1/W with respect to X of a triangle to be rendered.
// As a triangle is rendered, the dWdX register is added to the the internal 1/W register when the pixel
// drawn moves from left-to-right, and is subtracted from the internal 1/W register when the pixel drawn
// moves from right-to-left. The fdWdX registers are floating point equivalents of the dWdX registers.
// Banshee automatically converts both the dWdX and fdWdX registers into an internal fixed point notation
// used for rendering.

// dRdY, dGdY, dBdY, dAdY, fdRdY, fdGdY, fdBdY, and fdAdY Registers
// The dRdY, dGdY, dBdY, dAdY, fdRdY, fdGdY, fdBdY, and fdAdY registers specify the change in
// the color information (red, green, blue, and alpha) with respect to Y of a triangle to be rendered. As a
// triangle is rendered, the d?dY registers are added to the the internal color component registers when the
// pixel drawn in a positive Y direction, and are subtracted from the internal color component registers when
// the pixel drawn moves in a negative Y direction. The fd?dY registers are floating point equivalents of the
// d?dY registers. Banshee automatically converts both the d?dY and fd?dY registers into an internal fixed
// point notation used for rendering.

// dZdY and fdZdY Registers
// The dZdY and fdZdY registers specify the change in Z with respect to Y of a triangle to be rendered. As
// a triangle is rendered, the dZdY register is added to the the internal Z register when the pixel drawn
// moves in a positive Y direction, and is subtracted from the internal Z register when the pixel drawn moves
// in a negative Y direction. The fdZdY registers are floating point equivalents of the dZdY registers.
// Banshee automatically converts both the dZdY and fdZdY registers into an internal fixed point notation
// used for rendering.

// dSdY, dTdY, fdSdY, and fdTdY Registers
// The dYdY, dTdY, fdSdY, and fdTdY registers specify the change in the S/W and T/W texture
// coordinates with respect to Y of a triangle to be rendered. As a triangle is rendered, the d?dY registers
// are added to the the internal S/W and T/W registers when the pixel drawn moves in a positive Y direction,
// and are subtracted from the internal S/W and T/W registers when the pixel drawn moves in a negative Y
// direction. Note that the delta S/W and T/W values used by Banshee for rendering must be divided by W
// prior to being sent to Banshee (i.e. Banshee uses ∆S/W and ∆T/W ). The d?dY registers are floating
// point equivalents of the fd?dY registers. Banshee automatically converts both the d?dY and fd?dY
// registers into an internal fixed point notation used for rendering.

// dWdY and fdWdY Registers
// The dWdY and fdWdY registers specify the change in 1/W with respect to Y of a triangle to be rendered.
// As a triangle is rendered, the dWdY register is added to the the internal 1/W register when the pixel
// drawn moves in a positive Y direction, and is subtracted from the internal 1/W register when the pixel
// drawn moves in a negative Y direction. The fdWdY registers are floating point equivalents of the dWdY
// registers. Banshee automatically converts both the dWdY and fdWdY registers into an internal fixed
// point notation used for rendering.

typedef Fixed12p12_t Delta12p12_t;
namespace Delta12p12
{
    using namespace Fixed12p12;

    namespace Shift
    {
        using namespace Fixed12p12::Shift;
    }
}

typedef Fixed20p12_t Delta20p12_t;
namespace Delta20p12
{
    using namespace Fixed20p12;

    namespace Shift
    {
        using namespace Fixed20p12::Shift;
    }
}

typedef Fixed14p18_t Delta14p18_t;
namespace Delta14p18
{
    using namespace Fixed14p18;

    namespace Shift
    {
        using namespace Fixed14p18::Shift;
    }
}

typedef Fixed2p30_t Delta2p30_t;
namespace Delta2p30
{
    using namespace Fixed2p30;

    namespace Shift
    {
        using namespace Fixed2p30::Shift;
    }
}

}