//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

//This register specifies the scan line at which the vertical blanking period begins.
//Bit 8 is bit 3 of CR7. Bit 9 is bit 5 of CR9. Bit 10 is bit 2 of CR5E.

typedef uint8_t StartVerticalBlank_t;

namespace StartVerticalBlank
{
    enum //Mask
    {
        StartVerticalBlankLow = 0xFF    //11-bit value = (scan line count at which ~BLANK becomes active) - 1.
                                        //This register contains the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            StartVerticalBlankLow = 0x00
        };
    }

    inline StartVerticalBlank_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::StartVerticalBlank);
        return StartVerticalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, StartVerticalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::StartVerticalBlank, CRTControllerData_t(value));
    }

    inline uint8_t GetStartVerticalBlankLow(StartVerticalBlank_t value)
    {
        return (value & StartVerticalBlankLow) >> Shift::StartVerticalBlankLow;
    }

    inline StartVerticalBlank_t SetStartVerticalBlankLow(uint8_t value)
    {
        return StartVerticalBlank_t((value << Shift::StartVerticalBlankLow) & StartVerticalBlankLow);
    }

}

}}}
