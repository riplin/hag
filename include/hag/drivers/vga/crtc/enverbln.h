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
    EndVerticalBlank = 0x16,                                //EVB CR16
};

}

//This register specifies the scan line count value when the vertical blank period ends.

typedef uint8_t EndVerticalBlank_t;

namespace EndVerticalBlank
{
    enum //Mask
    {
        EndVerticalBlankLow = 0xFF, //Value = least significant 8 bits of the scan line counter 
                                    //        value at which vertical blank- ing ends. To obtain
                                    //        this value. add the desired width of the vertical
                                    //        blanking pulse in scan lines to [(value in the 
                                    //        Start Vertical Blank registerl-11. also in scan lines.
                                    //        The 8 least significant bits of this sum are programmed
                                    //        into this field. This allows a maxi- mum vertical
                                    //        blanking pulse of 63 scan line units.
    };

    namespace Shift
    {
        enum
        {
            EndVerticalBlankLow = 0x00,
        };
    }

    inline EndVerticalBlank_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::EndVerticalBlank);
        return EndVerticalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, EndVerticalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::EndVerticalBlank, CRTControllerData_t(value));
    }

}

}
