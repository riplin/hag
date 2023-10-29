//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline StartAddressLow_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::StartAddressLow);
        return StartAddressLow_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, StartAddressLow_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::StartAddressLow, CRTControllerData_t(value));
    }

    inline uint8_t GetDisplayStartAddressLow(StartAddressLow_t value)
    {
        return (value & DisplayStartAddressLow) >> Shift::DisplayStartAddressLow;
    }
    
    inline StartAddressLow_t SetDisplayStartAddressLow(uint8_t value)
    {
        return StartAddressLow_t((value << Shift::DisplayStartAddressLow) & DisplayStartAddressLow);
    }

}

}}}
