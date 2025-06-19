//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VideoDesktopStartAddress = 0xe4
    };
}

typedef uint32_t VideoDesktopStartAddress_t;
namespace VideoDesktopStartAddress
{
    enum //Mask
    {
        Value = 0x00ffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0
        };
    }
}

}