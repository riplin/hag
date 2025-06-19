//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        PLLControl1 = 0x44  //GRX clock PLL
    };
}

// Frequency output of PLL’s is given:
// fout = 14.31818 * (N + 2) / (M + 2) / (2 ^ K).

typedef uint32_t PLLControl1_t;
namespace PLLControl1
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