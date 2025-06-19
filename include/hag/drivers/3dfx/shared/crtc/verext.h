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
    VerticalExtension = 0x1b
};

}

typedef uint8_t VerticalExtension_t;

namespace VerticalExtension
{
    enum //Mask
    {
        VerticalTotal10 = 0x01,
        VerticalDisplayEnableEnd10 = 0x04,
        VerticalBlankStart10 = 0x10,
        VerticalRetraceStart10 = 0x40
    };

    namespace Shift
    {
        enum
        {
            VerticalTotal10 = 0x00,
            VerticalDisplayEnableEnd10 = 0x02,
            VerticalBlankStart10 = 0x04,
            VerticalRetraceStart10 = 0x06
        };
    }

    inline VerticalExtension_t Read(Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::VerticalExtension);
        return VerticalExtension_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, VerticalExtension_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::VerticalExtension, VGA::CRTControllerData_t(value));
    }

}

}