//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedRAMDACControl_t;

namespace ExtendedRAMDACControl
{
    
    enum //Mask
    {
        EnableGeneralInputPortRead = 0x04,  // 0 = RAMDAC reads enabled
                                            // 1 = General Input Port read enabled
        HardwareCursorMode = 0x10,          // 0 = MS-Windows mode (Default)
                                            // 1 = X11-Windows mode
    };

    namespace Shift
    {
        enum
        {
            EnableGeneralInputPortRead = 0x02,
            HardwareCursorMode = 0x04,
        };
    }

    inline ExtendedRAMDACControl_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedRAMDACControl);
        return ExtendedRAMDACControl_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedRAMDACControl_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedRAMDACControl, VGA::CRTControllerData_t(value));
    }

}

}}}
