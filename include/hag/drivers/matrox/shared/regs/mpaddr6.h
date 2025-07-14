//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress6 = 0x1C78,
    MultiPurposeAddress6Commit = 0x1D78
};

}

typedef uint32_t MultiPurposeAddress6_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
//accessing AR6.

namespace MultiPurposeAddress6
{

    enum //Mask
    {
        Value = 0x0003FFFF      // Address register 6. This field is an 18-bit signed value in two’s complement notation.
                                // It is sign extended to 24 bits before being used by the ALU.
                                // - At the beginning of AUTOLINE, ar6 holds the y start address (in pixels). See the
                                //   XYSTRT register on page 4-78. During AUTOLINE processing, this register is
                                //   loaded with the signed y displacement. At the end of AUTOLINE the register is
                                //   loaded with the y end, so it is not necessary to reload the register when drawing a
                                //   polyline.
                                // - This register is not used for LINE without auto initialization.
                                // - For TRAP, it holds the major axis increment (‘dYr’).
                                // - For ILOAD_SCALE, it holds the error increment which is the source dimension (in
                                //   pixels) minus the destination dimension for the x axis. (dXsrc - dXdst)
                                // - For ILOAD_FILTER, it holds the error increment which is the source dimension (in
                                //   pixels) minus the destination dimension for the x axis. (2 * dXsrc - 1 - dXdst)
                                // -> For ILOAD_SCALE and ILOAD_FILTER, ar6 must be less than or equal to
                                //    zero.
                                // - For ILOAD_HIQH and ILOAD_HIQHV, it holds:
                                //
                                //     (SRC_X_DIMEN - DST_X_DIMEN) << 16
                                //     ---------------------------------
                                //             (DST_X_DIMEN - 1)
                                //
                                // This register is not used for BLIT (without scaling) or IDUMP operations.
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
