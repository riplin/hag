//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        YUVBaseAddress = 0x100,
    };
}

// yuvBaseAddress register contains the starting frame buffer location of the yuv aperture.

typedef uint32_t YUVBaseAddress_t;
namespace YUVBaseAddress
{
    enum
    {
        Value = 0x01ffffff
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    inline YUVBaseAddress_t Read(uint8_t* baseAddress)
    {
        return *((YUVBaseAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::YUVBaseAddress));
    }
    
    inline void Write(uint8_t* baseAddress, YUVBaseAddress_t value)
    {
        *((YUVBaseAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::YUVBaseAddress)) = value;
    }

}

}