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
    HardwareGraphicsCursorStorageStartAddressH = 0x4C,      //HWGC_STAH CR4C
};

}

typedef uint8_t HardwareGraphicsCursorStorageStartAddressH_t;

namespace HardwareGraphicsCursorStorageStartAddressH
{

    inline HardwareGraphicsCursorStorageStartAddressH_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::HardwareGraphicsCursorStorageStartAddressH);
        return HardwareGraphicsCursorStorageStartAddressH_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, HardwareGraphicsCursorStorageStartAddressH_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::HardwareGraphicsCursorStorageStartAddressH, VGA::CRTControllerData_t(value));
    }

};

}
