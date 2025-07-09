//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiPixelsIn = 0x14c
    };
}

// fbiPixelsIn Register
// The fbiPixelsIn register is a 24-bit counter which is incremented for each pixel processed by the Banshee
// triangle walking engine. fbiPixelsIn is incremented irrespective if the triangle pixel is actually drawn or
// not as a result of the depth test, alpha test, etc. fbiPixelsIn is used primarily for statistical information,
// and in essence allows software to count the number of pixels in a screen-space triangle. fbiPixelsIn is
// reset to 0x0 on power-up reset, and is reset when a ‘1’ if written to the lsb of nopCMD.

typedef uint32_t FbiPixelsIn_t;
namespace FbiPixelsIn
{
    enum
    {
        Value = 0x00ffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}