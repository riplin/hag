//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    SystemPLLM = 0x2C
};

}

typedef uint8_t SystemPLLM_t;

namespace SystemPLLM
{
    enum //Mask
    {
        MValue = 0x1F       //System PLL M value register. The ‘m’ value is used by the reference clock prescaler circuit.
    };

    namespace Shift
    {
        enum
        {
            MValue = 0x00
        };
    }
}

}