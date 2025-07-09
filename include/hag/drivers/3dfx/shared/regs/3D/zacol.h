//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        DepthAlphaConstant = 0x130
    };
}

// zaColor Register
// The zaColor register is used to specify constant alpha and depth values for linear frame buffer writes,
// FASTFILL commands, and co-planar polygon rendering support. For certain linear frame buffer access
// formats, the alpha and depth values associated with a pixel written are the values specified in zaColor.
// See the lfbMode register description for more information. When executing the FASTFILL command,
// the constant 16-bit depth value written into the depth buffer is taken from bits(15:0) of zaColor. When
// fbzMode bit(16)=1 enabling depth-biasing, the constant depth value required is taken from zaColor
// bits(15:0).

typedef uint32_t DepthAlphaConstant_t;
namespace DepthAlphaConstant
{
    enum
    {
        Depth = 0x0000ffff,
        Alpha = 0xff000000
    };

    namespace Shift
    {
        enum
        {
            Depth = 0x00,
            Alpha = 0x24
        };
    }
}

}