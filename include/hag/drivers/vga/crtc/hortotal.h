//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t HorizontalTotal_t;

namespace HorizontalTotal
{
    enum //Mask
    {
        HorizontalTotalLow = 0xFF       //9 bit value = (number of character clocks in one scan line) - 5.
                                        //This register contains the least significan 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            HorizontalTotalLow = 0x00
        };
    }

    inline HorizontalTotal_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::HorizontalTotal);
        return HorizontalTotal_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, HorizontalTotal_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::HorizontalTotal, CRTControllerData_t(value));
        }

    inline uint8_t GetHorizontalTotalLow(HorizontalTotal_t value)
    {
        return (value & HorizontalTotalLow) >> Shift::HorizontalTotalLow;
    }

    inline HorizontalTotal_t SetHorizontalTotalLow(uint8_t value)
    {
        return HorizontalTotal_t((value << Shift::HorizontalTotalLow) & HorizontalTotalLow);
    }

}

}}}
