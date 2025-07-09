//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TextureMode = 0x300
    };
}

// textureMode Register
// The textureMode register controls texture mapping functionality including perspective correction, texture
// filtering, texture clamping, and multiple texture blending.

// Bit     Name                Description
// 0       tpersp_st           Enable perspective correction for S and T iterators (0=linear interploation of S,T, force
//                             W to 1.0, 1=perspective correct, S/W, T/W)
// 1       tminfilter          Texture minification filter (0=point-sampled, 1=bilinear)
// 2       tmagfilter          Texture magnification filter (0=point-sampled, 1=bilinear)
// 3       tclampw             Clamp when W is negative (0=disabled, 1=force S=0, T=0 when W is negative)
// 4       tloddither          Enable Level-of-Detail dithering (0=no dither, 1=dither)
// 5       tnccselect          Narrow Channel Compressed (NCC) Table Select (0=table 0, 1=table 1)
// 6       tclamps             Clamp S Iterator (0=wrap, 1=clamp)
// 7       tclampt             Clamp T Iterator (0=wrap, 1=clamp)
// 11:8    tformat             Texture format (see table below)
//                             Texture Color Combine Unit control (RGB):
// 12      tc_zero_other       Zero Other (0=c_other, 1=zero)
// 13      tc_sub_clocal       Subtract Color Local (0=zero, 1=c_local)
// 16:14   tc_mselect          Mux Select (0=zero, 1=c_local, 2=a_other, 3=a_local, 4=LOD, 5=LOD_frac, 6-
//                             7=reserved)
// 17      tc_reverse_blend    Reverse Blend (0=normal blend, 1=reverse blend)
// 18      tc_add_clocal       Add Color Local
// 19      tc_add_alocal       Add Alpha Local
// 20      tc_invert_output    Invert Output
//                             Texture Alpha Combine Unit control (A):
// 21      tca_zero_other      Zero Other (0=c_other, 1=zero)
// 22      tca_sub_clocal      Subtract Color Local (0=zero, 1=c_local)
// 25:23   tca_mselect         Mux Select (0=zero, 1=c_local, 2=a_other, 3=a_local, 4=LOD, 5=LOD_frac, 6-
//                             7=reserved)
// 26      tca_reverse_blend   Reverse Blend (0=normal blend, 1=reverse blend)
// 27      tca_add_clocal      Add Color Local
// 28      tca_add_alocal      Add Alpha Local
// 29      tca_invert_output   Invert Output
// 30      trilinear           Enable trilinear texture mapping (0=point-sampled/bilinear, 1=trilinear)

// tpersp_st bit of textureMode enables perspective correction for S and T iterators. Note that there is no
// performance penalty for performing perspective corrected texture mapping.

// tminfilter, tmagfilter bits of textureMode specify the filtering operation to be performed. When point
// sampled filtering is selected, the texel specified by <s,t> is read from texture memory. When bilinear
// filtering is selected, the four closet texels to a given <s,t> are read from memory and blended together as a
// function of the fractional components of <s,t>. tminfilter is referenced when LOD>=LODmin, otherwise
// tmagfilter is referenced.

// tclampw bit of textureMode is used when projecting textures to avoid projecting behind the source of the
// projection. If this bit is set, S, T are each forced to zero when W is negative. Though usually desireable, it
// is not necessary to set this bit when doing projected textures.

// tloddither bit of textureMode enables Level-of-Detail (LOD) dither. Dithering the LOD calculation is
// useful when performing texture mipmapping to remove the LOD bands which can occur from with
// mipmapping without trilinear filtering. This adds an average of 3/8 (.375) to the LOD value and needs to
// compensated in the amount of lodbias.

// tnccselect bit of textureMode selects the NCC lookup table to be used when decompressing 8-bit NCC
// textures.

// tclamps, tclampt bits of textureMode enable clamping of the S and T texture iterators. When clamping
// is enabled, the S iterator is clamped to [0, texture width) and the T iterator is clamped to [0, texture
// height). When clamping is disabled, S coordinates outside of [0, texture width) are allowed to wrap into
// the [0, texture width) range using bit truncation. Similarly when clamping is disabled, T coordinates
// outside of [0, texture height) are allowed to wrap into the [0, texture height) range using bit truncation.

// tformat field of textureMode specifies the texture format accessed by TREX. Note that the texture format
// field is used for both reading and writing of texture memory. The following table shows the texture
// formats and how the texture data is expanded into 32-bit ARGB color:

// tformat     Texture                         8-bit Alpha             8-bit Red               8-bit Green             8-bit Blue
//  Value      format
// 0           8-bit RGB (3-3-2)               0xff                    {r[2:0],r[2:0],r[2:1]}  {g[2:0],g[2:0],g[2:1]}  {b[1:0],b[1:0],b[1:0],b[1:0]}
// 1           8-bit YIQ (4-2-2)               See below
// 2           8-bit Alpha                     a[7:0]                  a[7:0]                  a[7:0]                  a[7:0]
// 3           8-bit Intensity                 0xff                    i[7:0]                  i[7:0]                  i[7:0]
// 4           8-bit Alpha, Intensity (4-4)    {a[3:0],a[3:0]}         {i[3:0],i[3:0]}         {i[3:0],i[3:0]}         {i[3:0],i[3:0]}
// 5           8-bit Palette to RGB            0xff                    palette r[7:0]          palette g[7:0]          palette b[7:0]
// 6           8-bit Palette to RGBA           {palette_r[7:2],        {palette_r[1:0],        {palette_g[3:0],        {palette_b[5:0],
//                                              palette_r[7:6]          palette_g[7:4],         palette_b[7:6],        palette_b[5:4]}
//                                                                      palette_r[1:0]}         palette_g[3:2]}
// 7           Reserved
// 8           16-bit ARGB (8-3-3-2)           a[7:0]                  {r[2:0],r[2:0],r[2:1]}  {g[2:0],g[2:0],g[2:1]}  {b[1:0],b[1:0],b[1:0],b[1:0]}
// 9           16-bit AYIQ (8-4-2-2)           See below
// 10          16-bit RGB (5-6-5)              0xff                    {r[4:0],r[4:2]}         {g[5:0],r[5:4]}         {b[4:0],b[4:2]}
// 11          16-bit ARGB (1-5-5-5)           {a[0],a[0],a[0],a[0],   {r[4:0],r[4:2]}         {g[4:0],g[4:2]}         {b[4:0],b[4:2]}
//                                             a[0],a[0],a[0],a[0]}
// 12          16-bit ARGB (4-4-4-4)           {a[3:0},a[3:0]}         {r[3:0},r[3:0]}         {g[3:0},g[3:0]}         {b[3:0},b[3:0]}
// 13          16-bit Alpha, Intensity (8-8)   a[7:0]                  i[7:0]                  i[7:0]                  i[7:0]
// 14          16-bit Alpha, Palette (8-8)     a[7:0]                  palette r[7:0]          palette g[7:0]          palette b[7:0]
// 15          Reserved

// where a, r, g, b, and i(intensity) represent the actual values read from texture memory. The following
// table shows how 32-bit RGBA texture information is derived from the YIQ texture formats. This is
// detailed later in the nccTable description.

// Texture format          8-bit Alpha         8-bit Red       8-bit Green         8-bit Blue
// 8-bit YIQ (4-2-2)       0xff                ncc _red[7:0]   ncc _green[7:0]     ncc _blue[7:0]
// 16-bit AYIQ (8-4-2-2)   a[7:0]              ncc _red[7:0]   ncc _green[7:0]     ncc _blue[7:0]

// There are three Texture Color Combine Units (RGB) and one Texture Alpha Combine Unit(A), all four
// are identical, except for the bit fields that control them. The tc_* fields of textureMode control the
// Texture Color Combine Units; the tca_* fields control the Texture Alpha Combine Units. The diagram
// below illustrates the Texture Color Combine Unit/Texture Alpha Combine Unit:

//                                                                                                |  tc/tca_reverse_blend
//                                                                                                   |
//                                                                                                |  |  For trilinear:
//                                                                                                   |  0: odd TREX
//                                                                                                |  |  1: even TREX
//                                          Blend with Incoming Color                                |
//                                                                                                |  |     trilinear_enable
//        tc/tca_ c_other    0                                                                       |     |
//                     |     |                          c_local                                   |  |     | LODB[0]
//                   --+-----+--                          |                                          _     | |
//                    * 0   1 *--- tc/tca_ zero_other     / 8                                     |  V    -+-+-
//                     ---+---                            |  a_other                                 o    |   |
//                        |      0     +--------*---------*  |                                    |  |     | |
//                        |      |     |        |         |  |  a_local                              +--+   *
//  tc/tca_sub_c_local    |    --+-----+--      |         |  |  |                                 |   _ |   | _
//             -----------+-----* 0   1 *       |      0  |  |  |  detail_factor                       --___--
//                        |      ---+---        |      |  |  |  |  |                              |   _       _
//                        |         |           |      |  |  |  |  |  LODB_frac[7:0]                  |--___--|
//                0.8.0 8 /         / 8 0.8.0   |      |  |  |  |  |  |                           |   |       |
//                        |         |           |      |  |  |  |  |  |                                |     |
//                        |   +-----+----+      |   ---+--+--+--+--+--+---                        |     |   |
//                        |   | 2's Comp |      |    *                  *---tc/tca_ mselect[2:0]        -___-
//                        |   +-----+----+      |     --------+---------                          |       |
//                        |         |           |             |                                           / 1
//                        |         / 9 1.8.0   |             / 8                                 |       |
//                        |         |           |             |   +------o<|------------------------------+
//                      --+---   ---+--         |           _ |   | _       alpha_inv             |
//                       *     V     *          |            --___--
//                        -----+----- [0,0x100] |           _       _                             |
//                             |                |           |--___--|
//                             / 9 1.8.0        |           |       |                             |
//                             |      +---------+---+        |     |
//                             |      |         |   |         |   |                               |
//                          +--+------+--+      |   |         -___-
//                          | 9 signed x |      |   |           |                                 |
//                          | 9 unsigned |      |   |           / 8
//                          |  multiply  |      |   |           |                                 |
//                          +------+-----+      |   |        +--+--+
//                                 |            |   |        | + 1 |                              |
//                                 |   a_local  |   |        +--+--+
//                                 |         |  |   |           |                                 |
//                                 |      0  |  |   |           / 9 (0.9.0)
//                                 |      |  |  |   +-----------+                                 |
//                                 |   ---+--+--+---
//                                 |    *00 01 10 *--- {tc/tca_ add_c_local, tc/tca_ add_a_local} |
//                                 |     ----+----
//                                 |         |                                                    |
//                                 |         / 8
//                                 |         |                                                    |
//                               --+---   ---+--
//                                *     V     *                                                   |
//                                 -----+-----
//                                      |                                                         |
//                                      / 10 1.9.0
//                                      |                                                         |
//                                +-----+------+
//                                | Clamp 0-FF |                                                  |
//                                +-----+------+
//                                      |                                                         |
//                                      / 8
//       tc/tca_invert_output ------+   |                                                         |
//                                _ |   | _
//                                 --___--                                                        |
//                                _       _
//                                |--___--|                                                       |
//                                |       |
//                                 |     |                                                        |
//                                  |   |
//                                  -___-                                                         |
//                                    |
//                                    / 8 Color                                Unique for a,r,g,b |   Combined in
//                                    |                                                               common unit
//                                    V 0.8                                                       |
//
//                                               tc_ prefix applies to R,G and B channels. tca_ prefix applies to A channel.
//

typedef uint32_t TextureMode_t;
namespace TextureMode
{
    enum
    {
        PerspectiveCorrection =             0x00000001, // 0 tpersp_st Enable perspective correction for S and T iterators (0=linear interploation of S,T, force
                                                        //   W to 1.0, 1=perspective correct, S/W, T/W)
        PerspectiveLinear =                 0x00000000,
        PerspectiveCorrect =                0x00000001,

        TextureMinification =               0x00000002, // 1 tminfilter Texture minification filter (0=point-sampled, 1=bilinear)
        TextureMinificationPoint =          0x00000000,
        TextureMinificationBilinear =       0x00000002,

        TextureMagnification =              0x00000004, // 2 tmagfilter Texture magnification filter (0=point-sampled, 1=bilinear)
        TextureMagnificationPoint =         0x00000000,
        TextureMagnificationBilinear =      0x00000004,

        ClampWNegative =                    0x00000008, // 3 tclampw Clamp when W is negative (0=disabled, 1=force S=0, T=0 when W is negative)

        LodDithering =                      0x00000010, // 4 tloddither Enable Level-of-Detail dithering (0=no dither, 1=dither)

        NCCTableSelect =                    0x00000020, // 5 tnccselect Narrow Channel Compressed (NCC) Table Select (0=table 0, 1=table 1)
        NCCTable0 =                         0x00000000,
        NCCTable1 =                         0x00000020,

        SIterator =                         0x00000040, // 6 tclamps Clamp S Iterator (0=wrap, 1=clamp)
        SIteratorWrap =                     0x00000000,
        SIteratorClamp =                    0x00000040,
        
        TIterator =                         0x00000080, // 7 tclampt Clamp T Iterator (0=wrap, 1=clamp)
        TIteratorWrap =                     0x00000000,
        TIteratorClamp =                    0x00000080,

        Format =                            0x00000f00, // 11:8 tformat Texture format (see table below)
        RGB8bpp =                           0x00000000, // 8-bit RGB (3-3-2)
        YIQ8bpp =                           0x00000100, // 8-bit YIQ (4-2-2)
        Alpha8bpp =                         0x00000200, // 8-bit Alpha
        Intensity8bpp =                     0x00000300, // 8-bit Intensity
        AlphaIntensity8bpp =                0x00000400, // 8-bit Alpha, Intensity (4-4)
        PaletteRGB8bpp =                    0x00000500, // 8-bit Palette to RGB
        PaletteRGBA8bpp =                   0x00000600, // 8-bit Palette to RGBA
        ARGB16bpp =                         0x00000800, // 16-bit ARGB (8-3-3-2)
        AYIQ16bpp =                         0x00000900, // 16-bit AYIQ (8-4-2-2)
        RGB16bpp =                          0x00000a00, // 16-bit RGB (5-6-5)
        ARGB16bpp1555 =                     0x00000b00, // 16-bit ARGB (1-5-5-5)
        ARGB16bpp4444 =                     0x00000c00, // 16-bit ARGB (4-4-4-4)
        AlphaIntensity16bpp =               0x00000d00, // 16-bit Alpha, Intensity (8-8)
        AlphaPalette16bpp =                 0x00000e00, // 16-bit Alpha, Palette (8-8)

        TCZeroOrOther =                     0x00001000, // 12 tc_zero_other Zero Other (0=c_other, 1=zero)
        TCOther =                           0x00000000,
        TCZero =                            0x00001000,

        TCSubColorLocal =                   0x00002000, // 13 tc_sub_clocal Subtract Color Local (0=zero, 1=c_local)
        TCSCLZero =                         0x00000000,
        TCSCLColorLocal =                   0x00002000,

        TCMuxSelect =                       0x0001c000, // 16:14 tc_mselect Mux Select (0=zero, 1=c_local, 2=a_other, 3=a_local, 4=LOD, 5=LOD_frac, 6-
                                                        //       7=reserved)
        TCMSZero =                          0x00000000,
        TCMSColorLocal =                    0x00004000,
        TCMSAlphaOther =                    0x00008000,
        TCMSAlphaLocal =                    0x0000c000,
        TCMSLod =                           0x00010000,
        TCMSLodFraction =                   0x00014000,

        TCBlend =                           0x00020000, // 17 tc_reverse_blend Reverse Blend (0=normal blend, 1=reverse blend)
        TCBNormal =                         0x00000000,
        TCBReverse =                        0x00020000,

        TCAddColorLocal =                   0x00040000, // 18 tc_add_clocal Add Color Local

        TCAddAlphaLocal =                   0x00080000, // 19 tc_add_alocal Add Alpha Local

        TCInvert =                          0x00100000, // 20 tc_invert_output Invert Output

        TCAZeroOrOther =                    0x00200000, // 21 tca_zero_other Zero Other (0=c_other, 1=zero)
        TCAOther =                          0x00000000,
        TCAZero =                           0x00200000,

        TCASubColorLocal =                  0x00400000, // 22 tca_sub_clocal Subtract Color Local (0=zero, 1=c_local)
        TCASCLZero =                        0x00000000,
        TCASCLColorLocal =                  0x00400000,

        TCAMuxSelect =                      0x03800000, // 25:23 tca_mselect Mux Select (0=zero, 1=c_local, 2=a_other, 3=a_local, 4=LOD, 5=LOD_frac, 6-
                                                        //       7=reserved)
        TCAMSZero =                         0x00000000,
        TCAMSColorLocal =                   0x00800000,
        TCAMSAlphaOther =                   0x01000000,
        TCAMSAlphaLocal =                   0x01800000,
        TCAMSLod =                          0x02000000,
        TCAMSLodFraction =                  0x02800000,

        TCABlend =                          0x04000000, // 26 tca_reverse_blend Reverse Blend (0=normal blend, 1=reverse blend)
        TCABNormal =                        0x00000000,
        TCABReverse =                       0x04000000,

        TCAAddColorLocal =                  0x08000000, // 27 tca_add_clocal Add Color Local

        TCAAddAlphaLocal =                  0x10000000, // 28 tca_add_alocal Add Alpha Local

        TCAInvert =                         0x20000000, // 29 tca_invert_output Invert Output

        Trilinear =                         0x40000000, // 30 trilinear Enable trilinear texture mapping (0=point-sampled/bilinear, 1=trilinear)
        TrilinearDisable =                  0x00000000,
        TrilinearEnable =                   0x40000000
    };

    namespace Shift
    {
        enum
        {
            PerspectiveCorrection = 0x00,
            TextureMinification = 0x01,
            TextureMagnification = 0x02,
            ClampWNegative = 0x03,
            LodDithering = 0x04,
            NCCTableSelect = 0x05,
            SIterator = 0x06,
            TIterator = 0x07,
            Format = 0x08,
            TCZeroOrOther = 0x0c,
            TCSubColorLocal = 0x0d,
            TCMuxSelect = 0x0e,
            TCBlend = 0x11,
            TCAddColorLocal = 0x12,
            TCAddAlphaLocal = 0x13,
            TCInvert = 0x14,
            TCAZeroOrOther = 0x15,
            TCASubColorLocal = 0x16,
            TCAMuxSelect = 0x17,
            TCABlend = 0x1a,
            TCAAddColorLocal = 0x1b,
            TCAAddAlphaLocal = 0x1c,
            TCAInvert = 0x1d,
            Trilinear = 0x1e
        };
    }
}

}