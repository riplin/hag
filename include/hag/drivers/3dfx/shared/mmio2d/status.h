//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/status.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Status
{
    
    inline Shared::TwoD::Status_t Read(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Status_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Status));
    }

}

}