//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress5 = 0x1C74,
    MultiPurposeAddress5Commit = 0x1D74
};

}

typedef uint32_t MultiPurposeAddress5_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR5.

namespace MultiPurposeAddress5
{

    enum //Mask
    {
        Value = 0x0003FFFF      // Address register 5. The ar5 field is an 18-bit signed value in two’s complement notation.
                                // - At the beginning of AUTOLINE, ar5 holds the x start address (in pixels). See the
                                //   XYSTRT register on page 4-78. At the end of AUTOLINE the register is loaded
                                //   with the x end, so it is not necessary to reload the register when drawing a polyline.
                                // - This register is not used for LINE without auto initialization.
                                // - For TRAP, it holds the minor axis increment (-|dXr|).
                                // - In BLIT algorithms, ar5 holds the pitch (in pixels) of the source operand. A negative
                                //   pitch value specifies that the source is scanned from bottom to top while a positive
                                //   pitch value specifies a top to bottom scan.
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
