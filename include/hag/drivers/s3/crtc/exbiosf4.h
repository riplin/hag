//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedBIOSFlag4_t;

namespace ExtendedBIOSFlag4
{

    inline ExtendedBIOSFlag4_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedBIOSFlag4);
        return ExtendedBIOSFlag4_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedBIOSFlag4_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedBIOSFlag4, VGA::CRTControllerData_t(value));
    }

}

}}}
