//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::TDfx::Shared::CRTController
{

namespace Register
{

enum
{
    Extension1 = 0x1d
};

}

typedef uint8_t Extension1_t;

namespace Extension1
{
    enum //Mask
    {
        Value = 0xFF
    };

    inline Extension1_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Extension1);
        return Extension1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Extension1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Extension1, VGA::CRTControllerData_t(value));
    }

}

}