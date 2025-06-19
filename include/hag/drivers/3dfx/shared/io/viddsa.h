//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/viddsa.h>
#include <hag/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace VideoDesktopStartAddress
{
    inline Shared::VideoDesktopStartAddress_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VideoDesktopStartAddress_t(SYS_ReadPortDouble(ioBaseAddress + Register::VideoDesktopStartAddress));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VideoDesktopStartAddress_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VideoDesktopStartAddress, value);
    }
}

}