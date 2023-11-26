//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/index.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    CRTControllerDataB = 0x3B5,                             //CRTC_DATA CRT //Mono
    CRTControllerDataD = 0x3D5,                             //CRTC_DATA CRT //Color
};

}

typedef uint8_t CRTControllerData_t;

namespace CRTControllerData
{

    inline CRTControllerData_t Read(VGA::Register_t controllerDataRegister)
    {
        return CRTControllerData_t(SYS_ReadPortByte(controllerDataRegister));
    }

    inline void Write(VGA::Register_t controllerDataRegister, CRTControllerData_t value)
    {
        SYS_WritePortByte(controllerDataRegister, value);
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, CRTController::Register_t crtIndex, CRTControllerData_t value)
    {
        SYS_WritePortBytes(controllerIndexRegister, crtIndex, value);
    }
    
}

}}
