//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    AdvancedFunctionControl = 0x4AE8,                       //ADVFUNC_CNTL
};

}

typedef uint16_t AdvancedFunctionControl_t;
typedef uint8_t AdvancedFunctionControlLower_t; //Just the lower byte

namespace AdvancedFunctionControl
{

    enum //Mask
    {
        EnableEnhancedFunctions = 0x0001,       //ENB EHFC 0 = Enable VGA and VESA planar (4 bits/pixel) modes
                                                //         1 = Enable all other modes (Enhanced and VESA non-planar)
        ReservedAs1 = 0x0002,                   //Reserved and should be written as 1.
        EnhancedModePixelLength = 0x0004,       //ENH PL   0 = 8 or more bits/pixel enhanced modes
                                                //         1 = 4 bits/pixel enhanced modes
                                                //         Bits 5-4 of CR50 are used to differentiate between 8, 16 and 32 bit pixel lengths.
        EnableLinearAddressing = 0x0010,        //LA       0 = Disable linear addressing
                                                //         1 = Enable linear addressing
                                                //         This bit is ORed with bit 4 of CR58 and is equivalent to it.
        EnableMemoryMappedIO = 0x0020,          //MIO      0 = Disable MMIO
                                                //         1 = Enable MMIO
                                                //         Tis bit is ORed with bit 4 of CR53 and is equivalent to it.
    };

    namespace Shift
    {
        enum
        {
            EnableEnhancedFunctions = 0x00,
            EnhancedModePixelLength = 0x02,
            EnableLinearAddressing = 0x04,
            EnableMemoryMappedIO = 0x05,
        };
    }

    inline AdvancedFunctionControl_t Read()
    {
        return AdvancedFunctionControl_t(SYS_ReadPortShort(Register::AdvancedFunctionControl));
    }
    
    inline void Write(AdvancedFunctionControl_t value)
    {
        SYS_WritePortShort(Register::AdvancedFunctionControl, value);
    }

    inline AdvancedFunctionControlLower_t ReadLower()
    {
        return AdvancedFunctionControlLower_t(SYS_ReadPortByte(Register::AdvancedFunctionControl));
    }
    
    inline void WriteLower(AdvancedFunctionControlLower_t value)
    {
        SYS_WritePortByte(Register::AdvancedFunctionControl, value);
    }

}

}
