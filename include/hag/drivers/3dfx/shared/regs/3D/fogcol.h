//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FogColor = 0x12c
    };
}

// fogColor Register
// The fogColor register is used to specify the fog color for fogging operations. Fog is enabled by setting
// bit(0) in fogMode. See the fogMode and fogTable register descriptions for more information fog.

typedef uint32_t FogColor_t;
namespace FogColor
{
    enum
    {
        Blue = 0x000000ff,
        Green = 0x0000ff00,
        Red = 0x00ff0000
    };

    namespace Shift
    {
        enum
        {
            Blue = 0x00,
            Green = 0x08,
            Red = 0x010
        };
    }
}

}