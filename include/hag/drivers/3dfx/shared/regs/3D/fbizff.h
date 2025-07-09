//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiZFuncFail = 0x154
    };
}

// fbiZfuncFail Register
// The fbiZfuncFail register is a 24-bit counter which is incremented each time an incoming source pixel
// (either from the triangle engine or linear frame buffer writes through the pixel pipeline) is invalidated in
// the pixel pipeline because of a failure in the Z test. The Z test is defined and enabled in the fbzMode
// register. fbiZfuncFail is reset to 0x0 on power-up reset, and is reset when a ‘1’ if written to the lsb of
// nopCMD.

typedef uint32_t FbiZFuncFail_t;
namespace FbiZFuncFail
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