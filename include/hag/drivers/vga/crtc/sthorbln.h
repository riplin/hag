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
    StartHorizontalBlank = 0x02,                            //S_H_BLNK CR2
};

}

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

    inline StartHorizontalBlank_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::StartHorizontalBlank);
        return StartHorizontalBlank_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, StartHorizontalBlank_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::StartHorizontalBlank, CRTControllerData_t(value));
    }

}

}
