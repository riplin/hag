//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/hwgcmode.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    HardwareGraphicsCursorPatternDisplayStartY = 0x4F,      //HWGC_DY CR4F
};

}

typedef uint8_t HardwareGraphicsCursorPatternDisplayStartY_t;

namespace HardwareGraphicsCursorPatternDisplayStartY
{

    inline HardwareGraphicsCursorPatternDisplayStartY_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorPatternDisplayStartY);
        return HardwareGraphicsCursorPatternDisplayStartY_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorPatternDisplayStartY_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorPatternDisplayStartY, VGA::CRTControllerData_t(value));
    }

};

}}}}
