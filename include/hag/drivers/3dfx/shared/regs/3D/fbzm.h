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
        RGBDepthMode = 0x110
    };
}

// fbzMode Register
// The fbzMode register controls frame buffer and depth buffer rendering functions of the Banshee
// processor. Bits in fbzMode control clipping, chroma-keying, depth-buffering, dithering, and masking.

// Bit(0) of fbzMode is used to enable the clipping register. When set, clipping to the rectangle defined by
// the clipLeftRight and clipBottomTop registers inclusive is enabled. When clipping is enabled, the
// bounding clipping rectangle must always be less than or equal to the screen resolution in order to clip to
// screen coordinates. Also note that if clipping is not enabled, rendering may not occur outside of the
// screen resolution. Bit(1) of fbzMode is used to enable the color compare check (chroma-keying). When
// enabled, any source pixel matching the color specified in the chromaKey register is not written to the
// RGB buffer. The chroma-key color compare is performed immediately after texture mapping lookup, but
// before the color combine unit and fog in the pixel datapath.

// Bit(2) of fbzMode is used to enable stipple register masking. When enabled, bit(12) of fbzMode is used
// to determine the stipple mode -- bit(12)=0 specifies stipple rotate mode, while bit(12)=1 specifies stipple
// pattern mode.

// When stipple register masking is enabled and stipple rotate mode is selected, bit(31) of the stipple register
// is used to mask pixels in the pixel pipeline. For all triangle commands and linear frame buffer writes
// through the pixel pipeline, pixels are invalidated in the pixel pipeline if stipple bit(31)=0 and stipple
// register masking is enabled in stipple rotate mode. After an individual pixel is processed in the pixel
// pipeline, the stipple register is rotated from right-to-left, with the value of bit(0) filled with the value of
// bit(31). Note that the stipple register is rotated regardless of whether stipple masking is enabled (bit(2) in
// fbzMode) when in stipple rotate mode.

// When stipple register masking is enabled and stipple pattern mode is selected, the spatial <x,y>
// coordinates of a pixel processed in the pixel pipeline are used to lookup a 4x8 monochrone pattern stored
// in the stipple register -- the resultant lookup value is used to mask pixels in the pixel pipeline. For all
// triangle commands and linear frame buffer writes through the pixel pipeline, a stipple bit is selected from
// the stipple register as follows:

// switch(pixel_Y[1:0])
// {
// case 0: stipple_Y_sel[7:0] = stipple[7:0];
// case 1: stipple_Y_sel[7:0] = stipple[15:8];
// case 2: stipple_Y_sel[7:0] = stipple[23:16];
// case 3: stipple_Y_sel[7:0] = stipple[31:24];
// }

// switch(pixel_X[2:0]
// {
// case 0: stipple_mask_bit = stipple_Y_sel[7];
// case 1: stipple_mask_bit = stipple_Y_sel[6];
// case 2: stipple_mask_bit = stipple_Y_sel[5];
// case 3: stipple_mask_bit = stipple_Y_sel[4];
// case 4: stipple_mask_bit = stipple_Y_sel[3];
// case 5: stipple_mask_bit = stipple_Y_sel[2];
// case 6: stipple_mask_bit = stipple_Y_sel[1];
// case 7: stipple_mask_bit = stipple_Y_sel[0];
// }

// If the stipple_mask_bit=0, the pixel is invalidated in the pixel pipeline when stipple register masking is
// enabled and stipple pattern mode is selected. Note that when stipple pattern mode is selected the stipple
// register is never rotated.

// Bits(4:3) specify the depth-buffering function during rendering operations. The depth buffering pipeline
// is shown below:

// iterated W[47:0],                    iterated Z[31:0], unclamped
//    unclamped                         treat as 4.28 value, line up      iterated W[47:0], unclamped
//        |                             decimal points with 16.32 w-term  |                                         iterated Z[27:12], clamped
//        / 48                          and zero extended to 48 bits      |                                         |
//        |                                                       |       |                                         |
//        |                                                    ---+-------+---                                      |
//        |                                                     * 1       0 *-- depthfloat_select                   |
//        |                                                      -----+-----                                        |
//        |                                                           |                                             |
//        |                                                           / 48 floatSel                                 |
//        |                                                           |                                             |
// +------+-------------------------------------------+ +-------------+--------------------------------------+      |
// | if(|w-iter[47:32])                               | | if(|floatSel[47:32])                               |      |
// | {                                                | | {                                                  |      |
// |     mant = 0, exp = 0xf, underflow = 1           | |     mant = 0, exp = 0xf, underflow = 1             |      |
// | }                                                | | }                                                  |      / 16 (integer only)
// | else if(!| w-iter[31:16])                        | | else if(!| floatSel[31:16])                        |      |
// | {                                                | | {                                                  |      |
// |     mant = 1, exp = 0xf, underflow = 0           | |     mant = 1, exp = 0xf, underflow = 0             |      |
// | }                                                | | }                                                  |      |
// | else                                             | | else                                               |      |
// | {                                                | | {                                                  |      |
// |     exp = find_first_one(w=iter[31:16])          | |     exp = find_first_one(floatSel[31:16])          |      |
// |     mant = (w-iter[30:16] << exp), underflow = 0 | |     mant = (floatSel[30:16] << exp), underflow = 0 |      |
// | }                                                | | }                                                  |      |
// +-----------+--------+-----------------------------+ +-------+------------+--------+----------------------+      |
//             |        |                                       |            |        |                             |
//  exponent 4 /        _                                       | exponent 4 /        _                             |
//             |        V  (inverted)                           |            |        V  (inverted)                 |
//             |        o                           underflow 1 /            |        o                             |
//             |        |                                       |            |        |                             |
//           +-+-+------+----+                                  V          +-+-+------+----+                        |
//           | 4 |     12    |                           To adder logic    | 4 |     12    |                        |
//           +---+--+--------+                                             +---+--+--------+                        |
//                  |                  wfloat format:                             |                                 |
//                  / 16              1.<mant> * 2^exp                            / 16   +--------------------------+
//                  |                                                             |      |
//                  |                                                          ---+------+---
//                  |                                                           * 1      0 *--- wfloat_select
//                  |                                                            -----+----
//                  |                                                                 |
//                  |                                                                 / 16
//                  |                                                                 |             _-|--- zaColor[15:0]
//                  |         +-------------------------------------------------------+---------*--*  |
//                  |         |                                                       |         |   -_|--- zbias_enable
//                 -+---   ---+-                                                     -+---   ---+-
//                  *    V    *--- cin = 1                                            *    V    *--- wfloat_select
//                   ----+----                                                         ----+----
//                       |                                                                 |        1. Sign extend 16-bit zaColor to 18 bits
//                  +----+----+                                                       +----+----+   2. Convert 16-bit depth to 18-bit
//                  |  Clamp  |                                                       |  Clamp  |      {underflow,underflow,depth}
//                  +----+----+                                                       +----+----+   3. Add 18-bit values
//                       |                                                                 |        4. Clamp to 0-FFFF
//              <--------+                                            +--------------------*--------------------+
//      To Fog Unit                                                   |                                         |
//                                                                    |                old Depth                |
//                                                                    |           (from Depth Buffer)           |
//                                                                    |                    |                    |
//                                                                    |       +------------*------------+       |
//                                                                    |       |                         |       |
//                                                                  +-+-------+-+                     +-+-------+-+
//                                                                  |     <?    |                     |     =?    |
//                                                                  +-----+-----+                     +-----+-----+
//                                                                        |                                 |
//                                                                        / 1                               / 1
//                                                                        |                                 |
//                                                         zfunc_lt ----+ *---------------+ +---------------* +--- zfunc_eq
//                                                                      | |               | |               | |
//                                                                      ---               -_-               ---
//                                                                      | |               | |               | |
//                                                                       *                 o                 *
//                                                                       |                 |                 |
//                                                                       |  zfunc_gt ----+ |                 |
//                                                                       |               | |                 |
//                                                                       |               ---                 |
//                                                                       |               | |                 |
//                                                                       |                *                  |
//                                                                       +--------------+ | +----------------+
//                                                                                      | | |
//                                              Depth Buffer Enable -|>o--------------+ | | |
//                                                                                    | | | |
//                                                                                   _| | | |_
//                                                                                   |--___--|
//                                                                                   |       |
//                                                                                    |     |
//                                                                                     |   |
//                                                                                     -___-
//                                                                                       |
//                                                                                       |
//                                                                                       V Depth test pass
//

// Bit(4) of fbzMode is used to enable depth-buffering. When depth buffering is enabled, a depth
// comparison is performed for each source pixel as defined in bits(7:5). When bit(3)=0, the z iterator is
// used for the depth buffer comparison. When bit(3)=1, the w iterator is used for the depth buffer
// comparison. When bit(3)=1 enabling w-buffering, the inverse of the normalized w iterator is used for the
// depth-buffer comparison. This in effect implements a floating-point w-buffering scheme utilizing a 4-bit
// exponent and a 12-bit mantissa. The inverted w iterator is used so that the same depth buffer comparisons
// can be used as with a typical z-buffer. Section 5.19.1 below further describes the depth-buffering
// algorithm.

// Bit(8) of fbzMode enables 16-bit color dithering. When enabled, native 24-bit source pixels are dithered
// into 16-bit RGB color values with no performance penalty. When dithering is disabled, native 24-bit
// source pixels are converted into 16-bit RGB color values by bit truncation. When dithering is enabled,
// bit(11) of fbzMode defines the dithering algorithm -- when bit(11)=0 a 4x4 ordered dither algorithm is
// used, and when bit(11)=1 a 2x2 ordered dither algorithm is used to convert 24-bit RGB pixels into 16-bit
// frame buffer colors.

// Bit(9) of fbzMode enables writes to the RGB buffers. Clearing bit(9) invalidates all writes to the RGB
// buffers, and thus the RGB buffers remain unmodified for all rendering operations. Bit(9) must be set for
// normal drawing into the RGB buffers. Similarly, bit(10) enables writes to the depth-buffer. When
// cleared, writes to the depth-buffer are invalidated, and the depth-buffer state is unmodified for all
// rendering operations. Bit(10) must be set for normal depth-buffered operation.

// Bit(13) of fbzMode enables the alpha-channel mask. When enabled, bit(0) of the incoming alpha value is
// used to mask writes to the color and depth buffers. If alpha channel masking is enabled and bit(0) of the
// incoming alpha value is 0, then the pixel is invalidated in the pixel pipeline, the fbiAfuncFail register is
// incremented, and no drawing occurs to the color or depth buffers. If alpha channel masking is enabled
// and bit(0) of the incoming alpha value is 1, then the pixel is drawn normally subject to depth function,
// alpha blending function, alpha test, and color/depth masking.

// Bit(16) of fbzMode is used to enable the Depth Buffer bias. When bit(16)=1, the calculated depth value
// (irrespective of Z or 1/W type of depth buffering selected) is added to bits(15:0) of zaColor. Depth buffer
// biasing is used to elimate aliasing artifacts when rendering co-planar polygons.

// Bit(17) of fbzMode is used to define the origin of the Y coordinate for rendering operations (FASTFILL
// and TRIANGLE commands) and linear frame buffer writes when the pixel pipeline is bypassed for linear
// frame buffer writes (lfbMode bit(8)=0). Note that bit(17) of fbzMode does not affect linear frame buffer
// writes when the pixel pipeline is bypassed for linear frame buffer writes (lfbMode bit(8)=0), as in this
// situation bit(13) of lfbMode specifies the Y origin for linear frame buffer writes. When cleared, the Y
// origin (Y=0) for all rendering operations and linear frame buffer writes when the pixel pipeline is enabled
// is defined to be at the top of the screen. When bit(17) is set, the Y origin is defined to be at the bottom of
// the screen.

// Bit(18) of fbzMode is used to enable the destination alpha planes. When set, the auxiliary buffer will be
// used as destination alpha planes. Note that if bit(18) of fbzMode is set that depth buffering cannot be
// used, and thus bit(4) of fbzMode (enable depth buffering) must be set to 0x0.

// Bit(19) of fbzMode is used to enable dither subtraction on the destination color during alpha blending.
// When dither subtraction is enabled (fbzMode bit(19)=1), the dither matrix used to convert 24-bit color to
// 16-bit color is subtracted from the destination color before applying the alpha-blending algorithm.
// Enabling dither subtraction is used to enhance image quality when performing alpha-blending.

// Bit(20) of fbzMode is used to select the source depth value used for depth buffering. When fbzMode
// bit(20)=0, the source depth value used for the depth buffer comparison is either iterated Z or iterated W
// (as selected by fbzMode bit(3)) and may be biased (as controlled by fbzMode bit(16)). When fbzMode
// bit(20)=1, the constant depth value defined by zaColor[15:0] is used as the source depth value for the
// depth buffer comparison. Regardless of the state of fbzMode bit(20), the biased iterated Z/W is written
// into the depth buffer if the depth buffer function passes.

// Bit(21) of fbzMode is used to select either the w iterator or the z iterator to be used for floating point
// depth buffering. Floating point depth buffering is enabled when fbzMode bit(4)=1. When fbzMode
// bit(21)=0, then the unclamped w iterator is converted to a 4.12 floating point representation and used for
// depth buffering. When fbzMode bit(21)=1, then the unclamped z iterator is converted into a 4.12
// floating point format and used for depth buffering.

// Depth-buffering function
// When the depth-buffering is enabled (fbzMode bit(4)=1), the following depth comparison is performed:
//     DEPTHsrc DepthOP DEPTHdst
// where DEPTHsrc and DEPTHdst represent the depth source and destination values respectively. A source
// pixel is written into an RGB buffer if the depth comparison is true and writing into the RGB buffer is
// enabled (fbzMode bit(9)=1). The source depth value is written into the depth buffer if the depth
// comparison is true and writing into the depth buffer is enabled (fbzMode bit(10)=1). The supported
// depth comparison functions (DepthOPs) are shown below:

// Value       DepthOP Function
// 0           never
// 1           less than
// 2           equal
// 3           less than or equal
// 4           greater than
// 5           not equal
// 6           greater than or equal
// 7           always

typedef uint32_t RGBDepthMode_t;
namespace RGBDepthMode
{
    enum
    {
        Clipping =                          0x00000001, // 0 Enable clipping rectangle (1=enable)
        ClipDisable =                       0x00000000,
        ClipEnable =                        0x00000001,

        ChromaKey =                         0x00000002, // 1 Enable chroma-keying (1=enable)
        ChromaKeyDisable =                  0x00000000,
        ChromaKeyEnable =                   0x00000002,

        StippleMasking =                    0x00000004, // 2 Enable stipple register masking (1=enable)
        StippleMaskingDisable =             0x00000000,
        StippleMaskingEnable =              0x00000004,
        
        DepthBufferType =                   0x00000008, // 3 W-Buffer Select (0=Use Z-value for depth buffering, 1=Use W-value for depth buffering)
        DepthBufferZ =                      0x00000000,
        DepthBufferW =                      0x00000008,

        DepthBuffer =                       0x00000010, // 4 Enable depth-buffering (1=enable)
        DepthBufferDisable =                0x00000000,
        DepthbufferEnable =                 0x00000010,

        DepthFunction =                     0x000000e0, // 7:5 Depth-buffer function (see table below)
        DepthFuncNever =                    0x00000000,
        DepthFuncLessThan =                 0x00000020,
        DepthFuncEqual =                    0x00000040,
        DepthFuncLessThanOrEqual =          0x00000060,
        DepthFuncGreaterThan =              0x00000080,
        DepthFuncNotEqual =                 0x000000a0,
        DepthFuncGreaterThanOrEqual =       0x000000c0,
        DepthFuncAlways =                   0x000000e0,

        Dithering =                         0x00000100, // 8 Enable dithering (1=enable)
        DitheringDisable =                  0x00000000,
        DitheringEnable =                   0x00000100,

        RGBWriteMask =                      0x00000200, // 9 RGB buffer write mask (0=disable writes to RGB buffer)
        RGBWriteMaskDisable =               0x00000000,
        RGBWriteMaskEnable =                0x00000200,

        DepthAlphaWriteMask =               0x00000400, // 10 Depth/alpha buffer write mask (0=disable writes to depth/alpha buffer)
        DepthAlphaWriteMaskDisable =        0x00000000,
        DepthAlphaWriteMaskEnable =         0x00000400,

        DitherAlgorithm =                   0x00000800, // 11 Dither algorithm (0=4x4 ordered dither, 1=2x2 ordered dither)
        DitherAlgorithmOrdered4x4 =         0x00000000,
        DitherAlgorithmOrdered2x2 =         0x00000800,
        
        StipplePatternMasking =             0x00001000, // 12 Enable Stipple pattern masking (1=enable)
        StipplePatternMaskingDisable =      0x00000000,
        StipplePatternMaskingEnable =       0x00001000,

        AlphaChannelMask =                  0x00002000, // 13 Enable Alpha-channel mask (1=enable alpha-channel masking)
        AlphaChannelMaskDisable =           0x00000000,
        AlphaChannelMaskEnabled =           0x00002000,

        DepthAliasing =                     0x00010000, // 16 Enable depth-biasing (1=enable)
        DepthAliasingDisable =              0x00000000,
        DepthAliasingEnable =               0x00010000,

        RenderCommandsYOrigin =             0x00020000, // 17 Rendering commands Y origin (0=top of screen is origin, 1=bottom of screen is origin)
        RenderCommandsYTop =                0x00000000,
        RenderCommandsYBottom =             0x00020000,

        AlphaPlanes =                       0x00040000, // 18 Enable alpha planes (1=enable)
        AlphaPlanesDisable =                0x00000000,
        AlphaPlanesEnable =                 0x00040000,

        AlphaBlendDitherSubtraction =       0x00080000, // 19 Enable alpha-blending dither subtraction (1=enable)
        AlphaDitherSubtractionDisable =     0x00000000,
        AlphaDitherSubtractionEnable =      0x00080000,

        DepthBufferSourceCompareSelect =    0x00100000, // 20 Depth buffer source compare select (0=normal operation, 1=zaColor[15:0])
        DepthBufferSourceCompareNormal =    0x00000000,
        DepthBufferSourceCompareZAColor =   0x00100000,

        DepthFloatSelect =                  0x00200000, // 21 Depth float select (0=iterated W is used for floating point depth buffering, 1=iterated Z
        DepthFloatIteratedW =               0x00000000, // is used for floating point depth buffering)
        DepthFloatIteratedZ =               0x00200000
    };
    
    namespace Shift
    {
        enum
        {

        };
    }
}

}