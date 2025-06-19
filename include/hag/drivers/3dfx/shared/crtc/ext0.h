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
    Extension0 = 0x1c
};

}

typedef uint8_t Extension0_t;

namespace Extension0
{
    enum //Mask
    {
        Value = 0xFF
    };

    inline Extension0_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Extension0);
        return Extension0_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Extension0_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Extension0, VGA::CRTControllerData_t(value));
    }

}

}