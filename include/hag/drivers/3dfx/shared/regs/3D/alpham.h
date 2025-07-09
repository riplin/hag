//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        EnableFunction =                    0x00000001, // Enable alpha function (1=enable)

        AlphaFunction =                     0x0000000e,
        FuncNever =                         0x00000000,
        FuncLessThan =                      0x00000002,
        FuncEqual =                         0x00000004,
        FuncLessThanOrEqual =               0x00000006,
        FuncGreaterThan =                   0x00000008,
        FuncNotEqual =                      0x0000000a,
        FuncGreaterThanOrEqual =            0x0000000c,
        FuncAlways =                        0x0000000e,

        EnableBlending =                    0x00000010,

        SourceRGBBlendingFactor =           0x00000f00,
        SrcRGBFZero =                       0x00000000,
        SrcRGBFSourceAlpha =                0x00000100,
        SrcRGBFColor =                      0x00000200,
        SrcRGBFDestinationAlpha =           0x00000300,
        SrcRGBFOne =                        0x00000400,
        SrcRGBFOneMinusSourceAlpha =        0x00000500,
        SrcRGBFOneMinusColor =              0x00000600,
        SrcRGBFOneMinusDestinationAlpha =   0x00000700,
        SrcRGBFASaturate =                  0x00000f00,

        DestinationRGBBlendingFactor =      0x0000f000,
        DstRGBFZero =                       0x00000000,
        DstRGBFSourceAlpha =                0x00001000,
        DstRGBFColor =                      0x00002000,
        DstRGBFDestinationAlpha =           0x00003000,
        DstRGBFOne =                        0x00004000,
        DstRGBFOneMinusSourceAlpha =        0x00005000,
        DstRGBFOneMinusColor =              0x00006000,
        DstRGBFOneMinusDestinationAlpha =   0x00007000,
        DstRGBFColorBeforeFog =             0x0000f000,

        SourceAlphaBlendingFactor =         0x000f0000,
        SrcAFZero =                         0x00000000,
        SrcAFSourceAlpha =                  0x00010000,
        SrcAFColor =                        0x00020000,
        SrcAFDestinationAlpha =             0x00030000,
        SrcAFOne =                          0x00040000,
        SrcAFOneMinusSourceAlpha =          0x00050000,
        SrcAFOneMinusColor =                0x00060000,
        SrcAFOneMinusDestinationAlpha =     0x00070000,
        SrcAFASaturateAntiAlias =           0x000f0000,

        DestinationAlphaBlendingFactor =    0x00f00000,
        DstAFZero =                         0x00000000,
        DstAFSourceAlpha =                  0x00100000,
        DstAFColor =                        0x00200000,
        DstAFDestinationAlpha =             0x00300000,
        DstAFOne =                          0x00400000,
        DstAFOneMinusSourceAlpha =          0x00500000,
        DstAFOneMinusColor =                0x00600000,
        DstAFOneMinusDestinationAlpha =     0x00700000,
        DstAFDestinationBlendFactor =       0x00f00000,
        
        AlphaReferenceValue =               0xff000000
    };
}

// alphaMode Register
// The alphaMode register controls the alpha blending and anti-aliasing functionality of Banshee.

// Bits(3:1) specify the alpha function during rendering operations. The alpha function and test pipeline is
// shown below:

//   Alpha from Alpha
//    Combine Unit
//        |
//        *---------------------+
//        |                     |
//        |   alphaMode(31:24)  |
//        |          |          |
//        |  +-------+-------+  |
//        |  |               |  |
//      +-+--+-+           +-+--+-+
//      |  <?  |           |  =?  |
//      +---+--+           +--+---+
//          |                 |
//          / 1               / 1
// afunc_lt |                 |   afunc_eq
//     ---+ +-------+ +-------+ +---
//        | |       | |       | |
//        ---       -_-       ---
//        | |       | |       | |
//         *         o         *
//         |         |         |
//         | afunc_gt|         |
//         |       | |         |
//         |       ---         |
//         |       | |         |
//         |        *          |
//         +------+ | +--------+
//                | | |
//  +---|>o-----+ | | |
//  |           | | | |
// Alpha Test  _| | | | _
// Enable      |--____--|
//             |        |
//              |      |
//               |    |
//               -____-
//                  |
//                  V Alpha Test Pass

// When alphaMode bit(0)=1, an alpha comparison is performed between the incoming source alpha and
// bits(31:24) of alphaMode. Section 5.18.1 below further describes the alpha function algorithm.
// Bit(4) of alphaMode enables alpha blending. When alpha blending is enabled, the blending function is
// performed to combine the source color with the destination pixel. The blending factors of the source and
// destinations pixels are individually programmable, as determined by bits(23:8). Note that the RGB and
// alpha color channels may have different alpha blending factors. Section 5.18.2 below further describes
// alpha blending.

// Bit(5) of alphaMode is reserved.

// Alpha function
// When the alpha function is enabled (alphaMode bit(0)=1), the following alpha comparison is performed:
//     AlphaSrc AlphaOP AlphaRef
// where AlphaSrc represents the alpha value of the incoming source pixel, and AlphaRef is the value of
// bits(31:24) of alphaMode. A source pixel is written into an RGB buffer if the alpha comparison is true
// and writing into the RGB buffer is enabled (fbzMode bit(9)=1. If the alpha function is enabled and the
// alpha comparison is false, the fbiAfuncFail register is incremented and the pixel is invalidated in the
// pixel pipeline and no drawing occurs to the color or depth buffers. The supported alpha comparison
// functions (AlphaOPs) are shown below:

// Value     AlphaOP Function
// 0          never
// 1          less than
// 2          equal
// 3          less than or equal
// 4          greater than
// 5          not equal
// 6          greater than or equal
// 7          always

// Alpha Blending
// When alpha blending is enabled (alphaMode bit(4)=1), incoming source pixels are blended with
// destination pixels. The alpha blending function for the RGB color components is as follows:
//      Dnew ⇐ (S ⋅ α) + (Dold ⋅ β)

// where
//      Dnew    The new destination pixel being written into the frame buffer
//      S       The new source pixel being generated
//      Dold    The old (current) destination pixel about to be modified
//      α       The source pixel alpha blending function.
//      β       The destination pixel alpha blending function.

// The alpha blending function for the alpha components is as follows:
//      Anew ⇐ (AS ⋅ αd) + (Aold ⋅ βd)

// where
//      Anew    The new destination alpha being written into the alpha buffer
//      AS      The new source alpha being generated
//      Aold    The old (current) destination alpha about to be modified
//      αd      The source alpha alpha-blending function.
//      βd      The destination alpha alpha-blending function.

// Note that the source and destination pixels may have different associated alpha blending functions. Also
// note that RGB color components and the alpha components may have different associated alpha blending
// functions. The alpha blending factors of the RGB color components are defined in bits(15:8) of
// alphaMode, while the alpha blending factors of the alpha component is specified in bits(23:16) of
// alphaMode. The following table lists the alpha blending functions supported:

//Alpha Blending Function           Alpha Blending Function Pneumonic   Alpha Blending Function Description
// 0x0                                          AZERO                     Zero
// 0x1                                          ASRC_ALPHA                Source alpha
// 0x2                                          A_COLOR                   Color
// 0x3                                          ADST_ALPHA                Destination alpha
// 0x4                                          AONE                      One
// 0x5                                          AOMSRC_ALPHA              1 - Source alpha
// 0x6                                          AOM_COLOR                 1 - Color
// 0x7                                          AOMDST_ALPHA              1 - Destination alpha
// 0x8-0xe                                                                Reserved
// 0xf (source alpha blending function)         ASATURATE                 MIN(Source alpha, 1 - Destination alpha)
// 0xf (destination alpha blending function)    A_COLORBEFOREFOG          Color before Fog Unit

// When the value 0x2 is selected as the destination alpha blending factor, the source pixel color is used as
// the destination blending factor. When the value 0x2 is selected as the source alpha blending factor, the
// destination pixel color is used as the source blending factor. Note also that the alpha blending function
// 0xf is different depending upon whether it is being used as a source or destination alpha blending
// function. When the value 0xf is selected as the destination alpha blending factor, the source color before
// the fog unit (“unfogged” color) is used as the destination blending factor -- this alpha blending function is
// useful for multi-pass rendering with atmospheric effects. When the value 0xf is selected as the source
// alpha blending factor, the alpha-saturate anti-aliasing algorithm is selected -- this MIN function performs
// polygonal anti-aliasing for polygons which are drawn front-to-back.


typedef uint32_t AlphaMode_t;
namespace AlphaMode
{
    enum
    {

    };

    namespace Shift
    {
        enum
        {

        };
    }
}

}