//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/srcfmt.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace SourceFormat
{

    inline Shared::TwoD::SourceFormat_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::SourceFormat_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceFormat));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::SourceFormat_t value)
    {
        *((Shared::TwoD::SourceFormat_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceFormat)) = value;
    }

}

}