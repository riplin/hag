//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    PixelPLLStatus = 0x4F
};

}

typedef uint8_t PixelPLLStatus_t;

namespace PixelPLLStatus
{
    enum //Mask
    {
        FrequencyStatus = 0x40,         // Pixel PLL lock status
        FrequencyNotLocked = 0x00,      // 0 = indicates that lock has not yet been achieved
        FrequencyLocked = 0x40          // 1 = indicates that the pixel PLL has locked to the selected frequency defined by
                                        //     Set A, B, or C
    };

    namespace Shift
    {
        enum
        {
            FrequencyStatus = 0x06
        };
    }
}

}