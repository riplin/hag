//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VideoScreenSize = 0x98
    };
}

typedef uint32_t VideoScreenSize_t;
namespace VideoScreenSize
{
    enum //Mask
    {
        Width = 0x00000fff,
        Height = 0x00fff000,
    };

    namespace Shift
    {
        enum
        {
            Width = 0x00,
            Height = 0x0c
        };
    }
}

}