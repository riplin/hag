//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    CRCRemainderHigh = 0x3D
};

}
    
typedef uint8_t CRCRemainderHigh_t;

// A 16-bit cyclic redundancy check (CRC) is provided so that the video data’s integrity
// can be verified at the input of the DACs. The CRCBitSelect register indicates which
// video line is checked. The CRCRemainderHigh and CRCRemainderLow registers accept video data
// when the screen is not in the blank period. The CRC Remainder register is reset to 0 at
// the end of vertical sync period and must be read at the beginning of the next vertical
// sync period (when VSYNC status goes to 1).

namespace CRCRemainderHigh
{
    enum //Mask
    {
        RemainderHigh = 0xFF            // High-order CRC remainder. This register is used to read the results of the 16-bit CRC
                                        // calculation. CRCRemainderHigh corresponds to bits 15:8 of the 16-bit CRC.
    };

    namespace Shift
    {
        enum
        {
            RemainderHigh = 0x00
        };
    }
}

}