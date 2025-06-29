//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        AGPGraphicsAddress = 0x00c
    };
}

// agpgraphicsAddress defines the destination frame buffer address and type of the AGP transfer. At the
// beginning of an AGP transfer this address is loaded into an internal address pointer that increments for
// each data received over AGP . This register is read write, and defaults to 0.

typedef uint32_t AGPGraphicsAddress_t;
namespace AGPGraphicsAddress
{
    enum
    {
        FramebufferOffset = 0x03ffffff
    };

    namespace Shift
    {
        enum
        {
            FramebufferOffset = 0x00
        };
    }

    inline AGPGraphicsAddress_t Read(uint8_t* baseAddress)
    {
        return *((AGPGraphicsAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPGraphicsAddress));
    }

    inline void Write(uint8_t* baseAddress, AGPGraphicsAddress_t value)
    {
        *((AGPGraphicsAddress_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::AGPGraphicsAddress)) = value;
    }

}

}