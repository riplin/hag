//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

//This register specifies the horizontal row scan position of underline and display buffer addressing modes.

typedef uint8_t UnderlineLocation_t;

namespace UnderlineLocation
{
    enum //Mask
    {
        Location = 0x1F,            //5-bit Value = (scan line count of a character rowan which an underline occurs) -1
        CountBy4Mode = 0x20,        //0 = The memory address counter depends on bit 3 of CR17 (count by 2)
                                    //1 = The memory address counter is incremented every four character clocks
                                    //The CNT BY4 bit is used when double word addresses are used.
        DoubleWordMode = 0x40       //0 = The memory addresses are byte or word addresses
                                    //1 = The memory addresses are doubleword addresses
    };

    namespace Shift
    {
        enum
        {
            Location = 0x00,
            CountBy4Mode = 0x05,
            DoubleWordMode = 0x06
        };
    }

    inline UnderlineLocation_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::UnderlineLocation);
        return UnderlineLocation_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, UnderlineLocation_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::UnderlineLocation, CRTControllerData_t(value));
    }

}

}}}
