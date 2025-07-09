//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbzColorPath = 0x104
    };
}

// fbzColorPath Register
// The fbzColorPath register controls the color and alpha rendering pixel pipelines. Bits in fbzColorPath
// control color/alpha selection and lighting. Individual bits of fbzColorPath are set to enable modulation,
// addition, etc. for various lighting effects including diffuse and specular highlights.

// Note that the color channels are controlled separately from the alpha channel. There are two primary
// color selection units: the Color Combine Unit(CCU) and the Alpha Combine Unit (ACU). Bits(1:0),
// bit(4), and bits(16:8) of fbzColorPath control the Color Combine Unit. The diagram below illustrates the
// Color Combine Unit controlled by the fbzColorPath register:

//                                      - Color Combine Unit -
//
//                              color1 RGB
//                          texture RGB  |  Linear frame         color0 RGB   iterated RGB
//                      iterated RGB  |  |  | buffer RGB                  |   |
//chromaKey, chromaRange           |  |  |  |                         ----+---+----      *|-- (0,1) texture alpha bit (0)
//          |                    --+--+--+--+--                        *  1   0  *------| |
//  +-------+-------+             *0  1  2   *--- rgbselect [1:0]       ----+----        *|-- cc_localselect
//  |    Optional   |              ----+-----                               |            |
//  | Chroma-Key or |                  |                                    |            cc_localselect_override
//  | Chroma-Range  +------------------+                                    |
//  |     Check     |                  |                                 c_local
//  +-------+-------+               c_other  0                              |
//          |                          |     |                              |  a_other
//          V                       ---+-----+---                           |  | 
//  Invalidate Pixel                 * 0     1 *---- cc_zero_other        8 /  |  a_local
//                                    ----+----                             |  |  |
//                                        |    0   +---------*--------------*  |  |  texture alpha
//                                        |    |   |         |           0  |  |  |  |
//                                        |  --+---+--       |           |  |  |  |  |  texture RGB
//                       cc_sub_local ----+---*0   1*        |           |  |  |  |  |  |
//                                        |    --+--         |        ---+--+--+--+--+--+---
//                                        |      |           |         * 0  1  2  3  4  5 *----- cc_mselect[2:0]
//                                        |      / 8         |          ----+-------------
//                                        |      |           |              |
//                                        | +----+-----+     |              / 8 +------o<|--- cc_reverse_blend
//                                        | | 2's Comp |     |            _ |   | _
//                                        | +----+-----+     |             --___--
//                                  0.8.0 / 8    |           |            _       _
//                                        |      / 9 1.8.0   |            |--___--|
//                                        |      |           |            |       |
//                                     ---+--   -+---        |             |     |
//                                      *     V    *         |              |   |
//                                       -----+----          |              -___-
//                                            |              |                |
//                                            / 9 1.8.0 +----+-------+        / 8
//                                            |         |    |       |        |
//                                          +-------------+  |       |    +---+--+
//                                          | 9 signed X  |  |       |    |  +1  |
//                                          | 9  unsigned |  |       |    +---+--+
//                                          |  multiply   |  |       |        |
//                                          +------+------+  |       |        / 9 0.9.0
//                                                 |         |       |        |
//                                                 / 9 1.8.0 |       +--------+
//                                                 |         |
//                                                 | a_local |
//                                                 |   0  |  |
//                                                 |   |  |  |
//                                                 | --+--+--+---
//                                                 |  *00 01 10*---- {cc_add_clocal, cc_add_alocal}
//                                                 |   ----+---
//                                                 |       |
//                                                 |       / 8
//                                                 |       |
//                                              ------   ------
//                                               *     V     *
//                                                -----+-----
//                                                     |
//                                                     / 10 1.9.0
//                                                     |
//                                              +------+-----+
//                                              | Clamp 0-FF |
//                                              +------+-----+
//                                                     |
//                                                     / 8
//                              cc_invert_output       |
//                                           ------+   |
//                                               _ |   | _
//                                                --___--
//                                               _       _
//                                               |--___--|
//                                               |       |
//                                                |     |
//                                                 |   |
//                                                 -___-
//                                                   |
//                                                   / 8 Color
//                                                   |
//                                                   V 0.8
//
//
// Bits(3:2), bits(6:5), and bits(25:17) of fbzColorPath control the Alpha Combine Unit. The diagram
// below illustrates the Alpha Combine Unit controlled by the fbzColorPath register:
//
//
//                            color1 alpha                        color0 alpha  iterated Z(27:20), clamped
//                        texture alpha  |  Linear frame     iterated alpha  |  |  iterated W(39:32), clamped
//                    iterated alpha  |  |  | buffer alpha                |  |  |  |
//    Alpha-Mask Enable            |  |  |  |                         ----+--+--+--+----
//          |                    --+--+--+--+--                        *  0  1  2  3  *--- cca_localselect[1:0]
//  +-------+-------+             *0  1  2   *--- aselect [1:0]         ----+---------
//  |               |              ----+-----                               |
//  |   Alpha-Mask  |                  |                                    |
//  |      Check    +------------------+                                    |
//  |               |                  |                                 a_local
//  +-------+-------+               a_other  0                              |
//          |                          |     |                              |  a_other
//          V                       ---+-----+---                           |  | 
//                                   * 0     1 *---- cca_zero_other         / 8|  a_local
//                                    ----+----                             |  |  |
//                                        |    0   +---------*--------------*  |  |  texture alpha
//                                        |    |   |         |           0  |  |  |  |
//                                        |  --+---+--       |           |  |  |  |  |
//                      cca_sub_local ----+---*0   1*        |           |  |  |  |  |
//                                        |    --+--         |        ---+--+--+--+--+---
//                                        |      |           |         * 0  1  2  3  4 *----- cca_mselect[2:0]
//                                        |      / 8         |          ----+----------
//                                        |      |           |              |
//                                        | +----+-----+     |              / 8 +------o<|--- cc_reverse_blend
//                                        | | 2's Comp |     |            _ |   | _
//                                        | +----+-----+     |             --___--
//                                  0.8.0 / 8    |           |            _       _
//                                        |      / 9 1.8.0   |            |--___--|
//                                        |      |           |            |       |
//                                     ---+--   -+---        |             |     |
//                                      *     V    *         |              |   |
//                                       -----+----          |              -___-
//                                            |              |                |
//                                            / 9 1.8.0 +----+-------+        / 8
//                                            |         |    |       |        |
//                                          +-------------+  |       |    +---+--+
//                                          | 9 signed X  |  |       |    |  +1  |
//                                          | 9  unsigned |  |       |    +---+--+
//                                          |  multiply   |  |       |        |
//                                          +------+------+  |       |        / 9 0.9.0
//                                                 |         |       |        |
//                                                 / 9 1.8.0 |       +--------+
//                                                 |         |
//                                                 | a_local |
//                                                 |   0  |  |
//                                                 |   |  |  |
//                                                 | --+--+--+---
//                                                 |  *00 01 10*---- {cc_add_clocal, cc_add_alocal}
//                                                 |   ----+---
//                                                 |       |
//                                                 |       / 8
//                                                 |       |
//                                              ------   ------
//                                               *     V     *
//                                                -----+-----
//                                                     |
//                                                     / 10 1.9.0
//                                                     |
//                                              +------+-----+
//                                              | Clamp 0-FF |
//                                              +------+-----+
//                                                     |
//                                                     / 8
//                             cca_invert_output       |
//                                           ------+   |
//                                               _ |   | _
//                                                --___--
//                                               _       _
//                                               |--___--|
//                                               |       |
//                                                |     |
//                                                 |   |
//                                                 -___-
//                                                   |
//                                                   / 8 Alpha
//                                                   |
//                                                   V 0.8

// Bit(26) of fbzColorPath enables subpixel correction for all parameters. When enabled, Banshee will
// automatically subpixel correct the incoming color, depth, and texture coordinate parameters for triangles
// not aligned on integer spatial boundaries. Enabling subpixel correction decreases the on-chip triangle
// setup performance from 7 clocks to 16 clocks, but as the triangle setup engine is separately pipelined from
// the triangle rasterization engine, little if any performance penalty is seen when subpixel correction is
// enabled.

// Important Note: When subpixel correction is enabled, the correction is performed on the start registers as
// they are passed into the triangle setup unit from the PCI FIFO. As a result, the host must pass down new
// starting parameter information for each new triangle -- if new starting parameter information is not
// passed down for a new triangle, the starting parameters will be subpixel corrected starting with the start
// registers already subpixel corrected for the last rendered triangle [in effect the parameters will be subpixel
// corrected twice, resulting in inaccuracies in the starting parameter values].

// Bit(27) of fbzColorPath is used to enable texture mapping. If texture-mapped rendering is desired, then
// bit(27) of fbzColorPath must be set. When bit(27)=1, then data is transfered from TREX to FBI. If
// texture mapping is not desired (i.e. Gouraud shading, flat shading, etc.), then bit(27) may be cleared and
// no data is transfered from TREX to FBI.

// Bit(28) of fbzColorpath is used to enable RGBA, Z, and W parameter clamping. When fbzColorpath
// bit(28)=1, then the RGBA triangle parameters are be clamped to [0,0xff] inclusive during triangle
// rasterization. Note that fbzColorpath bit(28) has no effect on the RGBA triangle parameters during
// triangle setup or sub-pixel correction. When fbzColorpath bit(28)=0, then the RGBA parameters are
// allowed to wrap according to the following formula:

//         if(rgbaIterator[23:12] == 0xfff)
//             rgbaClamped[7:0] = 0x0;
//         else if(rgbaIterator[23:12] == 0x100)
//             rgbaClamped[7:0] = 0xff;
//         else
//             rgbaClamped[7:0] = rgbaIterator[19:12];

// When fbzColorpath bit(28)=1, then the Z triangle parameter is clamped to [0,0xffff] inclusive during
// triangle rasterization. Note that fbzColorpath bit(28) has no effect on the Z triangle parameter during
// triangle setup or sub-pixel correction. Note also that the unclamped Z triangle iterator is used when
// performing floating point Z-buffering (fbzMode bit(21)=1). When fbzColorpath bit(28)=0, then the Z
// parameter is allowed to wrap according to the following formula:

//         if(zIterator[31:12] == 0xfffff)
//             zClamped[15:0] = 0x0;
//         else if(zIterator[31:12] == 0x10000)
//             zClamped[15:0] = 0xffff;
//         else
//             zClamped[15:0] = zIterator[27:12];

// When fbzColorpath bit(28)=1, then the W triangle parameter is clamped to [0,0xff] inclusive for use in
// the Alpha Combine Unit and the fog unit. Note that fbzColorpath bit(28) has no effect on the W triangle
// parameter during triangle setup or sub-pixel correction. Note also that the unclamped W triangle iterator
// is used when performing floating point W-buffering (fbzMode bit(21)=0). When fbzColorpath
// bit(28)=0, then the W parameter used as inputs to the ACU and fog units is allowed to wrap according to
// the following formula:

//         if(wIterator[47:32] == 0xffff)
//             wClamped[7:0] = 0x0;
//         else if(zIterator[47:32] == 0x0100)
//             wClamped[7:0] = 0xff;
//         else
//             wClamped[7:0] = wIterator[39:32];

// Bit(29) of fbzColorpath used to enable anti-aliasing. FIXME…


typedef uint32_t FbzColorPath_t;
namespace FbzColorPath
{
    enum
    {
        RGBSelect =                 0x00000003, // 1:0 RGB Select (0=Iterated RGB, 1=TREX Color Output, 2=Color1 RGB, 3=Reserved)
        RGBIterated =               0x00000000,
        RGBTrex =                   0x00000001,
        RGBColor1 =                 0x00000002,

        AlphaSelect =               0x0000000c, // 3:2 Alpha Select (0=Iterated A, 1=TREX Alpha Output, 2=Color1 Alpha, 3=Reserved)
        AlphaIteraged =             0x00000000,
        AlphaTrex =                 0x00000004,
        AlphaColor1 =               0x00000008,
        
        CCULocalSelect =            0x00000010, // 4 Color Combine Unit control (cc_localselect mux control: 0=iterated RGB, 1=Color0 RGB)
        CCULSIterated =             0x00000000,
        CCULSColor0 =               0x00000010,
        
        ACULocalSelect =            0x00000060, // 6:5 Alpha Combine Unit control (cca_localselect mux control: 0=iterated alpha, 1=Color0
                                                //     alpha, 2=iterated Z, 3=clamped iterated W)
        ACULSIterated =             0x00000000,
        ACULSColor0 =               0x00000020,
        ACULSIteratedZ =            0x00000040,
        ACULSClampedIteratedW =     0x00000060,
        
        CCULocalSelectOverride =    0x00000080, // 7 Color Combine Unit control (cc_localselect_override mux control: 0=cc_localselect,
                                                //   1=Texture alpha bit(7))
        CCULSOLocal =               0x00000000,
        CCULSOTextureAlpha =        0x00000080,

        CCUZeroOther =              0x00000100, // 8 Color Combine Unit control (cc_zero_other mux control: 0=c_other, 1=zero)
        CCUZOOther =                0x00000000,
        CCUZOZero =                 0x00000100,

        CCUZeroLocal =              0x00000200, // 9 Color Combine Unit control (cc_sub_clocal mux control: 0=zero, 1=c_local)
        CCUZLZero =                 0x00000000,
        CCUZLLocal =                0x00000200,

        CCUSelect =                 0x00001c00, // 12:10 Color Combine Unit control (cc_mselect mux control: 0=zero, 1=c_local, 2=a_other,
        CCUSZero =                  0x00000000, //       3=a_local, 4=texture alpha, 5=texture rgb, 6-7=reserved)
        CCUSColorLocal =            0x00000400,
        CCUSAlphaOther =            0x00000800,
        CCUSAlphaLocal =            0x00000c00,
        CCUSTextureAlpha =          0x00001000,
        CCUSTextureColor =          0x00001400,

        CCUBlendControl =           0x00002000, // 13 Color Combine Unit control (cc_reverse_blend control)
        CCUBCNormal =               0x00000000,
        CCUBCReverse =              0x00002000,

        CCUAddColorLocal =          0x00004000, // 14 Color Combine Unit control (cc_add_clocal control)

        CCUAddAlphaLocal =          0x00008000, // 15 Color Combine Unit control (cc_add_alocal control)

        CCUInvert =                 0x00010000, // 16 Color Combine Unit control (cc_invert_output control)

        ACUZeroOther =              0x00020000, // 17 Alpha Combine Unit control (cca_zero_other mux control: 0=a_other, 1=zero)
        ACUZOOther =                0x00000000,
        ACUZOZero =                 0x00020000,

        ACUZeroLocal =              0x00040000, // 18 Alpha Combine Unit control (cca_sub_clocal mux control: 0=zero, 1=a_local)
        ACUZLZero =                 0x00000000,
        ACUZLLocal =                0x00040000,

        ACUSelect =                 0x00380000, // 21:19 Alpha Combine Unit control (cca_mselect mux control: 0=zero, 1=a_local, 2=a_other,
        ACUSZero =                  0x00000000, //       3=a_local, 4=texture alpha, 5-7=reserved)
        ACUSAlphaLocal =            0x00080000,
        ACUSAlphaOther =            0x00100000,
        ACUSAlphaLocal_2 =          0x00180000, // ?
        ACUSTextureAlpha =          0x00200000,

        ACUBlendControl =           0x00400000, // 22 Alpha Combine Unit control (cca_reverse_blend control)
        ACUBCNormal =               0x00000000,
        ACUBCReverse =              0x00400000,

        ACUAddColorLocal =          0x00800000, // 23 Alpha Combine Unit control (cca_add_clocal control)

        ACUAddAlphaLocal =          0x01000000, // 24 Alpha Combine Unit control (cca_add_alocal control)

        ACUInvert =                 0x02000000, // 25 Alpha Combine Unit control (cca_invert_output control)

        ParameterAdjust =           0x04000000, // 26 Parameter Adjust (1=adjust parameters for subpixel correction)

        TextureMapping =            0x08000000, // 27 Enable Texture Mapping (1=enable)
        TextureMappingDisable =     0x00000000,
        TextureMappingEnable =      0x08000000,

        Clamping =                  0x10000000, // 28 Enable RGBA, Z, and W parameter clamping (1=enable)
        ClampingDisable =           0x00000000,
        ClampingEnable =            0x10000000,

        AntiAliasing =              0x20000000, // 29 Enable anti-aliasing (1=enable)
        AntiAliasingDisable =       0x00000000, // Documentation has "used to be anti-aliasing, FIXME"
        AntiAliasingEnable =        0x20000000  // So this might not do what it says.
    };

    namespace Shift
    {
        enum
        {
            RGBSelect =                 0x00,
            AlphaSelect =               0x02,
            CCULocalSelect =            0x04,
            ACULocalSelect =            0x05,
            CCULocalSelectOverride =    0x07,
            CCUZeroOther =              0x08,
            CCUZeroLocal =              0x09,
            CCUSelect =                 0x0a,
            CCUBlendControl =           0x0d,
            CCUAddColorLocal =          0x0e,
            CCUAddAlphaLocal =          0x0f,
            CCUInvert =                 0x10,
            ACUZeroOther =              0x11,
            ACUZeroLocal =              0x12,
            ACUSelect =                 0x13,
            ACUBlendControl =           0x16,
            ACUAddColorLocal =          0x17,
            ACUAddAlphaLocal =          0x18,
            ACUInvert =                 0x19,
            ParameterAdjust =           0x1a,
            TextureMapping =            0x1b,
            Clamping =                  0x1c,
            AntiAliasing =              0x1d
        };
    }
}

}