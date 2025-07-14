//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    MemoryPitch = 0x1C8C,
    MemoryPitchCommit = 0x1D8C
};

}

typedef uint32_t MemoryPitch_t;

namespace MemoryPitch
{

    enum //Mask
    {
        YIncrement = 0x00000FFF,        // The y increment. This field is a 12-bit unsigned value. The y increment value is a pixel
                                        // unit which must be a multiple of 32 (the five LSB = 0) and must be less than or equal
                                        // to 2048. The iy field specifies the increment to be added to or subtracted from ydst
                                        // (see YDST on page 4-80) between two destination lines. The iy field is also used as
                                        // the multiplicator factor for linearizing the ydst register.
                                        // Note that only a few values are supported for linearization. If the pitch selected can’t
                                        // be linearized, the ylin bit should be used to disable the linearization operation. The
                                        // following table provides the supported pitches for linearization:
                                        // Pitch        YIncrement
                                        // 512          001000000000b
                                        // 640          001010000000b
                                        // 768          001100000000b
                                        // 800          001100100000b
                                        // 832          001101000000b
                                        // 960          001111000000b
                                        // 1024         010000000000b
                                        // 1152         010010000000b
                                        // 1280         010100000000b
                                        // 1600         011001000000b
                                        // 1664         011010000000b
                                        // 1920         011110000000b
                                        // 2048         100000000000b
        YNoLinearization = 0x00008000,  // The y linearization. This bit specifies whether the address must be linearized or not.
                                        // 0 = The address is an xy address, so it must be linearized by the hardware
                                        // 1 = The address is already linear
    };

    namespace Shift
    {
        enum
        {
            YIncrement = 0x00,
            YNoLinearization = 0x0F
        };
    }
}

}
