//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/2dbase.h>
#include <hag/drivers/3dfx/shared/regs/status.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Status
{
    
    inline Shared::Status_t Read(uint8_t* baseAddress)
    {
        return *((Shared::Status_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::Register::Status));
    }

}

}