//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline HorizontalDisplayEnd_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::HorizontalDisplayEnd);
        return HorizontalDisplayEnd_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, HorizontalDisplayEnd_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::HorizontalDisplayEnd, CRTControllerData_t(value));
    }

    inline uint8_t GetHorizontalDisplayEndLow(HorizontalDisplayEnd_t value)
    {
        return (value & HorizontalDisplayEndLow) >> Shift::HorizontalDisplayEndLow;
    }

    inline HorizontalDisplayEnd_t SetHorizontalDisplayEndLow(uint8_t value)
    {
        return HorizontalDisplayEnd_t((value << Shift::HorizontalDisplayEndLow) & HorizontalDisplayEndLow);
    }
    
}

}}}
