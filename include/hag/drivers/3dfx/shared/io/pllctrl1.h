//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/pllctrl1.h>
#include <hag/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace PLLControl1
{
    inline Shared::PLLControl1_t Read(uint16_t ioBaseAddress)
    {
        return Shared::PLLControl1_t(SYS_ReadPortDouble(ioBaseAddress + Register::PLLControl1));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::PLLControl1_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::PLLControl1, value);
    }
}

}