//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        SourceSize = 0x058,
        DestinationSize = 0x068
    };
}

// These registers are used only for blts and rectangle fills. They contain the height and width in pixels of
// the source and destination rectangles. The srcSize register will only be used in Stretch-blt modes. For
// non-stretched blts, the blt source size will be the same as the blt destination size, determined by the
// dstSize register.

typedef uint32_t Size_t;
namespace Size
{

    enum
    {
        BltWidth = 0x00001fff,
        BltHeight = 0x1fff0000
    };

    namespace Shift
    {
        enum
        {
            BltWidth = 0x00,
            BltHeight = 0x10
        };
    }

}

}