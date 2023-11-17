//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t HorizontalTotal_t;

namespace HorizontalTotal
{

    inline HorizontalTotal_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::HorizontalTotal);
        return HorizontalTotal_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, HorizontalTotal_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::HorizontalTotal, CRTControllerData_t(value));
    }

}

}}}
