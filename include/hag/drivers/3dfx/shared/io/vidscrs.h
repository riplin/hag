//Copyright 2025-Present riplin

#pragma once

#include <has/system/sysasm.h>
#include <hag/drivers/3dfx/shared/regs/vidscrs.h>

namespace Hag::TDfx::Shared::IO
{

namespace VideoScreenSize
{
    inline Shared::VideoScreenSize_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VideoScreenSize_t(SYS_ReadPortDouble(ioBaseAddress + Register::VideoScreenSize));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VideoScreenSize_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VideoScreenSize, value);
    }
}

}