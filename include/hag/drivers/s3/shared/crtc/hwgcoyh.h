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
    HardwareGraphicsCursorOriginYH = 0x48,                  //HWGC_ORGYH CR48
};

}

typedef uint8_t HardwareGraphicsCursorOriginYH_t;

namespace HardwareGraphicsCursorOriginYH
{

    inline HardwareGraphicsCursorOriginYH_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginYH);
        return HardwareGraphicsCursorOriginYH_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorOriginYH_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginYH, VGA::CRTControllerData_t(value));
    }

};

}
