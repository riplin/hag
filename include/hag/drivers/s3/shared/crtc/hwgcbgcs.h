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
    HardwareGraphicsCursorBackgroundColorStack = 0x4B,      //HWGC_BGSTK CR4B
};

}

typedef uint8_t HardwareGraphicsCursorBackgroundColorStack_t;

namespace HardwareGraphicsCursorBackgroundColorStack
{

    inline HardwareGraphicsCursorBackgroundColorStack_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorBackgroundColorStack);
        return HardwareGraphicsCursorBackgroundColorStack_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister,
                      HardwareGraphicsCursorBackgroundColorStack_t red,
                      HardwareGraphicsCursorBackgroundColorStack_t green,
                      HardwareGraphicsCursorBackgroundColorStack_t blue)
    {
        HardwareGraphicsCursorMode::Read(controllerIndexRegister);
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorBackgroundColorStack, VGA::CRTControllerData_t(red));
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorBackgroundColorStack, VGA::CRTControllerData_t(green));
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorBackgroundColorStack, VGA::CRTControllerData_t(blue));
    }

};

}}}}
