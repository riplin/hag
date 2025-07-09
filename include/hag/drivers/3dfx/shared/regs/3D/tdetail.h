//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TextureDetail = 0x308
    };
}

// tDetail Register
// The tDetail register controls the detail texture.

// Bit     Name                    Description
// 7:0     detail_max              Detail texture LOD clamp (8.0 unsigned)
// 13:8    detail_bias             Detail texture bias (6.0 signed)
// 16:14   detail_scale            Detail texture scale shift left
// 17      rgb_tminfilter          RGB texture minification filter(0 = point-sampled, 1 = bilinear)
// 18      rgb_tmagfilter          RGB texture magnification filter(0 = point-sampled, 1 = bilinear)
// 19      a_tminfilter            Alpha texture minification filter(0 = point-sampled, 1 = bilinear)
// 20      a_tmagfilter            Alpha texture magnification filter(0 = point-sampled, 1 = bilinear)
// 21      rgb_a_separate_filter   0 = tminfilter and tmagfilter of textureMode define the filter for RGBA
//                                 1 = rgb_tminfilter and rgb_tmagfilter define the filter for RGB,
//                                     a_tminfilter and a_tmagfilter define the filter for alpha.

// detail_factor is used in the Texture Combine Unit to blend between the main texture and the detail
// texture.
// detail_factor (0.8 unsigned) = max(detail_max, ((detail_bias - LOD) << detail_scale)).

// When rgb_a_separate_filter is set, rgb_tminfilter and rgb_tmagfilter are used for RGB filtering and
// a_tminfilter and a_tmagfilter are used for A filtering.

typedef uint32_t TextureDetail_t;
namespace TextureDetail
{
    enum
    {
        LodMax = 0x000000ff, // 7:0 detail_max Detail texture LOD clamp (8.0 unsigned)
        
        Bias = 0x00003f00, // 13:8 detail_bias Detail texture bias (6.0 signed)
        
        Scale = 0x0001c000, // 16:14 detail_scale Detail texture scale shift left
        
        RGBMinificationFilter = 0x00020000, // 17 rgb_tminfilter RGB texture minification filter(0 = point-sampled, 1 = bilinear)
        RGBMinificationPoint = 0x00000000,
        RGBMinificationBilinear = 0x00020000,

        RGBMagnificationFilter = 0x00040000, // 18 rgb_tmagfilter RGB texture magnification filter(0 = point-sampled, 1 = bilinear)
        RGBMagnificationPoint = 0x00000000,
        RGBMagnificationBilinear = 0x00040000,

        AlphaMinificationFilter = 0x00080000, // 19 a_tminfilter Alpha texture minification filter(0 = point-sampled, 1 = bilinear)
        AlphaMinificationPoint = 0x00000000,
        AlphaMinificationBilinear = 0x00080000,

        AlphaMagnificationFilter = 0x00100000, // 20 a_tmagfilter Alpha texture magnification filter(0 = point-sampled, 1 = bilinear)
        AlphaMagnificationPoint = 0x00000000,
        AlphaMagnificationBilinear = 0x00100000,

        RGBAndAlphaFiltering = 0x00200000, // 21 rgb_a_separate_filter 0 = tminfilter and tmagfilter of textureMode define the filter for RGBA
                                           // 1 = rgb_tminfilter and rgb_tmagfilter define the filter for RGB,
                                           // a_tminfilter and a_tmagfilter define the filter for alpha.
        RGBACombinedFiltering = 0x00000000,
        RGBAndAlphaSeparateFiltering = 0x00200000
    };

    namespace Shift
    {
        enum
        {
            LodMax = 0x00,
            Bias = 0x08,
            Scale = 0x0e,
            RGBMinificationFilter = 0x11,
            RGBMagnificationFilter = 0x12,
            AlphaMinificationFilter = 0x13,
            AlphaMagnificationFilter = 0x14,
            RGBAndAlphaFiltering = 0x15
        };
    }
}

}