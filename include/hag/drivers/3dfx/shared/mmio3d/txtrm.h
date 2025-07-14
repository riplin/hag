//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/txtrm.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace TextureMode
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::TextureMode_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureMode | (int)chip))) = value;
    }

}

}