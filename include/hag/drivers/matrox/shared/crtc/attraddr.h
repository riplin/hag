//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/matrox/shared/crtc/regtype.h>

namespace Hag { namespace Matrox { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    AttributeAddress = 0x26,                          //CR26
};

}

typedef VGA::CRTControllerData_t AttributeAddress_t;

namespace AttributeAddress
{
    
    enum //Mask
    {
        Address = 0x1F,                 //VGA Attribute address
        DataSelect = 0x20,              //VGA Palette enable
    };

    namespace Shift
    {
        enum
        {
            Address = 0x00,
            DataSelect = 0x05,
        };
    }

    inline AttributeAddress_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::AttributeAddress);
        return AttributeAddress_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, AttributeAddress_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::AttributeAddress, VGA::CRTControllerData_t(value));
    }

}

}}}}