//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/lnstl.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace LineStyle
{

    inline Shared::TwoD::LineStyle_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::LineStyle_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::LineStyle));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::LineStyle_t value)
    {
        *((Shared::TwoD::LineStyle_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::LineStyle)) = value;
    }

}

}