//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    CRCBitSelect = 0x3E
};

}
    
typedef uint8_t CRCBitSelect_t;

namespace CRCBitSelect
{
    enum //Mask
    {
        BitSelect = 0x1F        // CRC bit selection. This register determines which of the 24 DAC data lines the 16-bit
                                // CRC should be calculated on. Valid values are 0h-17h:
                                // Value    DAC Data line to use
                                // 00h-07h  blue0 - blue7
                                // 08h-0Fh  green0 - green7
                                // 10h-17h  red0 - red7
    };

    namespace Shift
    {
        enum
        {
            BitSelect = 0x00
        };
    }
}

}