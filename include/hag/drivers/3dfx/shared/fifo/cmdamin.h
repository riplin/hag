//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandAMin0 = 0x034,
        CommandAMin1 = 0x064
    };
}

// cmdAMin? is a 25-bit register containing the min address register. CmdAMin register is updated with
// the cmdAMax register when hole count is zero. This register is read write and has no default value. At
// initialization this register should be set to cmdBaseAddr - 4. The value read back from this register is 4
// more than that written.

typedef uint32_t CommandAMin_t;
namespace CommandAMin
{
    enum
    {
        Value = 0x00ffffff  // Byte Aligned Address Min register, bits 0 and 1 are ignored. Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00
        };
    }

    template <int V>
    CommandAMin_t Read(uint8_t* baseAddress)
    {
        return *((CommandAMin_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandAMin0 : Register::CommandAMin1)));
    }
    
    template <int V>
    void Write(uint8_t* baseAddress, CommandAMin_t value)
    {
        *((CommandAMin_t volatile *)(baseAddress + Shared::Fifo::Register::Base + ((V == 0) ? Register::CommandAMin0 : Register::CommandAMin1))) = value;
    }

}

}