//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/pattern.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Pattern
{

    inline Shared::TwoD::Pattern_t Read0Alias(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Pattern_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Pattern0Alias));
    }

    inline Shared::TwoD::Pattern_t Read1Alias(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Pattern_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Pattern1Alias));
    }

    inline void Write0Alias(uint8_t* baseAddress, Shared::TwoD::Pattern_t value)
    {
        *((Shared::TwoD::Pattern_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Pattern0Alias)) = value;
    }

    inline void Write1Alias(uint8_t* baseAddress, Shared::TwoD::Pattern_t value)
    {
        *((Shared::TwoD::Pattern_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Pattern1Alias)) = value;
    }

    inline Shared::TwoD::Pattern_t volatile * Get(uint8_t* baseAddress)
    {
        return ((Shared::TwoD::Pattern_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Pattern));
    }

}

}