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
    DeviceIDHigh = 0x2D,                                    //CR2D
};

}

//This register should contain the same value as the upper byte of the PCI Device ID (Index 0x00) register.

typedef uint8_t DeviceIDHigh_t;

namespace DeviceIDHigh
{

    inline DeviceIDHigh_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::DeviceIDHigh);
        return DeviceIDHigh_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}
