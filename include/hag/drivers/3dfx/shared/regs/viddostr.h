//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VideoDesktopOverlayStride = 0xe8
    };
}

typedef uint32_t VideoDesktopOverlayStride_t;
namespace VideoDesktopOverlayStride
{
    enum //Mask
    {
        Value = 0x7fff,
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