//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{

enum
{
    PixelPLLPA = 0x46,
    PixelPLLPB = 0x4A,
    PixelPLLPC = 0x4E
};

}
    
typedef uint8_t PixelPLLP_t;

// The PValue and SValue fields can be programmed from any of the ‘p’ registers in Set
// A, B, or C: PixelPLLPA, PixelPLLPB, or PixelPLLPC. The register set which
// defines the pixel PLL operation is selected by the clksel field of the MISC VGA
// register as shown in the following table:
//
// clksel   Pixel Clock PLL Frequency   Reset Value
//  00      Register Set A (25.172 MHz) P = 1, S = 0
//  01      Register Set B (28.361 MHz) P = 1, S = 0
//  1X      Register Set C              Unknown
namespace PixelPLLP
{
    enum //Mask
    {
        PValue = 0x7,       // Pixel PLL P value register. The ‘p’ value is used by the VCO clock divider circuit.
                            // The permitted values are:
                            // P = 0 -> Fo = Fvco/1
                            // P = 1 -> Fo = Fvco/2
                            // P = 3 -> Fo = Fvco/4
                            // P = 7 -> Fo = Fvco/8
        SValue = 0x18,      // Pixel PLL S value register. The ‘s’ value controls the loop filter bandwidth.
                            // 50 MHz <= Fvco < 100 MHz S=0
                            // 100 MHz <= Fvco < 140 MHz S=1
                            // 140 MHz <= Fvco < 180 MHz S=2
                            // 180 MHz <= Fvco < 220 MHz S=3
    };

    namespace Shift
    {
        enum
        {

        };
    }
}

}}}}