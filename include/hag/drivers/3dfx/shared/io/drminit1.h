//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <hag/drivers/3dfx/shared/regs/drminit1.h>

namespace Hag::TDfx::Shared::IO
{

namespace DRAMInit1
{
    inline Shared::DRAMInit1_t Read(uint16_t ioBaseAddress)
    {
        return Shared::DRAMInit1_t(SYS_ReadPortDouble(ioBaseAddress + Register::DRAMInit1));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::DRAMInit1_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::DRAMInit1, value);
    }
}

}