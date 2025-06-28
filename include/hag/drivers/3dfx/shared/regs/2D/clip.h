//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        Clip0Min = 0x008,
        Clip0Max = 0x00c,
        Clip1Min = 0x04c,
        Clip1Max = 0x050
    };
}

// The clip registers define the maximum and minimum x & y values of pixel that can be written in the
// destination pixel map. There are two sets of clip registers, however, only one set is used at a time, as
// determined by the clip select bit in the command register.
// Clipping is inclusive of the minimum values, and exclusive of the maximum values. Thus if the clip
// select bit is zero, only pixels with x values in the range [clip0Min_x, clip0Max_x) and y values in the
// range [clip0Min_y, clip0Max_y) will be written.

typedef uint32_t Clip_t;
namespace Clip
{

    enum
    {
        X = 0x00000fff, // 11:0 x minimum clip
        Y = 0x0fff0000  // 27:16 y minimum clip
    };

    namespace Shift
    {
        enum
        {
            X = 0x00,
            Y = 0x10
        };
    }
}

}