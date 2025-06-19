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
    HardwareGraphicsCursorOriginXH = 0x46,                  //HWGC_ORGXH CR46
};

}

typedef uint8_t HardwareGraphicsCursorOriginXH_t;

namespace HardwareGraphicsCursorOriginXH
{

    inline HardwareGraphicsCursorOriginXH_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginXH);
        return HardwareGraphicsCursorOriginXH_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorOriginXH_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginXH, VGA::CRTControllerData_t(value));
    }

};

}
