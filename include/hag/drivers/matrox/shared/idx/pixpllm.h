//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{

enum
{
    PixelPLLMA = 0x44,
    PixelPLLMB = 0x48,
    PixelPLLMC = 0x4C
};

}
    
typedef uint8_t PixelPLLM_t;

// There are three sets of PIXPLL registers:
//
//  Set A       Set B       Set C
//  PixelPLLMA  PixelPLLMB  PixelPLLMC
//  PixelPLLNA  PixelPLLNB  PixelPLLNC
//  PixelPLLPA  PixelPLLPB  PixelPLLPC
//
// The MValue field can be programmed from any of the ‘m’ registers in Set A, B, or C:
// PixelPLLMA, PixelPLLMB, or PixelPLLMC. The register set which defines the
// pixel PLL operation is selected by the clksel field of the MISC VGA register as
// shown in the following table:
// 
// clksel   Pixel Clock PLL Frequency   Reset Value
//   00     Register Set A (25.172 MHz) M = 30
//   01     Register Set B (28.361 MHz) M = 25
//   1X     Register Set C              Unknown
//
namespace PixelPLLM
{
    enum //Mask
    {
        MValue = 0x1F           // Pixel PLL M value register. The ‘m’ value is used by the reference clock prescaler circuit.  
    };

    namespace Shift
    {
        enum
        {
            MValue = 0x00
        };
    }
}

}}}}