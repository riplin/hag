//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/viddostr.h>
#include <has/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace VideoDesktopOverlayStride
{
    inline Shared::VideoDesktopOverlayStride_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VideoDesktopOverlayStride_t(SYS_ReadPortDouble(ioBaseAddress + Register::VideoDesktopOverlayStride));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VideoDesktopOverlayStride_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VideoDesktopOverlayStride, value);
    }
}

}