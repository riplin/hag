//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::VGA::CRTController
{

namespace Register
{

enum
{
    StartAddressLow = 0x0D,                                 //STA(L) CRD
};

}

typedef uint8_t StartAddressLow_t;

namespace StartAddressLow
{
    enum //Mask
    {
        DisplayStartAddressLow = 0xFF       //Start address (low) contains the 8 low order bits of the address.
    };

    namespace Shift
    {
        enum
        {
            DisplayStartAddressLow = 0x00
        };
    }

    inline StartAddressLow_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::StartAddressLow);
        return StartAddressLow_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, StartAddressLow_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::StartAddressLow, CRTControllerData_t(value));
    }

}

}
