//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/dacmode.h>
#include <hag/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace DACMode
{
    inline Shared::DACMode_t Read(uint16_t ioBaseAddress)
    {
        return Shared::DACMode_t(SYS_ReadPortDouble(ioBaseAddress + Register::DACMode));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::DACMode_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::DACMode, value);
    }
}

}