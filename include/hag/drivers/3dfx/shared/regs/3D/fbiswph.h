//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        FbiSwapHistory = 0x258
    };
}

// fbiSwapHistory Register
// The fbiSwapHistory register keeps track of the number of vertical syncs which occur between executed
// swap commands. fbiSwapHistory logs this information for the last 8 executed swap commands. Upon
// completion of a swap command, fbiSwapHistory bits (27:0) are shifted left by four bits to form the new
// fbiSwapHistory bits (31:4), which maintains a history of the number of vertical syncs between execution
// of each swap command for the last 7 frames. Then, fbiSwapHistory bits(3:0) are updated with the
// number of vertical syncs which occurred between the last swap command and the just completed swap
// command or the value 0xf, whichever is less.

typedef uint32_t FbiSwapHistory_t;
namespace FbiSwapHistory
{
    enum
    {
        VSyncCountBeforeLastSwap = 0x0000000f,  // 3:0 Number of vertical syncs between the second most recently completed swap command
                                                // and the most recently completed swap command, or the value 0xf, whichever is less for
                                                // Frame N.
        VSyncCountBeforeSwapN1 = 0x000000f0,    // 7:4 Vertical sync swapbuffer history for Frame N-1
        VSyncCountBeforeSwapN2 = 0x00000f00,    // 11:8 Vertical sync swapbuffer history for Frame N-2
        VSyncCountBeforeSwapN3 = 0x0000f000,    // 15:12 Vertical sync swapbuffer history for Frame N-3
        VSyncCountBeforeSwapN4 = 0x000f0000,    // 19:16 Vertical sync swapbuffer history for Frame N-4
        VSyncCountBeforeSwapN5 = 0x00f00000,    // 23:20 Vertical sync swapbuffer history for Frame N-5
        VSyncCountBeforeSwapN6 = 0x0f000000,    // 27:24 Vertical sync swapbuffer history for Frame N-6
        VSyncCountBeforeSwapN7 = 0xf0000000     // 31:28 Vertical sync swapbuffer history for Frame N-7
    };

    namespace Shift
    {
        enum
        {
            VSyncCountBeforeLastSwap = 0x00,
            VSyncCountBeforeSwapN1 = 0x04,
            VSyncCountBeforeSwapN2 = 0x08,
            VSyncCountBeforeSwapN3 = 0x0c,
            VSyncCountBeforeSwapN4 = 0x10,
            VSyncCountBeforeSwapN5 = 0x14,
            VSyncCountBeforeSwapN6 = 0x18,
            VSyncCountBeforeSwapN7 = 0x1c
        };
    }
}

}