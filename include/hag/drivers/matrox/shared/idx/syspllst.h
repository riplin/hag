//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    SystemPLLStatus = 0x2F
};

}

typedef uint8_t SystemPLLStatus_t;

namespace SystemPLLStatus
{
    enum //Mask
    {
        FrequencyStatus = 0x40,         // System PLL lock status.
        FrequencyNotLocked = 0x00,      // 0 = indicates that lock has not yet been achieved
        FrequencyLocked = 0x40          // 1 = indicates that the system PLL has locked to the selected frequency
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