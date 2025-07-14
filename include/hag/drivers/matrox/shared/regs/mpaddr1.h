//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress1 = 0x1C64,
    MultiPurposeAddress1Commit = 0x1D64
};

}

typedef uint32_t MultiPurposeAddress1_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR1.

namespace MultiPurposeAddress1
{

    enum //Mask
    {
        Value = 0x00FFFFFF      // Address register 1. The ar1 field is a 24-bit signed value in two’s complement notation
                                // This register is also loaded when ar3 is accessed.
                                // - For LINE, it holds the error term (initially 2 x ‘b’ - ‘a’ -[sdy]).
                                // - This register does not need to be loaded for AUTOLINE.
                                // - For a filled trapezoid, it holds the error term in two’s complement notation; initially:
                                //      ‘errl’ = [sdxl] ? ‘dXl’ + ‘dYl’ - 1 : -‘dXl’
                                // - For a BLIT, ar1 holds the line start source address (in pixels). Because the start
                                //   source address is also required by ar3, and because ar1 is loaded when writing ar3
                                //   this register doesn’t need to be explicitly initialized.
                                // - In the ILOAD_SCALE and ILOAD_FILTER algorithms, ar1 contains the destina-
                                //   tion starting address (in pixels) minus one line. Because the same value is also
                                //   required by ar3 and because ar1 is loaded when writing ar3, this register doesn't
                                //   need to be explicitly initialized.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }
}

}
