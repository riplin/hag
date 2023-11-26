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
    HardwareGraphicsCursorMode = 0x45,                      //HGC_MODE CR45
};

}

typedef uint8_t HardwareGraphicsCursorMode_t;

namespace HardwareGraphicsCursorMode
{

    enum //Mask
    {
        Enable = 0x01,          //0 = Hardware graphics cursor disabled in any mode
                                //1 = Hardware graphics cursor enabled in Enhanced mode
        RightStorage = 0x10     //0 = Function disabled
                                //1 = For 4 bits/pixel, the last 256 bytes in each 1-KByte line of the hardware cursor start
                                //    address become the hardware graphics cursor storage area. For 8 bits/pixel, the last
                                //    512 bytes in each 2-KByte line of the hardware cursor start address become the hardware
                                //    graphics cursor storage area. In either case, bits 1-0 of CR4D must be 11b.
    };

    namespace Shift
    {
        enum
        {
            Enable = 0x01,
            RightStorage = 0x04
        };
    }

    inline HardwareGraphicsCursorMode_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorMode);
        return HardwareGraphicsCursorMode_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorMode_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorMode, VGA::CRTControllerData_t(value));
    }

};

}}}}
