//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/index.h>
#include <hag/drivers/vga/crtc/regs.h>

namespace Hag { namespace VGA
{

typedef uint8_t CRTControllerData_t;

namespace CRTControllerData
{

    inline CRTControllerData_t Read(Register_t controllerDataRegister)
    {
        return CRTControllerData_t(SYS_ReadPortByte(controllerDataRegister));
    }

    inline void Write(Register_t controllerDataRegister, CRTControllerData_t value)
    {
        SYS_WritePortByte(controllerDataRegister, value);
    }
    
    inline void Write(Register_t controllerIndexRegister, CRTControllerIndex_t crtIndex, CRTControllerData_t value)
    {
        SYS_WritePortBytes(controllerIndexRegister, crtIndex, value);
    }
    
}

}}
