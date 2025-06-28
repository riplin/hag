//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/rop.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace RasterOperation
{

    inline Shared::TwoD::RasterOperation_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::RasterOperation_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::RasterOperation));
    }

    inline void Write(uint8_t* baseAddress, Shared::TwoD::RasterOperation_t value)
    {
        *((Shared::TwoD::RasterOperation_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::RasterOperation)) = value;
    }

}

}