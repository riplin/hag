//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared
{

namespace Register
{
    
enum
{
    FunnelShifterControl = 0x1C50,
    FunnelShifterControlCommit = 0x1D50
};

}

typedef uint32_t FunnelShifterControl_t;

namespace FunnelShifterControl
{

    enum //Mask
    {
        FunnelCountValue = 0x0000007F,      // Funnel count value. This field is used to drive the funnel shifter bit selection.
                                            // - For LINE operations, this is a countdown register. For 3D vectors, this field must be
                                            //   initialized to 0.
                                            // This field will be modified during Blit operations.
        FunnelShifterOffset = 0x003F0000,   // Funnel shifter offset. For Blit operations, this field is used to specify a bit offset in the
                                            // funnel shifter count. In this case funoff is interpreted as a 6-bit signed value.
        PatternXOffset = 0x0000000F,        // Pattern x offset. This field is used for TRAP operations without depth, to specify the x
                                            // offset in the pattern. This offset must be in the range 0-7 (bit 3 is always 0).
                                            // This field will be modified during Blit operations.
        PatternYOffset = 0x0000007F,        // Pattern y offset. This field is used for TRAP operations without depth, to specify the y
                                            // offset in the pattern.
                                            // This field will be modified during Blit operations.
        LineStyleLength = 0x007F0000        // Line style length. For LINE operations, this field specifies the linestyle length. It indi-
                                            // cates a location in the SRC registers (see page 4-73), so its value is the number of bits
                                            // in the complete pattern minus one. For 3D vectors, this field must be initialized to 0.
        
    };

    namespace Shift
    {
        enum
        {
            FunnelCountValue = 0x00,
            FunnelShifterOffset = 0x10,
            PatternXOffset = 00,
            PatternYOffset = 0x04,
            LineStyleLength = 0x10
        };
    }
}

}
