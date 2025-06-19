//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    ExtendedMode = 0x43,                                    //EXT_MODE CR43
};

}

typedef uint8_t ExtendedMode_t;

namespace ExtendedMode
{

    enum //Mask
    {
        UnknownMask = 0x03,
        LogicalScreenWidthHigh = 0x04,      //OLD LSW8 This is an extension of the Offset (Screen Width) register (CR13).
                                            //         This is disabled if bits 5-4 of the ExtendedSystemControl2 register (CR51) are not 00b.
        HorizontalCounterDoubleMode = 0x80  //HCTR X2 0 = Disable horizontal counter double mode, 1 = Enable horizontal counter double mode
                                            //        (horizontal CRT parameters are doubled)
    };

    namespace Shift
    {
        enum
        {
            LogicalScreenWidthHigh = 0x02,
            HorizontalCounterDoubleMode = 0x07
        };
    }

    inline ExtendedMode_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedMode);
        return ExtendedMode_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMode_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedMode, VGA::CRTControllerData_t(value));
    }

}

}
