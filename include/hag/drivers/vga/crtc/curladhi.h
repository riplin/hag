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
    CursorLocationAddressHigh = 0x0E,                       //CLA(H) CRE
};

}

typedef uint8_t CursorLocationAddressHigh_t;

namespace CursorLocationAddressHigh
{
    enum //Mask
    {
        CursorAddressHigh = 0xFF            //20 bit Value = the cursor location address of the video memory where the text cursor is active.
                                            //This register along with bits 3-0 of CR69 are the high order bits of the address.
    };

    namespace Shift
    {
        enum
        {
            CursorAddressHigh = 0x00
        };
    }

    inline CursorLocationAddressHigh_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::CursorLocationAddressHigh);
        return CursorLocationAddressHigh_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, CursorLocationAddressHigh_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::CursorLocationAddressHigh, CRTControllerData_t(value));
    }

}

}}}
