//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Indexed
{

namespace Register
{

enum
{
    CRCRemainderLow = 0x3C
};

}
    
typedef uint8_t CRCRemainderLow_t;

namespace CRCRemainderLow
{
    enum //Mask
    {
        RemainderLow = 0xFF         // Low-order CRC remainder. This register is used to read the results of the 16-bit CRC
                                    // calculation. CRCRemainderLow corresponds to bits 7:0 of the 16-bit CRC. See
                                    // CRCRemainderHigh.
    };

    namespace Shift
    {
        enum
        {
            RemainderLow = 0x00
        };
    }
}

}}}}