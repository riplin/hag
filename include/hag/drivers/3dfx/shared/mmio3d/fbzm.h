//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbzm.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace RGBDepthMode
{

    inline Shared::ThreeD::RGBDepthMode_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::RGBDepthMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::RGBDepthMode)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::RGBDepthMode_t value)
    {
        *((Shared::ThreeD::RGBDepthMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::RGBDepthMode))) = value;
    }

}

}