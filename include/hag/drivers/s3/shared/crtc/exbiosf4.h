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
    ExtendedBIOSFlag4 = 0x6C,                               //EBIOS-FLG4 CR6C
};

}

typedef uint8_t ExtendedBIOSFlag4_t;

namespace ExtendedBIOSFlag4
{

    inline ExtendedBIOSFlag4_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedBIOSFlag4);
        return ExtendedBIOSFlag4_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedBIOSFlag4_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedBIOSFlag4, VGA::CRTControllerData_t(value));
    }

}

}
