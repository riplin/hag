//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ModeControl_t;

namespace ModeControl
{
    enum
    {
        Interlaced = 0x20   //This bit enables the function of CR3C - Interlace Retrace Start register
    };

    namespace Shift
    {
        enum
        {
            Interlaced = 0x05
        };
    }
    inline ModeControl_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ModeControl);
        return ModeControl_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ModeControl_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ModeControl, VGA::CRTControllerData_t(value));
    }

};

}}}
