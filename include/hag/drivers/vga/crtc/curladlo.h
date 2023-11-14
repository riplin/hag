//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

typedef uint8_t CursorLocationAddressLow_t;

namespace CursorLocationAddressLow
{
    enum //Mask
    {
        CursorAddressLow = 0xFF       //Cursor location address (low) contains the 8 low order bits of the address.
    };

    namespace Shift
    {
        enum
        {
            CursorAddressLow = 0x00
        };
    }

    inline CursorLocationAddressLow_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::CursorLocationAddressLow);
        return CursorLocationAddressLow_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, CursorLocationAddressLow_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::CursorLocationAddressLow, CRTControllerData_t(value));
    }
}

}}}
