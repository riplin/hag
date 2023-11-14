//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t StartHorizontalBlank_t;

namespace StartHorizontalBlank
{
    enum //Mask
    {
        StartHorizontalBlankLow = 0xFF  //9 bit value = (number of character clocks of active display) - 1.
                                        //This register contains the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            StartHorizontalBlankLow = 0x00
        };
    }

    inline StartHorizontalBlank_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::StartHorizontalBlank);
        return StartHorizontalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(Register_t controllerIndexRegister, StartHorizontalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::StartHorizontalBlank, CRTControllerData_t(value));
    }

}

}}}
