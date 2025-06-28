//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/baseaddr.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace BaseAddress
{

    inline Shared::TwoD::BaseAddress_t ReadSource(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::BaseAddress_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceBaseAddress));
    }

    inline void WriteSource(uint8_t* baseAddress, Shared::TwoD::BaseAddress_t value)
    {
        *((Shared::TwoD::BaseAddress_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceBaseAddress)) = value;
    }

    inline Shared::TwoD::BaseAddress_t ReadDestination(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::BaseAddress_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationBaseAddress));
    }

    inline void WriteDestination(uint8_t* baseAddress, Shared::TwoD::BaseAddress_t value)
    {
        *((Shared::TwoD::BaseAddress_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationBaseAddress)) = value;
    }

}

}