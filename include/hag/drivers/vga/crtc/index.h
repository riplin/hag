//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace VGA
{

typedef uint8_t CRTControllerIndex_t;

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

    inline CRTControllerIndex_t Read(Register_t controllerIndexRegister)
    {
        return CRTControllerIndex_t(SYS_ReadPortByte(controllerIndexRegister));
    }

    inline void Write(Register_t controllerIndexRegister, CRTControllerIndex_t value)
    {
        SYS_WritePortByte(controllerIndexRegister, value);
    }

}

}}
