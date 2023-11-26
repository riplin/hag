//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    HardwareGraphicsCursorOriginXL = 0x47,                  //HWGC_ORGXL CR47
};

}

typedef uint8_t HardwareGraphicsCursorOriginXL_t;

namespace HardwareGraphicsCursorOriginXL
{

    inline HardwareGraphicsCursorOriginXL_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginXL);
        return HardwareGraphicsCursorOriginXL_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorOriginXL_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorOriginXL, VGA::CRTControllerData_t(value));
    }

};

}}}}
