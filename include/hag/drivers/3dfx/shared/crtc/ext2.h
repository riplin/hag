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
    Extension2 = 0x1e
};

}

typedef uint8_t Extension2_t;

namespace Extension2
{
    enum //Mask
    {
        Value = 0xFF
    };

    inline Extension2_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Extension2);
        return Extension2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Extension0_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Extension2, VGA::CRTControllerData_t(value));
    }

}

}