//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedMemoryControl3_t;

namespace ExtendedMemoryControl3
{

    inline ExtendedMemoryControl3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMemoryControl3);
        return ExtendedMemoryControl3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMemoryControl3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMemoryControl3, VGA::CRTControllerData_t(value));
    }

}

}}}
