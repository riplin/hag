//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/stipple.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Stipple
{

    inline Shared::ThreeD::Stipple_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::Stipple_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Stipple)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::Stipple_t value)
    {
        *((Shared::ThreeD::Stipple_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Stipple))) = value;
    }

}

}