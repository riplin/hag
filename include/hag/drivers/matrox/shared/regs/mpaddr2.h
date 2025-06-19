//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress2 = 0x1C68,
    MultiPurposeAddress2Commit = 0x1D68
};

}

typedef uint32_t MultiPurposeAddress2_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR2.

namespace MultiPurposeAddress2
{

    enum //Mask
    {
        Value = 0x0003FFFF      // Address register 2. The ar2 field is an 18-bit signed value in two’s complement notation.
                                // - For AUTOLINE, this register holds the y end address (in pixels). See the XYEND
                                //   register on page 4-77.
                                // - For LINE, it holds the minor axis error increment (initially 2 x ‘b’ - 2 x ‘a’).
                                // - For a filled trapezoid, it holds the minor axis increment (-|dXl|).
                                // - For ILOAD_SCALE, it holds the error increment which is the source dimension for
                                //   the x axis. (dXsrc)
                                // - For ILOAD_FILTER, it holds the error increment which is the source dimension
                                //   after the filter process for the x axis. (2 * dXsrc - 1)
                                // - For ILOAD_HIQH and ILOAD_HIQHV, it holds:
                                //
                                //   (SRC_X_DIMEN - 1) << 16 
                                //   -----------------------   + 1
                                //       (DST_X_DIMEN - 1)
                                //
                                // This register is not used for BLIT operations without scaling.
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
