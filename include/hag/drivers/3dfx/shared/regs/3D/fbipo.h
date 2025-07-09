//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiPixelsOut = 0x15c
    };
}

// fbiPixelsOut Register
// The fbiPixelsOut register is a 24-bit counter which is incremented each time a pixel is written into a color
// buffer during rendering operations (rendering operations include triangle commands, linear frame buffer
// writes, and the FASTFILL command). Pixels tracked by fbiPixelsOut are therefore subject to the
// chroma-test, Z test, Alpha test, etc. that are part of the regular Banshee pixel pipeline. fbiPixelsOut is
// used to count the number of pixels actually drawn (as opposed to the number of pixels processed counted
// by fbiPixelsIn). Note that the RGB mask (fbzMode bit(9) is ignored when determining fbiPixelsOut.
// fbiPixelsOut is reset to 0x0 on power-up reset, and is reset when a ‘1’ if written to the lsb of nopCMD.

typedef uint32_t FbiPixelsOut_t;
namespace FbiPixelsOut
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