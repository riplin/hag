//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    ExtendedMiscellaneousControl = 0x65,                    //EXT-MISC-CTL CR65
};

}

typedef uint8_t ExtendedMiscellaneousControl_t;

namespace ExtendedMiscellaneousControl
{

    enum //Mask
    {
        Enable3C3 = 0x04,           //0 = 46EBH is the address for video subsystem setup
                                    //1 = 3C3H is the address for video subsystem setup
                                    //This bit allows the CPU to initialize a second video card.
        DelayBlankByDCLK = 0x18     //(Trio32 only)
                                    //00 = No delay of BLANK
                                    //01 = Delay BLANK for 1 DCLK
                                    //10 = Delay BLANK for 2 DCLKs (required for color mode 12)
                                    //11 = Delay BLANK for 3 DCLKs
    };

    namespace Shift
    {
        enum
        {
            Enable3C3 = 0x01,
            DelayBlankByDCLK = 0x03
        };
    }

    inline ExtendedMiscellaneousControl_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedMiscellaneousControl);
        return ExtendedMiscellaneousControl_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMiscellaneousControl_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedMiscellaneousControl, VGA::CRTControllerData_t(value));
    }

}

}}}}
