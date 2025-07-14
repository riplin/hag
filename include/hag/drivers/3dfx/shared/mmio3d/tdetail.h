//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/tdetail.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace TextureDetail
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::TextureDetail_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureDetail_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureDetail | (int)chip))) = value;
    }

}

}