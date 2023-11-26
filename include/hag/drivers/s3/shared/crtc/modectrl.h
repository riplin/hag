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
    ModeControl = 0x42,                                     //MODE_CTL CR42
};

}

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
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ModeControl);
        return ModeControl_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ModeControl_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ModeControl, VGA::CRTControllerData_t(value));
    }

};

}}}}
