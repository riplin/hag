//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbipi.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbiPixelsIn
{

    inline Shared::ThreeD::FbiPixelsIn_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbiPixelsIn_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbiPixelsIn)));
    }

}

}