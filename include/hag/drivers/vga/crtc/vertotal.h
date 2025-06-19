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
    VerticalTotal = 0x06,                                   //V_TOTAL CR6
};

}

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

    inline VerticalTotal_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::VerticalTotal);
        return VerticalTotal_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, VerticalTotal_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::VerticalTotal, CRTControllerData_t(value));
    }

}

}
