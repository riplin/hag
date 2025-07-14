//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    Source0 = 0x1C30,
    Source1 = 0x1C34,
    Source2 = 0x1C38,
    Source3 = 0x1C3C,
    Source0Commit = 0x1D30,
    Source1Commit = 0x1D34,
    Source2Commit = 0x1D38,
    Source3Commit = 0x1D3C
};

}

typedef uint32_t Source_t;

namespace Source
{

    enum //Mask
    {
        Value = 0xFFFFFFFF          // Source register. The source register is used as source data for all drawing operations.
                                    // For LINE with the RPL or RSTR attribute, the source register is used to store the line
                                    // style. The funcnt field of the SHIFT register points to the selected source register bit
                                    // being used as the linestyle for the current pixel. Refer to Section 5.5.4.3 on page 5-30
                                    // for more details.
                                    // For TRAP with the RPL or RSTR attribute, the source register is used to store an 8 × 8
                                    // pattern (the odd bytes of the SRC registers must be a copy of the even bytes). Refer to
                                    // Section 5.5.5.3 on page 5-36 for more details.
                                    // For all BLIT operations, and for TRAP or LINE using depth mode, the source register
                                    // is used internally for intermediate data.
                                    // A write to the PAT registers (see page 4-67) will load the SRC registers.
                                    //
                                    //    Source3     Source2     Source1     Source0
                                    // 127.......96 95.......64 63.......32 31.......0
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
