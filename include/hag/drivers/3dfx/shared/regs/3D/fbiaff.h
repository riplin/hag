//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiAFuncFail = 0x158
    };
}

// fbiAfuncFail Register
// The fbiAfuncFail register is a 24-bit counter which is incremented each time an incoming source pixel
// (either from the triangle engine or linear frame buffer writes through the pixel pipeline) is invalidated in
// the pixel pipeline because of a failure in the alpha test. The alpha test is defined and enabled in the
// alphaMode register. The fbiAfuncFail register is also incremented if an incoming source pixel is
// invalidated in the pixel pipeline as a result of the alpha masking test (bit(13) in fbzMode). fbiAfuncFail
// is reset to 0x0 on power-up reset, and is reset when a ‘1’ if written to the lsb of nopCMD.

typedef uint32_t FbiAFuncFail_t;
namespace FbiAFuncFail
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