//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <hag/drivers/3dfx/shared/regs/pllctrl0.h>

namespace Hag::TDfx::Shared::IO
{

namespace PLLControl0
{
    inline Shared::PLLControl0_t Read(uint16_t ioBaseAddress)
    {
        return Shared::PLLControl0_t(SYS_ReadPortDouble(ioBaseAddress + Register::PLLControl0));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::PLLControl0_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::PLLControl0, value);
    }
}

}