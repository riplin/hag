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
    LinearAddressWindowPositionH = 0x59,                    //LAW_POS_H CR59
};

}

typedef uint8_t LinearAddressWindowPositionH_t;

namespace LinearAddressWindowPositionH
{

    inline LinearAddressWindowPositionH_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::LinearAddressWindowPositionH);
        return LinearAddressWindowPositionH_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, LinearAddressWindowPositionH_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::LinearAddressWindowPositionH, VGA::CRTControllerData_t(value));
    }

};

}}}}
