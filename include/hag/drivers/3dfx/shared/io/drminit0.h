//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/drminit0.h>
#include <has/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace DRAMInit0
{
    inline Shared::DRAMInit0_t Read(uint16_t ioBaseAddress)
    {
        return Shared::DRAMInit0_t(SYS_ReadPortDouble(ioBaseAddress + Register::DRAMInit0));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::DRAMInit0_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::DRAMInit0, value);
    }
}

}