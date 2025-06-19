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
    StartVerticalBlank = 0x15,                              //SVB CR15
};

}

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

    inline StartVerticalBlank_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::StartVerticalBlank);
        return StartVerticalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, StartVerticalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::StartVerticalBlank, CRTControllerData_t(value));
    }

}

}
