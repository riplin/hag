//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::VGA::CRTController
{

namespace Register
{

enum
{
    HorizontalTotal = 0x00,                                 //H_TOTAL CR0
};

}

typedef uint8_t HorizontalTotal_t;

namespace HorizontalTotal
{

    inline HorizontalTotal_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::HorizontalTotal);
        return HorizontalTotal_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, HorizontalTotal_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::HorizontalTotal, CRTControllerData_t(value));
    }

}

}
