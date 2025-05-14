//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{

enum
{
    PixelPLLNA = 0x45,
    PixelPLLNB = 0x49,
    PixelPLLNC = 0x4D
};

}
    
typedef uint8_t PixelPLLN_t;

// The NValue field can be programmed from any of the ‘n’ registers in Set A, B, or C:
// PixelPLLNA, PixelPLLNB, or PixelPLLNC. The register set which defines the pixel
// PLL operation is selected by the clksel field of the MISC VGA register as shown in
// the following table:
//
// clksel   Pixel Clock PLL Frequency   Reset Value
//  00      Register Set A (25.172 MHz) N = 108
//  01      Register Set B (28.361 MHz) N = 102
//  1X      Register Set C              Unknown
//
// Note: The pixplln value must be in the range of 100 (64h) to 127 (7Fh) inclusive.

namespace PixelPLLN
{
    enum //Mask
    {
        NValue = 0x7F       //Pixel PLL N value register. The ‘n’ value is used by the VCO feedback divider circuit.
    };

    namespace Shift
    {
        enum
        {
            NValue = 0x00
        };
    }
}

}}}}