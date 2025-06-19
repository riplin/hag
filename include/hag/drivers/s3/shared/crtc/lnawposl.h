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
    LinearAddressWindowPositionL = 0x5A,                    //LAW_POS_L CR5A
};

}

typedef uint8_t LinearAddressWindowPositionL_t;

namespace LinearAddressWindowPositionL
{

    inline LinearAddressWindowPositionL_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::LinearAddressWindowPositionL);
        return LinearAddressWindowPositionL_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, LinearAddressWindowPositionL_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::LinearAddressWindowPositionL, VGA::CRTControllerData_t(value));
    }

};

}
