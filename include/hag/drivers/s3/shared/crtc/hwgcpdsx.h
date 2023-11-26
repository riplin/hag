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
    HardwareGraphicsCursorPatternDisplayStartX = 0x4E,      //HWGC_DX CR4E
};

}

typedef uint8_t HardwareGraphicsCursorPatternDisplayStartX_t;

namespace HardwareGraphicsCursorPatternDisplayStartX
{

    inline HardwareGraphicsCursorPatternDisplayStartX_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorPatternDisplayStartX);
        return HardwareGraphicsCursorPatternDisplayStartX_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorPatternDisplayStartX_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorPatternDisplayStartX, VGA::CRTControllerData_t(value));
    }

};

}}}}
