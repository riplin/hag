//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/dstfmt.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace DestinationFormat
{

    inline Shared::TwoD::DestinationFormat_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::DestinationFormat_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationFormat));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::DestinationFormat_t value)
    {
        *((Shared::TwoD::DestinationFormat_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationFormat)) = value;
    }

}

}