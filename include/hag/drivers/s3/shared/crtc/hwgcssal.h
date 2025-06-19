//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/hwgcmode.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    HardwareGraphicsCursorStorageStartAddressL = 0x4D,      //HWGC_STAL CR4D
};

}

typedef uint8_t HardwareGraphicsCursorStorageStartAddressL_t;

namespace HardwareGraphicsCursorStorageStartAddressL
{

    inline HardwareGraphicsCursorStorageStartAddressL_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorStorageStartAddressL);
        return HardwareGraphicsCursorStorageStartAddressL_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorStorageStartAddressL_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorStorageStartAddressL, VGA::CRTControllerData_t(value));
    }

};

}
