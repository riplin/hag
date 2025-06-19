//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MultiPurposeAddress3 = 0x1C6C,
    MultiPurposeAddress3Commit = 0x1D6C
};

}

typedef uint32_t MultiPurposeAddress3_t;

// Note: Writing to this register when the DWGCTL register’s arzero bit = 1 will produce
// unpredictable results. Make sure that a ‘0’ has been written to arzero prior to
// accessing AR3.

namespace MultiPurposeAddress3
{

    enum //Mask
    {
        Value = 0x00FFFFFF,     // Address register 3. The ar3 field is a 24-bit signed value in two’s complement notation
                                // or a 24-bit unsigned value.
                                // - This register is used during AUTOLINE, but does not need to be initialized.
                                // - This register is not used for LINE without auto initialization, nor is it used by TRAP.
                                // - In the two-operand Blit algorithms and ILOAD ar3 contains the source current
                                //   address (in pixels). This value must be initialized as the starting address for a Blit.
                                //   The source current address is always linear.
                                // - In the ILOAD_SCALE and ILOAD_FILTER algorithms, ar3 contains the destination
                                //   current address (in pixels) minus one line. This value must be initialized as the
                                //   destination starting address minus one line.
        Extension = 0x03000000  // These two bits are used as an extension to ar3 in order to generate a 26-bit source or
                                // pattern address (in pixels). They are not modified by ALU operations.
                                // In BLIT operations, the spage field is only used with monochrome source data.
                                // The spage field is not used for TRAP, LINE or AUTOLINE operations.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Extension = 0x19
        };
    }
}

}
