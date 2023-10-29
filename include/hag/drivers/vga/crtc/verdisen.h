//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t VerticalDisplayEnd_t;

namespace VerticalDisplayEnd
{
    enum //Mask
    {
        VerticalDisplayEndLow = 0xFF        //11 bit Value = (number of scan line of active display) - 1. This register contains
                                            //       the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            VerticalDisplayEndLow = 0x00
        };
    }

    inline VerticalDisplayEnd_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::VerticalDisplayEnd);
        return VerticalDisplayEnd_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, VerticalDisplayEnd_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::VerticalDisplayEnd, CRTControllerData_t(value));
    }

    inline uint8_t GetVerticalDisplayEndLow(VerticalDisplayEnd_t value)
    {
        return (value & VerticalDisplayEndLow) >> Shift::VerticalDisplayEndLow;
    }

    inline VerticalDisplayEnd_t SetVerticalDisplayEndLow(uint8_t value)
    {
        return (value << Shift::VerticalDisplayEndLow) & VerticalDisplayEndLow;
    }

}

}}}
