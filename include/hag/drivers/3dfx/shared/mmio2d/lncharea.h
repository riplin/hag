//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/lncharea.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace LaunchArea
{

    //The Launch area is 128 bytes (32 commands) large.

    inline Shared::TwoD::LaunchCommand_t volatile * Get(uint8_t* baseAddress)
    {
        return (Shared::TwoD::LaunchCommand_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::LaunchArea);
    }

}

}