//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t VerticalTotal_t;

namespace VerticalTotal
{
    enum //Mask
    {
        VerticalTotalLow = 0xFF     //11 bit value = (number of scan lines from VSYNC active to the next VSYNC active) - 2.
                                    //This register contains the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            VerticalTotalLow = 0x00
        };
    }

    inline VerticalTotal_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::VerticalTotal);
        return VerticalTotal_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, VerticalTotal_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::VerticalTotal, CRTControllerData_t(value));
    }

    inline uint8_t GetVerticalTotalLow(VerticalTotal_t value)
    {
        return (value & VerticalTotalLow) >> Shift::VerticalTotalLow;
    }

    inline VerticalTotal_t SetVerticalTotalLow(uint8_t value)
    {
        return VerticalTotal_t((value << Shift::VerticalTotalLow) & VerticalTotalLow);
    }

}

}}}
