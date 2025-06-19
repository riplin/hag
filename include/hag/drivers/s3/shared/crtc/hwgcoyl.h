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
    HardwareGraphicsCursorOriginYL = 0x49,                  //HWGC_ORGYL CR49
};

}

typedef uint8_t HardwareGraphicsCursorOriginYL_t;

namespace HardwareGraphicsCursorOriginYL
{

    inline HardwareGraphicsCursorOriginYL_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginYL);
        return HardwareGraphicsCursorOriginYL_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorOriginYL_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginYL, VGA::CRTControllerData_t(value));
    }

};

}
