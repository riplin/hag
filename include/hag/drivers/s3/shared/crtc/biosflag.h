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
    BIOSFlag = 0x41,                                        //BIOS_FLAG CR41
};

}

typedef uint8_t BIOSFlag_t;

namespace BIOSFlag
{

    inline BIOSFlag_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::BIOSFlag);
        return BIOSFlag_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, BIOSFlag_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::BIOSFlag, VGA::CRTControllerData_t(value));
    }

}

}
