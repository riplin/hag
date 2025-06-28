//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        RasterOperation = 0x030
    };
}

namespace Rop
{
    enum
    {
        Zero = 0x00,
        Nor = 0x11,
        AndI = 0x22,
        NotSource = 0x33,
        AndR = 0x44,
        NotDestination = 0x55,
        Xor = 0x66,
        NotAnd = 0x77,
        And = 0x88,
        ExclusiveNor = 0x99,
        Destination = 0xaa,
        OrI = 0xbb,
        Source = 0xcc,
        OrR = 0xdd,
        Or = 0xee,
        One = 0xff,

        PatternInvert = 0x5a,
        MergeCopy = 0xc0,
        PatternCopy = 0xf0,
        PatternPaint = 0xfb
    };
}

// This is a set of ternary ROPs used to determine how the source, destination, and pattern pixels will be
// combined. The default ROP, ROP0 is stored in the command register. Which of the four ROPs will be
// used is determined on a per-pixel basis, based on the results of the source and destination colorkey tests,
// as shown in the following table:

// Source Color    Destination Color   ROP
// Key Test        Key Test
// -----------------------------------------
// Fail            Fail                ROP 0
// Fail            Pass                ROP 1
// Pass            Fail                ROP 2
// Pass            Pass                ROP 3

typedef uint32_t RasterOperation_t;
namespace RasterOperation
{

    enum
    {
        RasterOperation1 = 0x000000ff,  // 7:0 ROP 1
        RasterOperation2 = 0x0000ff00,  // 15:8 ROP 2
        RasterOperation3 = 0x00ff0000   // 23:16 ROP 3
    };

    namespace Shift
    {
        enum
        {
            RasterOperation1 = 0x00,
            RasterOperation2 = 0x08,
            RasterOperation3 = 0x10
        };
    }

}

}