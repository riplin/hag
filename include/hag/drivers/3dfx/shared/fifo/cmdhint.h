//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/fifo/fifobase.h>

namespace Hag::TDfx::Shared::Fifo
{

namespace Register
{
    enum
    {
        CommandHoleInterrupt = 0x084,
    };
}

// cmdHoleInt bits 21:0 contain the number of MCLK cycles a hole counter can have a hole before
// genreating an interrupt. The counter is only enabled when bit 22 of this register is set. This register
// should be used in combination with the IntrCtrl register to product PCI interrupts for flagging
// insufficient data.

typedef uint32_t CommandHoleInterrupt_t;
namespace CommandHoleInterrupt
{
    enum
    {
        TimeOut = 0x003fffff,   // CMDFIFO 0 and 1 (holes !=0) time out value. Default is 0x0.
        Enable = 0x00400000,    // CMDFIFO Time Out Counter Enable. (0=Disable, 1 = Enable). Default is 0x0.
    };

    namespace Shift
    {
        enum
        {
            TimeOut = 0x00,
            Enable = 0x16
        };
    }

    inline CommandHoleInterrupt_t Read(uint8_t* baseAddress)
    {
        return *((CommandHoleInterrupt_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::CommandHoleInterrupt));
    }
    
    inline void Write(uint8_t* baseAddress, CommandHoleInterrupt_t value)
    {
        *((CommandHoleInterrupt_t volatile *)(baseAddress + Shared::Fifo::Register::Base + Register::CommandHoleInterrupt)) = value;
    }

}

}