//Copyright 2023-Present riplin

#pragma once

#include <has/types.h>
#include <has/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    InputStatus0 = 0x3C2,                                   //STATUS_0
};

}

typedef uint8_t InputStatus0_t;

namespace InputStatus0
{

    enum //Mask
    {
        MonitorSenseStatus = 0x10,      //MON SENS 0 = internal ~SENSE signal is logical 0, 1 = internal ~SENSE signal is logical 1
        FeatureInput0 = 0x20,           //Always reads as 1. Writing has no effect
        FeatureInput1 = 0x40,           //Always reads as 1. Writing has no effect
        CRTInterrupt = 0x80,            //0 = Vertical retrace interrupt is cleared
                                        //1 = Vertical retrace interrupt is pending
    };

    namespace Shift
    {
        enum
        {
            MonitorSenseStatus = 0x04,
            FeatureInput0 = 0x05,
            FeatureInput1 = 0x06,
            CRTInterrupt = 0x07,
        };
    };

    inline InputStatus0_t Read()
    {
        return InputStatus0_t(SYS_ReadPortByte(Register::InputStatus0));
    }

}

}
