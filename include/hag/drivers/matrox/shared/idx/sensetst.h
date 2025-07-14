//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    SenseTest = 0x3A
};

}
    
typedef uint8_t SenseTest_t;

// This register reports the sense comparison function, which determines the
// presence of the CRT monitor and if the termination is correct. The output of
// the comparator is sampled at the end of every active line. When doing a sense
// test, the software should program a uniform color for the entire screen.

namespace SenseTest
{
    enum //Mask
    {
        BlueCompare = 0x01,                 // Sampled blue compare. Verifies that the blue termination is correct.
        BlueUnder350mV = 0x00,              // 0 = blue DAC output is below 350 mV
        BlueOver350mV = 0x01,               // 1 = blue DAC output exceeds 350 mV
        GreenCompare = 0x02,                // Sampled green compare. Verifies that the green termination is correct.
        GreenUnder350mV = 0x00,             // 0 = green DAC output is below 350 mV
        GreenOver350mV = 0x02,              // 1 = green DAC output exceeds 350 mV
        RedCompare = 0x04,                  // Sampled red compare. Verifies that the red termination is correct.
        RedUnder350mV = 0x00,               // 0 = red DAC output is below 350 mV
        RedOver350mV = 0x04,                // 1 = red DAC output exceeds 350 mV
        SenseComparatorPower = 0x80,        // Sense comparator power
        SenseComparatorPowerDown = 0x00,    // 0 = power down
        SenseComparatorPowerUp = 0x80       // 1 = power up
    };

    namespace Shift
    {
        enum
        {
            BlueCompare = 0x00,
            GreenCompare = 0x01,
            RedCompare = 0x02,
            SenseComparatorPower = 0x07
        };
    }
}

}