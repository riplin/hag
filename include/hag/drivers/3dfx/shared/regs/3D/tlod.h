//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TextureLevelOfDetail = 0x304
    };
}

// tLOD Register
// The tLOD register controls the texture mapping LOD calculations.

// Bit     Name            Description
// 5:0     lodmin              Minimum LOD. (4.2 unsigned)
// 11:6    lodmax              Maximum LOD. (4.2 unsigned)
// 17:12   lodbias             LOD Bias. (4.2 signed)
// 18      lod_odd             LOD odd (0=even, 1=odd)
// 19      lod_tsplit          Texture is Split. (0=texture contains all LOD levels, 1=odd or even levels only, as
//                             controlled by lod_odd)
// 20      lod_s_is_wider      S dimension is wider, for rectilinear texture maps. This is a don’t care for square
//                             textures. (1=S is wider than T).
// 22:21   lod_aspect          Aspect ratio. Equal to 2^n. (00 is square texture, others are rectilinear: 01 is
//                             2x1/1x2, 10 is 4x1/1x4, 10 is 8x1/1x8)
// 23      lod_zerofrac        LOD zero frac, useful for bilinear when even and odd levels are split across two
//                             TREXs (0=normal LOD frac, 1=force fraction to 0)
// 24      tmultibaseaddr      Use multiple texbaseAddr registers
// 25      tdata_swizzle       Byte swap incoming texture data (bytes 0<->3, 1<->2).
// 26      tdata_swap          Short swap incoming texture data (shorts 0<->1).
// 27      reserved            Used to be tdirect_write in Voodoo graphics.
// 28      tmirrors            Mirror texture in S dimension
// 29      tmirrort            Mirror texture in T dimension

// lodbias is added to the calculated LOD value, then it is clamped to the range [lodmin, min(8.0, lodmax)].
// Note that whether the LOD is clamped to lodmin is used to determine whether to use the minification or
// magnification filter, selected by the tminfilter and tmagfilter bits of textureMode:

//                  LOD bias, clamp
//
//              LOD min     LOD max
//                |           |
//  <--------|----+-----------+--|----------> LOD
//           0    |           |  8
//        256x256               1x1
//  <-------------o
//    tmagfilter  -------------------->
//                  tminfilter
//

// The tdata_swizzle and tdata_swap bits in tLOD are used to modify incoming texture data for endian
// dependencies. The tdata_swizzle bit causes incoming texture data bytes to be byte order reversed, such
// that bits(31:24) are swapped with bits(7:0), and bits(23:16) are swapped with bits(15:8). Short-word
// swapping is performed after byte order swizzling, and is selected by the tdata_swap bit in tLOD. When
// enabled, short-word swapping causes the post-swizzled 16-bit shorts to be order reversed, such that
// bits(31:16) are swapped with bits(15:0). The following diagram shows the data manipulation functions
// perfomed by the tdata_swizzle and tdata_swap bits:

//
//                          Incoming Texture Data
//                                    |
//                                    / 32
//                                    |
//                                    V
//                +---------+---------+---------+---------+
//                |    3    |    2    |    1    |    0    | (Bytes 0-3)
//                +----+----+----+----+----+----+----+----+
//                     |         |         |         |
//                     / 8       / 8       / 8       / 8
//                     |         |         |         |
//                     |         |  +------*         |
//                     |         |  |      |         |
//                     |         *--+------+--+      |
//                     |         |  |      |  |      |
//                     *---------+--+------+--+------+--+
//                     |         |  |      |  |      |  |
//                     |  +------+--+------+--+------*  |
//                     |  |      |  |      |  |      |  |
//                   --+--+--  --+--+--  --+--+--  --+--+--
//                    *0  1*----*0  1*----*0  1*----*0  1*---- tdata_swizzle
//                     -+--      -+--      -+--      -+--
//                      |         |         |         |
//                      / 8       / 8       / 8       / 8
//                      |         |         |         |
//                      V         V         V         V
//                +-------------------+-------------------+
//                |         1         |         0         | (Shorts 0-1)
//                +---------+---------+---------+---------+
//                          |                   |
//                          / 16                / 16
//                          |                   |
//                          *----------------+  |
//                          |                |  |
//                          |  +-------------+--*
//                          |  |             |  |
//                        --+--+--         --+--+--
//                         *0  1*-----------*1  0*----- tdata_swap
//                          -+--             -+--
//                           |                |
//                           / 16             / 16
//                           |                |
//                           V                V
//                     Texture Memory   Texture Memory
//                      Data [31:16]     Data [15:0]


typedef uint32_t TextureLevelOfDetail_t;
namespace TextureLevelOfDetail
{
    enum
    {
        LodMin =                0x0000003f, // 5:0 lodmin Minimum LOD. (4.2 unsigned)

        LodMax =                0x00000fc0, // 11:6 lodmax Maximum LOD. (4.2 unsigned)

        LodBias =               0x0003f000, // 17:12 lodbias LOD Bias. (4.2 signed)

        LodContent =            0x00040000, // 18 lod_odd LOD odd (0=even, 1=odd)
        LodEven =               0x00000000,
        LodOdd =                0x00040000,

        LodMakeup =             0x00080000, // 19 lod_tsplit Texture is Split. (0=texture contains all LOD levels, 1=odd or even levels only, as
                                            // controlled by lod_odd)
        LodComplete =           0x00000000,
        LodSplit =              0x00080000,

        LodS =                  0x00100000, // 20 lod_s_is_wider S dimension is wider, for rectilinear texture maps. This is a don’t care for square
                                            // textures. (1=S is wider than T).
        LodSNotWider =          0x00000000,
        LodSWider =             0x00100000,

        LodAspectRatio =        0x00600000, // 22:21 lod_aspect Aspect ratio. Equal to 2^n. (00 is square texture, others are rectilinear: 01 is
                                            // 2x1/1x2, 10 is 4x1/1x4, 11 is 8x1/1x8)
        LodAspect1x1 =          0x00000000,
        LodAspect1x2 =          0x00200000,
        LodAspect2x1 =          0x00200000,
        LodAspect1x4 =          0x00400000,
        LodAspect4x1 =          0x00400000,
        LodAspect1x8 =          0x00600000,
        LodAspect8x1 =          0x00600000,

        LodFraction =           0x00800000, // 23 lod_zerofrac LOD zero frac, useful for bilinear when even and odd levels are split across two
                                            // TREXs (0=normal LOD frac, 1=force fraction to 0)
        LodFractionNormal =     0x00000000,
        LodFractionZero =       0x00800000,

        Addressing =            0x01000000, // 24 tmultibaseaddr Use multiple texbaseAddr registers
        SingleAddress =         0x00000000,
        MultiAddress =          0x01000000,

        ByteOrdering =          0x02000000, // 25 tdata_swizzle Byte swap incoming texture data (bytes 0<->3, 1<->2).
        ByteNormal =            0x00000000,
        ByteSwizzle =           0x02000000,

        WordOrdering =          0x04000000, // 26 tdata_swap Short swap incoming texture data (shorts 0<->1).
        WordNormal =            0x00000000,
        WordSwap =              0x04000000,

                                            // 27 reserved used to be tdirect_write in Voodoo graphics.

        SOrientation =          0x10000000, // 28 tmirrors Mirror texture in S dimension
        SNormal =               0x00000000,
        SMirrored =             0x10000000,

        TOrientation =          0x20000000, // 29 tmirrort Mirror texture in T dimension
        TNormal =               0x00000000,
        TMirrored =             0x20000000
    };

    namespace Shift
    {
        enum
        {
            LodMin = 0x00,
            LodMax = 0x06,
            LodBias = 0x0c,
            LodContent = 0x12,
            LodMakeup = 0x13,
            LodS = 0x14,
            LodAspectRatio = 0x15,
            LodFraction = 0x17,
            Addressing = 0x18,
            ByteOrdering = 0x19,
            WordOrdering = 0x1a,
            SOrientation = 0x1c,
            TOrientation = 0x1d
        };
    }
}

}