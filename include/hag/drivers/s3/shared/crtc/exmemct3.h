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
    ExtendedMemoryControl3 = 0x60,                          //EXT-MCTL-3 CR60
};

}

typedef uint8_t ExtendedMemoryControl3_t;

namespace ExtendedMemoryControl3
{

    inline ExtendedMemoryControl3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedMemoryControl3);
        return ExtendedMemoryControl3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMemoryControl3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedMemoryControl3, VGA::CRTControllerData_t(value));
    }

}

}
