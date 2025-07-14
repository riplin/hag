//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/tlod.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace TextureLevelOfDetail
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::TextureLevelOfDetail_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureLevelOfDetail_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureLevelOfDetail | (int)chip))) = value;
    }

}

}