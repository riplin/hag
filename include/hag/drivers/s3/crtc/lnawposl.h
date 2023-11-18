//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t LinearAddressWindowPositionL_t;

namespace LinearAddressWindowPositionL
{

    inline LinearAddressWindowPositionL_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::LinearAddressWindowPositionL);
        return LinearAddressWindowPositionL_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, LinearAddressWindowPositionL_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::LinearAddressWindowPositionL, VGA::CRTControllerData_t(value));
    }

};

}}}
