//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fogm.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FogMode
{

    inline Shared::ThreeD::FogMode_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FogMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FogMode)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::FogMode_t value)
    {
        *((Shared::ThreeD::FogMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FogMode))) = value;
    }

}

}