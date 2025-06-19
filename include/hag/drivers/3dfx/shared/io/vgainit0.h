//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/vgainit0.h>
#include <hag/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace VGAInit0
{
    inline Shared::VGAInit0_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VGAInit0_t(SYS_ReadPortDouble(ioBaseAddress + Register::VGAInit0));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VGAInit0_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VGAInit0, value);
    }
}

}