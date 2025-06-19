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
    HorizontalExtension = 0x1a
};

}

typedef uint8_t HorizontalExtension_t;

namespace HorizontalExtension
{
    enum //Mask
    {
        HorizontalTotal8 = 0x01,
        HorizontalDisplayEnableEnd8 = 0x04,
        HorizontalBlankStart8 = 0x10,
        HorizontalBlankEnd6 = 0x20,
        HorizontalRetraceStart8 = 0x40,
        HorizontalRetraceEnd5 = 0x80
    };

    namespace Shift
    {
        enum
        {
            HorizontalTotal8 = 0x00,
            HorizontalDisplayEnableEnd8 = 0x02,
            HorizontalBlankStart8 = 0x04,
            HorizontalBlankEnd6 = 0x05,
            HorizontalRetraceStart8 = 0x06,
            HorizontalRetraceEnd5 = 0x07
        };
    }

    inline HorizontalExtension_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HorizontalExtension);
        return HorizontalExtension_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, HorizontalExtension_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HorizontalExtension, VGA::CRTControllerData_t(value));
    }

}

}