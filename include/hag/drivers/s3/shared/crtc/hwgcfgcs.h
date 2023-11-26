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
    HardwareGraphicsCursorForegroundColorStack = 0x4A,      //HWGC_FGSTK CR4A
};

}

typedef uint8_t HardwareGraphicsCursorForegroundColorStack_t;

namespace HardwareGraphicsCursorForegroundColorStack
{

    inline HardwareGraphicsCursorForegroundColorStack_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorForegroundColorStack);
        return HardwareGraphicsCursorForegroundColorStack_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister,
                      HardwareGraphicsCursorForegroundColorStack_t red,
                      HardwareGraphicsCursorForegroundColorStack_t green,
                      HardwareGraphicsCursorForegroundColorStack_t blue)
    {
        HardwareGraphicsCursorMode::Read(controllerIndexRegister);
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorForegroundColorStack, VGA::CRTControllerData_t(red));
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorForegroundColorStack, VGA::CRTControllerData_t(green));
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorForegroundColorStack, VGA::CRTControllerData_t(blue));
    }

};

}}}}
