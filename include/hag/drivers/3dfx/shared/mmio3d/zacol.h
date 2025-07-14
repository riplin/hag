//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/zacol.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace DepthAlphaConstant
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::DepthAlphaConstant_t value)
    {
        *((Shared::ThreeD::DepthAlphaConstant_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DepthAlphaConstant))) = value;
    }

}

}