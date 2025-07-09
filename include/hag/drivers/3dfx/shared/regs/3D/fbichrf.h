//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiChromaFail = 0x150
    };
}

// fbiChromaFail Register
// The fbiChromaFail register is a 24-bit counter which is incremented each time an incoming source pixel
// (either from the triangle engine or linear frame buffer writes through the pixel pipeline) is invalidated in
// the pixel pipeline because of the chroma-key color match test. If an incoming source pixel color matches
// the chomaKey register, fbiChromaFail is incremented. fbiChromaFail is reset to 0x0 on power-up
// reset, and is reset when a ‘1’ if written to the lsb of nopCMD.

typedef uint32_t FbiChromaFail_t;
namespace FbiChromaFail
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