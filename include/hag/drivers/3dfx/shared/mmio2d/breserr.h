//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/breserr.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace BresenhamError
{

    inline Shared::TwoD::BresenhamError_t Read0(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::BresenhamError_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BresenhamError0));
    }

    inline Shared::TwoD::BresenhamError_t Read1(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::BresenhamError_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BresenhamError1));
    }

    inline void Write0(uint8_t* baseAddress, Shared::TwoD::BresenhamError_t value)
    {
        *((Shared::TwoD::BresenhamError_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BresenhamError0)) = value;
    }

    inline void Write1(uint8_t* baseAddress, Shared::TwoD::BresenhamError_t value)
    {
        *((Shared::TwoD::BresenhamError_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::BresenhamError1)) = value;
    }

}

}