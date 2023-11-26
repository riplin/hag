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
    ExtendedBIOSFlag1 = 0x52,                               //EXT_BBFLG1 CR52
};

}

typedef uint8_t ExtendedBIOSFlag1_t;

namespace ExtendedBIOSFlag1
{

    inline ExtendedBIOSFlag1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedBIOSFlag1);
        return ExtendedBIOSFlag1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedBIOSFlag1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedBIOSFlag1, VGA::CRTControllerData_t(value));
    }

}

}}}}
