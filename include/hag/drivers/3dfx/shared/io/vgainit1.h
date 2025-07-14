//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/vgainit1.h>
#include <has/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace VGAInit1
{
    inline Shared::VGAInit1_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VGAInit1_t(SYS_ReadPortDouble(ioBaseAddress + Register::VGAInit1));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VGAInit1_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VGAInit1, value);
    }
}

}