//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/intctrl.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace InterruptControl
{

    inline Shared::TwoD::InterruptControl_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::InterruptControl_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::InterruptControl));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::InterruptControl_t value)
    {
        *((Shared::TwoD::InterruptControl_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::InterruptControl)) = value;
    }

}

}