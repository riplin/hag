//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        CommandExtra = 0x038
    };
}

// This register contains miscellaneous control bits in addition to those in the command register.

// Bits(1:0) enable colorkeying, if the bit is 0, colorkeying is disabled. Enabling source colorkeying with
// monochrome source, or in line, polyline, polygon, or rectangle modes has no effect. For further
// explanation of these bits, see the description of the colorkey registers.

// If bit(2) is set, the current command, and any following it will not proceed until the next vertical blanking
// period begins. Wait for Vsync should not be used when performing non-DMA host blts.

// If bit(3) is set, only row 0 of the pattern will be used, rather than the usual 8 pattern rows.

typedef uint32_t CommandExtra_t;
namespace CommandExtra
{

    enum
    {
        EnableSourceColorKey =          0x00000001, // 0 Enable source colorkey (1=source colorkeying enabled, 0=source colorkeying disabled)
        EnableDestinationColorKey =     0x00000002, // 1 Enable destination colorkey (1=enable dst colorkeying, 0=disable dst colorkeying)
        WaitForVSync =                  0x00000004, // 2 Wait for Vsync (1=wait for vsync, 0=execute immediately)
        ForcePatternRow0 =              0x00000008  // 3 Force pattern row 0 (1 = use only row 0, 0 = use all 8 pattern rows)
    };

    namespace Shift
    {
        enum
        {
            EnableSourceColorKey = 0x00,
            EnableDestinationColorKey = 0x01,
            WaitForVSync = 0x02,
            ForcePatternRow0 = 0x03
        };
    }
}

}