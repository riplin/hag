//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/pllctrl2.h>
#include <hag/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace PLLControl2
{
    inline Shared::PLLControl2_t Read(uint16_t ioBaseAddress)
    {
        return Shared::PLLControl2_t(SYS_ReadPortDouble(ioBaseAddress + Register::PLLControl2));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::PLLControl2_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::PLLControl2, value);
    }
}

}