//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        PLLControl2 = 0x48  //Mem clock PLL
    };
}

// Frequency output of PLL’s is given:
// fout = 14.31818 * (N + 2) / (M + 2) / (2 ^ K).

typedef uint32_t PLLControl2_t;
namespace PLLControl2
{
    enum //Mask
    {
        KPostDivider = 0x0003,
        MInputDivider = 0x00fc,
        NMultiplier = 0xff00,
        Test = 0x10000
    };

    namespace Shift
    {
        enum
        {
            KPostDivider = 0x00,
            MInputDivider = 0x02,
            NMultiplier = 0x08,
            Test = 0x10
        };
    }
}

}