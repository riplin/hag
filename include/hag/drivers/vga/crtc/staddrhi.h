//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t StartAddressHigh_t;

namespace StartAddressHigh
{
    enum //Mask
    {
        DisplayStartAddressHigh = 0xFF      //20 bit Value = the first address after a vertical retrace at which the display on the screen begins on each screen refresh.
                                            //These along with bits 3-0 of CR69 are the high order start address bits.
    };

    namespace Shift
    {
        enum
        {
            DisplayStartAddressHigh = 0x00
        };
    }

    inline StartAddressHigh_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::StartAddressHigh);
        return StartAddressHigh_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, StartAddressHigh_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::StartAddressHigh, CRTControllerData_t(value));
    }

    inline uint8_t GetDisplayStartAddressHigh(StartAddressHigh_t value)
    {
        return (value & DisplayStartAddressHigh) >> Shift::DisplayStartAddressHigh;
    }
    
    inline StartAddressHigh_t SetDisplayStartAddressHigh(uint8_t value)
    {
        return StartAddressHigh_t((value << Shift::DisplayStartAddressHigh) & DisplayStartAddressHigh);
    }

}

}}}
