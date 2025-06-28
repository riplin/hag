//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/xy.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace XY
{

    inline Shared::TwoD::XY_t ReadSource(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::XY_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceXY));
    }

    inline Shared::TwoD::XY_t ReadDestination(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::XY_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationXY));
    }

    inline void WriteSource(uint8_t* baseAddress, Shared::TwoD::XY_t value)
    {
        *((Shared::TwoD::XY_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceXY)) = value;
    }

    inline void WriteDestination(uint8_t* baseAddress, Shared::TwoD::XY_t value)
    {
        *((Shared::TwoD::XY_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationXY)) = value;
    }

}

}