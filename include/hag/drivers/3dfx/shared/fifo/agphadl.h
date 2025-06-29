//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPHostAddressLow = 0x004
    };
}

// During AGP transfers this address defines the source address bits 31:0 of AGP memory to fetch data
// from. AGP addresses are 36-bits in length and are byte aligned. The upper 4 bits reside in the
// agpHostAddressHigh register. This register is read write, and defaults to 0.

typedef uint32_t AGPHostAddressLow_t;
namespace AGPHostAddressLow
{
    enum
    {
        Address31_0 = 0xffffffff
    };

    namespace Shift
    {
        enum
        {
            Address31_0 = 0x00
        };
    }

    inline AGPHostAddressLow_t Read(uint8_t* baseAddress)
    {
        return *((AGPHostAddressLow_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPHostAddressLow));
    }

    inline void Write(uint8_t* baseAddress, AGPHostAddressLow_t value)
    {
        *((AGPHostAddressLow_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPHostAddressLow)) = value;
    }

}

}