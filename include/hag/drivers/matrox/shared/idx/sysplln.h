//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    SystemPLLN = 0x2D
};

}

typedef uint8_t SystemPLLN_t;

namespace SystemPLLN
{
    enum //Mask
    {
        NValue = 0x7F               // System PLL N value register. The ‘n’ value is used by the VCO feedback divider circuit.
                                    // Note: The sysplln value must be in the range of 100 (64h) to 127 (7Fh) inclusive.
    };

    namespace Shift
    {
        enum
        {
            NValue = 0x00
        };
    }
}

}