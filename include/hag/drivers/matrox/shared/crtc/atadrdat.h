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
    AttributeAddressDataSelect = 0x24,                          //CR24
};

}

typedef VGA::CRTControllerData_t AttributeAddressDataSelect_t;

namespace AttributeAddressDataSelect
{
    
    enum //Mask
    {
        DataSelect = 0x80,              //0 = The attributes controller is ready to accept an address value.
                                        //1 = The attributes controller is ready to accept a data value.
    };

    namespace Shift
    {
        enum
        {
            DataSelect = 0x07,
        };
    }

    inline AttributeAddressDataSelect_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::AttributeAddressDataSelect);
        return AttributeAddressDataSelect_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, AttributeAddressDataSelect_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::AttributeAddressDataSelect, VGA::CRTControllerData_t(value));
    }

}

}}}}