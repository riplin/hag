//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/alpham.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace AlphaMode
{

    inline Shared::ThreeD::AlphaMode_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::AlphaMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AlphaMode)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::AlphaMode_t value)
    {
        *((Shared::ThreeD::AlphaMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AlphaMode))) = value;
    }

}

}