//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>

namespace Hag::TDfx::Shared::TwoD
{

namespace Register
{
    enum
    {
        SourceColorKeyMin = 0x018,
        SourceColorKeyMax = 0x01c,
        DestinationColorKeyMin = 0x020,
        DestinationColorKeyMax = 0x024
    };
}

typedef uint32_t ColorKey_t;
namespace ColorKey
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