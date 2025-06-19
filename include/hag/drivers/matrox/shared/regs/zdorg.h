//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    ZDepthOrigin = 0x1C0C,
    ZDepthOriginCommit = 0x1D0C
};

}

typedef uint32_t ZDepthOrigin_t;

namespace ZDepthOrigin
{

    enum //Mask
    {
        Origin = 0x007FFFFF     // Z-depth origin. The zorg field is a 23-bit unsigned value which provides an offset
                                // value (the base address) in order to position the first pixel in the z-depth buffer.
                                // The zorg field corresponds to a byte address in memory. This register must be set so
                                // that there is no overlap with the frame buffer.
                                // This field must be loaded with a multiple of 512 (the nine LSBs = 0).
                                // zorg = ydstorg + 2048 + n * 4096,
                                // where n is any integer that does not cause an overlap between the intensity and depth
                                // buffers.
    };

    namespace Shift
    {
        enum
        {
            Origin = 0x00
        };
    }
}

}
