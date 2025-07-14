//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbipo.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbiPixelsOut
{

    inline Shared::ThreeD::FbiPixelsOut_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbiPixelsOut_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbiPixelsOut)));
    }

}

}