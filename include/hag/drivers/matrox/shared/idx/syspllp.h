//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{
    
enum
{
    SystemPLLP = 0x2E
};

}

typedef uint8_t SystemPLLP_t;

namespace SystemPLLP
{
    enum //Mask
    {
        PValue = 0x07,          // System PLL P value register. The ‘p’ value is used by the VCO post-divider circuit.
                                // The permitted values are:
                                // P=0 -> Fo = Fvco/1
                                // P=1 -> Fo = Fvco/2
                                // P=3 -> Fo = Fvco/4
                                // P=7 -> Fo = Fvco/8
                                // Other values are reserved.
        SValue = 0x18,          // System PLL S value register. The ‘s’ value controls the loop filter bandwidth.
                                // 50 MHz <= Fvco < 100 MHz -> S=0
                                // 100 MHz <= Fvco < 140 MHz -> S=1
                                // 140 MHz <= Fvco < 180 MHz -> S=2
                                // 180 MHz <= Fvco < 220 MHz -> S=3
    };

    namespace Shift
    {
        enum
        {
            PValue = 0x00,
            SValue = 0x03
        };
    }
}

}