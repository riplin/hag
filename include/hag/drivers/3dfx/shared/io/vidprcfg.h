//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/vidprcfg.h>
#include <has/system/sysasm.h>

namespace Hag::TDfx::Shared::IO
{

namespace VideoProcessorConfiguration
{
    inline Shared::VideoProcessorConfiguration_t Read(uint16_t ioBaseAddress)
    {
        return Shared::VideoProcessorConfiguration_t(SYS_ReadPortDouble(ioBaseAddress + Register::VideoProcessorConfiguration));
    }

    inline void Write(uint16_t ioBaseAddress, Shared::VideoProcessorConfiguration_t value)
    {
        SYS_WritePortDouble(ioBaseAddress + Register::VideoProcessorConfiguration, value);
    }
}

}