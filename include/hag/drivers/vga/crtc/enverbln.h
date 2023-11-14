//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline EndVerticalBlank_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::EndVerticalBlank);
        return EndVerticalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, EndVerticalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::EndVerticalBlank, CRTControllerData_t(value));
    }

}

}}}
