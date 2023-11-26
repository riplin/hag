//Copyright 2023-Present riplin

#pragma once

#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/crtc/regtype.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    CRTControllerIndexB = 0x3B4,                            //CRX //Mono
    CRTControllerIndexD = 0x3D4,                            //CRX //Color
};

}

namespace CRTControllerIndex
{
    enum //Mask
    {
        CRTCRegisterIndex = 0xFF   //CRTC ADDRESS
    };

    namespace Shift
    {
        enum
        {
            CRTCRegisterIndex = 0x00
        };
    }

    inline CRTController::Register_t Read(VGA::Register_t controllerIndexRegister)
    {
        return CRTController::Register_t(SYS_ReadPortByte(controllerIndexRegister));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, CRTController::Register_t value)
    {
        SYS_WritePortByte(controllerIndexRegister, value);
    }

}

}}
