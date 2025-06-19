//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::VGA::CRTController
{

namespace Register
{

enum
{
    ScreenOffset = 0x13,                                    //SCREEN-OFFSET CR13
};

}

typedef uint8_t ScreenOffset_t;

namespace ScreenOffset
{
    enum //Mask
    {
        LogicalScreenWidthLow = 0xFF        //10 bit value = quantity that is multiplied by 2 (word mode), 4 (doubleword mode) or 8 (quadword mode)
                                            //       to specify the difference between the starting byte address of two consecutive scan lines.
                                            //       This register contains the least significant 8 bits of this value. The addressing mode is 
                                            //       specified by bit 6 of CR14 and bit 3 of CR17. Setting bit 3 of CR31 to 1 forces doubleword mode.
    };

    namespace Shift
    {
        enum
        {
            LogicalScreenWidthLow = 0x00
        };
    }

    inline ScreenOffset_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::ScreenOffset);
        return ScreenOffset_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ScreenOffset_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::ScreenOffset, CRTControllerData_t(value));
    }

}

}
