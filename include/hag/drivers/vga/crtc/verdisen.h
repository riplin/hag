//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

namespace Register
{

enum
{
    VerticalDisplayEnd = 0x12,                              //VDE CR12
};

}

typedef uint8_t VerticalDisplayEnd_t;

namespace VerticalDisplayEnd
{
    enum //Mask
    {
        VerticalDisplayEndLow = 0xFF        //11 bit Value = (number of scan line of active display) - 1. This register contains
                                            //       the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            VerticalDisplayEndLow = 0x00
        };
    }

    inline VerticalDisplayEnd_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::VerticalDisplayEnd);
        return VerticalDisplayEnd_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, VerticalDisplayEnd_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::VerticalDisplayEnd, CRTControllerData_t(value));
    }

}

}}}
