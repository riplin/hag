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
    Extension3 = 0x1f
};

}

typedef uint8_t Extension3_t;

namespace Extension3
{
    enum //Mask
    {
        Value = 0xFF
    };

    inline Extension3_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Extension3);
        return Extension3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Extension3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Extension3, VGA::CRTControllerData_t(value));
    }

}

}