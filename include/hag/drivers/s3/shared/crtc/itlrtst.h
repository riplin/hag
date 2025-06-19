//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    InterlaceRetraceStart = 0x3C,                           //IL_RTSTART CR3C
};

}

typedef uint8_t InterlaceRetraceStart_t;

namespace InterlaceRetraceStart
{

    inline InterlaceRetraceStart_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::InterlaceRetraceStart);
        return InterlaceRetraceStart_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, InterlaceRetraceStart_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::InterlaceRetraceStart, VGA::CRTControllerData_t(value));
    }

};

}
