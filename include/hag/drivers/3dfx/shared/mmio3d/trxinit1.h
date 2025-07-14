//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/trxinit1.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace TrexInit1
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::TrexInit1_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TrexInit1_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TrexInit1 | (int)chip))) = value;
    }

}

}