//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/cmd.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Command
{

    inline Shared::TwoD::Command_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Command_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Command));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::Command_t value)
    {
        *((Shared::TwoD::Command_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Command)) = value;
    }

}

}