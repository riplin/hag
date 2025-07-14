//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbizff.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbiZFuncFail
{

    inline Shared::ThreeD::FbiZFuncFail_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbiZFuncFail_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbiZFuncFail)));
    }

}

}