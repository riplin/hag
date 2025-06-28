//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/lnstp.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace LineStipple
{

    inline Shared::TwoD::LineStipple_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::LineStipple_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::LineStipple));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::LineStipple_t value)
    {
        *((Shared::TwoD::LineStipple_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::LineStipple)) = value;
    }

}

}