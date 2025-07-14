//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress4 = 0x1C70,
    MultiPurposeAddress4Commit = 0x1D70
};

}

typedef uint32_t MultiPurposeAddress4_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR4.

namespace MultiPurposeAddress4
{

    enum //Mask
    {
        Value = 0x0003FFFF      // Address register 4. The ar4 field is an 18-bit signed value in two’s complement notation.
                                // - For TRAP, it holds the error term. Initially:
                                //
                                //      ‘errr’ = [sdxr] ? ‘dXr’ + ‘dYr’ - 1 : -‘dXr’
                                //
                                // - This register is used during AUTOLINE, but doesn’t need to be initialized.
                                // - This register is not used for LINE or BLIT operations without scaling.
                                // - For the ILOAD_SCALE, ILOAD_FILTER, ILOAD_HIQH, and ILOAD_HIQHV, it
                                //   holds the error term, but it doesn't need to be initialized.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x0
        };
    }
}

}
