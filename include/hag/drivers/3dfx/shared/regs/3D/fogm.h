//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FogMode = 0x108
    };
}

// fogMode Register
// The fogMode register controls the fog functionality of Banshee.

// The diagram below shows the fog unit of Banshee:

// Color Channel
// (from Color                                  iterated W
//  Combine Unit)                           (4.12 floating point)
//      |                                           |
//      |               fogcolor                    +------------------------+-----------------------+
// +----+  0    fogmult     |    0                  |                        |                       |
// |    |  |         |      |    |                  / 6 {4 bits exponent,    / 6 {4 bits exponens,   / 8 mantissa(9:2)
// |  --+--+--  _-|--+    --+----+--                |    mantissa (11:10)}   |    mantissa (11:0)}   |
// |   *0  1*--*  |        *0    1*---fogadd  +-----+-------+   +------------+------+                |
// |    -+--    -_|--+      --+---            |  64x8 RAM   |   |     64x8 RAM      |                |
// |     |           |        |               | (fog alpha) |   | (fog delta alpha) |                |
// |     |         fogenable  |               +-----+-------+   +-------+-----------+                |
// |     +----------+         |                     |                   |                            |
// |     |          |         |                     / 8                 / 8 (6.2 format)             |
// |     |          / 8       |                     |   +---------------+                +-----------+
// |     |          |         |                     |   |               |                |
// |     |   +------+---+     |                     |   / 1 (0.1        / 6 (6.0 format) / 8 (8.0 format)
// |     |   | 2's Comp |     |                     |   |    format,    |                |
// |     |   +------+---+     |                     |   |    2nd LSB) +-+----------------+-+
// |     |          |         |                     |   |             |    8 unsigned x    |
// / 8   / 8        / 9       |                     |   |             |     6 unsigned     |        +------------------------+
// |     |          |         |                     |   |             |      multiply      |        |     Dither Matrix      |
// |     |         -+---   ---+-                    |   |             +---------+----------+        | bit(3) = y[0] xor x[0] |
// |     |          *    V    *                     |   |                       |                   | bit(2) = y[0]          |
// |     |           ----+----                      |   |                       / 10 (6.4 format)   | bit(1) = y[1] xor x[1] |
// |     |               |                          |   |              +--------+                   | bit(0) = y[1]          |
// |     |               / 9                        |   |              |        |                   +-------+----------------+
// |     |               |                          |   |              |   +----+-----+                     |
// |     |               |                          |   |              |   | 2's Comp |                     / 4 (0.4 format)
// |     |               |                          |   |              |   +----+-----+                     |
// |     |               |                          |   |              |        |                           |
// |     |               |                          |   |              |        / 11                        |
// |     |               |                          |   |              |        |                           |
// |     |               |                          |   +--------|-_  -+--------+-                          |
// |     |               |                          |            |  *--* 0    1 *                           |
// |     |               |                          |        +---|_-    ----+---                            |
// |     |               |                          |        |              |                               |
// |     |               |                          |   fogzones            / 11                            |
// |     |               |                          |                       |                               |
// |     |               |                          |                +------+-----------+                   |
// |     |               |                          |                |                  |                   |
// |     |               |                          +--------+       / 7 (7.0 format)   / 4 (0.4 format)    |
// |     |               |                                   |       |                  |                   |
// |     |               |                                   |       |                  |       +-----------+
// |     |               |                                   |       |                  |       |
// |     |               |                                   |       |  carry-in       -+--   --+-
// |     |               |                                   |       |   |              *       *
// |     |               |                                   |       |   |               ---+---
// |     |               |                                   |       |   |                  |
// |     |               |                                   |       |   |                  / 1 (carry-out)
// |     |               |                                   |       |   |                  |
// |     |               |                                  -+--   --+-  V  _-|-------------+
// |     |               |                                   *   V   *-----*  |
// |     |               |                                    ---+---       -_|- fogdither
// |     |               |                                       |
// |     |               |                                fog table alpha
// |     |               |                                       |
// |     |               |                                       |  iterated alpha
// |     |               |                                       |  |
// |     |               |                                       |  |  iterated Z(27:30), clamped
// |     |               |                                       |  |  |
// |     |               |                                       |  |  |  iterated W(39:32), clamped
// |     |               |                                       |  |  |  |
// |     |               |                                    ---+--+--+--+---
// |     |               |                                     * 0  1  2  3 *-- {fogz, fogalpha}
// |     |               |                                      ------+-----
// |     |               |                                            |
// |     |               |      +------------------------+            / 8     1
// |     |               |      |                        |            |       |
// |     |            +--+------+--+                     |           -+--   --+-
// |     |            | 9 signed x |                     |            *   V   *
// |     |            | 9 unsigned |                     |             ---+---
// |     |            |  multiply  |                     / 9 (1.8         |
// |     |            +-----+------+                     |    format      |
// |     |       fogcolor   |                            +----------------+
// |     |              |   |
// |     |            --+---+--
// |     |             *1   0*--- fogconstant
// |     |              --+--
// |     |                |
// |     |                | +---- fogenable
// |     |                | |
// |     |                ---
// |     |                | |
// |     +---------+       *
// |               |       |
// |               |       / 8
// |               |       |
// |              -+--   --+-
// |               *   V   *
// +--------+       ---+---
//          |          |
//          / 8 Color  / 8 Fogged Color
//          | before   |
//          | fog      |
//          V          V
//

// Bit(0) of fogMode is used to enable fog and atmospheric effects. When fog is enabled, the fog color
// specified in the fogColor register is blended with the source pixels as a function of the fogTable values
// and iterated W. Banshee supports a 64-entry lookup table (fogTable) to support atmospheric effects such
// as fog and haze. When enabled, the MSBs of a normalized floating point representation of (1/W) is used
// to index into the 64-entry fog table. The ouput of the lookup table is an “alpha” value which represents
// the level of blending to be performed between the static fog/haze color and the incoming pixel color. 8
// lower order bits of the floating point (1/W) are used to blend between multiple entries of the lookup table
// to reduce fog “banding.” The fog lookup table is loaded by the Host CPU, so various fog equations,
// colors, and effects can be supported.
// The following table shows the mathematical equations for the supported values of bits(2:1) of fogMode
// when bits(5:3)=0:

// Bit(0) - Enable Fog     Bit(1) - fogadd mux control     Bit(2) - fogmult mux control        Fog Equation
//  0                       ignored                         ignored                            Cout = Cin
//  1                       0                               0                                  Cout = Afog*Cfog + (1-Afog)*Cin
//  1                       0                               1                                  Cout = Afog*Cfog
//  1                       1                               0                                  Cout = (1-Afog)*Cin
//  1                       1                               1                                  Cout = 0

// where:

//     Cout = Color output from Fog block
//     Cin = Color input from Color Combine Unit Module
//     Cfog = fogColor register
//     AFog = alpha value calculated from Fog table


// When bit(3) of fogMode is set, the integer part of the iterated alpha component is used as the fog alpha
// instead of the calculated fog alpha value from the fog table. When bit(4) of fogMode is set, the upper 8
// bits of the iterated Z component are used as the fog alpha instead of the calculated fog alpha value from
// the fog table. If both bit(3) and bit(4) are set, then bit(4) takes precedence, and the upper 8 bits of the
// iterated Z component are used for the fog alpha value. Bit(5) of fogMode takes precedence over bits(4:3)
// and enables a constant value(fogColor) to be added to incoming source color.

typedef uint32_t FogMode_t;
namespace FogMode
{
    enum
    {
        Enable =            0x00000001, // Enable fog (1=enable)

        FogAdd =            0x00000002, // Fog Unit control (fogadd control: 0=fogColor, 1=zero)
        FAColor =           0x00000000,
        FAZero =            0x00000002,

        FogMult =           0x00000004, // Fog Unit control (fogmult control: 0=Color Combine Unit RGB, 1=zero)
        FMCCURGB =          0x00000000,
        FMZero =            0x00000004,

        FogAlpha =          0x00000008, // Fog Unit control (fogalpha control)

        FogZ =              0x00000010, // Fog Unit control (fogz control)

        FogConstant =       0x00000020, // Fog Unit control (fogconstant control: 0=fog multiplier output, 1=fogColor)
        FCMultiplier =      0x00000000,
        FCColor =           0x00000020,

        FogDither =         0x00000040, // Fog Unit control (fogdither control, dither the fog blending component)

        FogZones =          0x00000080, // Fog Unit control (fogzones control, enable signed fog delta)
        FZNormal =          0x00000000,
        FZSignedDelta =     0x00000080
    };

    namespace Shift
    {
        enum
        {
            Enable = 0x00,
            FogAdd = 0x01,
            FogMult = 0x02,
            FogAlpha = 0x03,
            FogZ = 0x04,
            FogConstant = 0x05,
            FogDither = 0x06,
            FogZones = 0x07
        };
    }
}

}