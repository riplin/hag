//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/color.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Color
{

    inline Shared::ThreeD::Color_t Read0(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::Color_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Color0)));
    }

    inline Shared::ThreeD::Color_t Read1(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::Color_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Color1)));
    }

    inline void Write0(uint8_t* baseAddress, Shared::ThreeD::Color_t value)
    {
        *((Shared::ThreeD::Color_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Color0))) = value;
    }

    inline void Write1(uint8_t* baseAddress, Shared::ThreeD::Color_t value)
    {
        *((Shared::ThreeD::Color_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Color1))) = value;
    }

}

}