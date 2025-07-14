//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/ssetupm.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace sSetupMode
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::sSetupMode_t value)
    {
        *((Shared::ThreeD::sSetupMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sSetupMode))) = value;
    }

}

}