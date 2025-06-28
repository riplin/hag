//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/color.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Color
{

    inline Shared::TwoD::Color_t ReadForeground(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Color_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::ForegroundColor));
    }

    inline Shared::TwoD::Color_t ReadBackground(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Color_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BackgroundColor));
    }

    inline void WriteForeground(uint8_t* baseAddress, Shared::TwoD::Color_t value)
    {
        *((Shared::TwoD::Color_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::ForegroundColor)) = value;
    }

    inline void WriteBackground(uint8_t* baseAddress, Shared::TwoD::Color_t value)
    {
        *((Shared::TwoD::Color_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BackgroundColor)) = value;
    }

}

}