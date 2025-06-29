//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPHostAddressHigh = 0x008
    };
}

// During AGP transfers this address defines the source address bits 31:0 of AGP memory to fetch data
// from. AGP addresses are 36-bits in length and are byte aligned. The upper 4 bits reside in the
// agpHostAddressHigh register. This register is read write, and defaults to 0.

typedef uint32_t AGPHostAddressHigh_t;
namespace AGPHostAddressHigh
{
    enum
    {
        Width =         0x00003fff,
        Stride =        0x0fffc000,
        Address35_32 =  0xf0000000
    };

    namespace Shift
    {
        enum
        {
            Width = 0x00,
            Stride = 0x0e,
            Address35_32 = 0x1c
        };
    }

    inline AGPHostAddressHigh_t Read(uint8_t* baseAddress)
    {
        return *((AGPHostAddressHigh_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPHostAddressHigh));
    }

    inline void Write(uint8_t* baseAddress, AGPHostAddressHigh_t value)
    {
        *((AGPHostAddressHigh_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPHostAddressHigh)) = value;
    }

}

}