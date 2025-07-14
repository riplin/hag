//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    CursorBaseAddressLow = 0x04
};

}

typedef uint8_t CursorBaseAddressLow_t;

namespace CursorBaseAddressLow
{
    enum //Mask
    {
        AddressLow = 0xFF           // Cursor address low. These are the low-order bits of the cursor map address. See the
                                    // CursorBaseAddressHigh register description for more details.
    };

    namespace Shift
    {
        enum
        {
            AddressLow = 0x00
        };
    }
}

}