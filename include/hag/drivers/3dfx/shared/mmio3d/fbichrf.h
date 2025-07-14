//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbichrf.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbiChromaFail
{

    inline Shared::ThreeD::FbiChromaFail_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbiChromaFail_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbiChromaFail)));
    }

}

}