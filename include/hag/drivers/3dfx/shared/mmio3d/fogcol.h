//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fogcol.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FogColor
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::FogColor_t value)
    {
        *((Shared::ThreeD::FogColor_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FogColor))) = value;
    }

}

}