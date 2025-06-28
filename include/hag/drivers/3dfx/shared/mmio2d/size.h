//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/size.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Size
{

    inline Shared::TwoD::Size_t ReadSource(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Size_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceSize));
    }

    inline Shared::TwoD::Size_t ReadDestination(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Size_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationSize));
    }

    inline void WriteSource(uint8_t* baseAddress, Shared::TwoD::Size_t value)
    {
        *((Shared::TwoD::Size_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceSize)) = value;
    }

    inline void WriteDestination(uint8_t* baseAddress, Shared::TwoD::Size_t value)
    {
        *((Shared::TwoD::Size_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationSize)) = value;
    }

}

}