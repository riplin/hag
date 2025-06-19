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
    ExtendedBIOSFlag3 = 0x6B,                               //EBIOS-FLG3 CR6B
};

}

typedef uint8_t ExtendedBIOSFlag3_t;

namespace ExtendedBIOSFlag3
{

    inline ExtendedBIOSFlag3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedBIOSFlag3);
        return ExtendedBIOSFlag3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedBIOSFlag3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedBIOSFlag3, VGA::CRTControllerData_t(value));
    }

}

}
