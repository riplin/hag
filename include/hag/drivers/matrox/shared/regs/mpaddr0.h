//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress0 = 0x1C60,
    MultiPurposeAddress0Commit = 0x1D60
};

}

typedef uint32_t MultiPurposeAddress0_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR0.

namespace MultiPurposeAddress0
{

    enum //Mask
    {
        Value = 0x0003FFFF      // Address register 0. The ar0 field is an 18-bit signed value in two’s complement notation.
                                // For AUTOLINE, this register holds the x end address (in pixels). See the XYEND
                                // register on page 4-77.
                                // - For LINE, it holds 2 x ‘b’.
                                // - For a filled trapezoid, it holds ‘dYl’.
                                // - For a BLIT, ar0 holds the line end source address (in pixels).
                                // - For an ILOAD_SCALE or ILOAD_FILTER, ar0 holds the destination end address
                                // (in pixels) minus one line.
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
