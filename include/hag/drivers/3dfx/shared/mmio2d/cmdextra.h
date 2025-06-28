//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/cmdextra.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace CommandExtra
{

    inline Shared::TwoD::CommandExtra_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::CommandExtra_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::CommandExtra));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::CommandExtra_t value)
    {
        *((Shared::TwoD::CommandExtra_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::CommandExtra)) = value;
    }

}

}