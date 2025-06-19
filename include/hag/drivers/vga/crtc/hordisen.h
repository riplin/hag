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
    HorizontalDisplayEnd = 0x01,                            //H_D_END CR1
};

}

typedef uint8_t HorizontalDisplayEnd_t;

namespace HorizontalDisplayEnd
{
    enum //Mask
    {
        HorizontalDisplayEndLow = 0xFF  //9 bit value = (number of character clocks of active display) - 1.
                                        //This register contains the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            HorizontalDisplayEndLow = 0x00
        };
    }

    inline HorizontalDisplayEnd_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::HorizontalDisplayEnd);
        return HorizontalDisplayEnd_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, HorizontalDisplayEnd_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::HorizontalDisplayEnd, CRTControllerData_t(value));
    }

}

}
