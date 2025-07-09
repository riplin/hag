//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        sARGB = 0x26c
    };
}

// sARGB Register
// The ARGB register specifies the color at the current vertex in a packed 32 bit value.

typedef uint32_t sARGB_t;
namespace sARGB
{
    enum
    {
        Blue = 0x000000ff,
        Green = 0x0000ff00,
        Red = 0x00ff0000,
        Alpha = 0xff000000
    };

    namespace Shift
    {
        enum
        {
            Blue = 0x00,
            Green = 0x08,
            Red = 0x10,
            Alpha = 0x18
        };
    }
}

}