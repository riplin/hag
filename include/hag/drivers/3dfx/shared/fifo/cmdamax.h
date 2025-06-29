//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandAMax0 = 0x03c,
        CommandAMax1 = 0x06c
    };
}

// cmdAMax? is a 32-bit register containing the 25 bits of the max address register. CmdAMax register
// is automatically updated when an memory address greater to the existing cmdAMax register is written.
// At initialization, this register should be set to cmdBaseAddr - 4. The value read back from this register
// is 4 more than that written.

typedef uint32_t CommandAMax_t;
namespace CommandAMax
{
    enum
    {
        Value = 0x00ffffff  // Byte Aligned Address Max register, bits 0 and 1 are ignored. Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandAMax_t Read(uint8_t* baseAddress)
    {
        return *((CommandAMax_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandAMax0 : Register::CommandAMax1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandAMax_t value)
    {
        *((CommandAMax_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandAMax0 : Register::CommandAMax1))) = value;
    }

}

}