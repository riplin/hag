//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/fbiaff.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbiAFuncFail
{

    inline Shared::ThreeD::FbiAFuncFail_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbiAFuncFail_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbiAFuncFail)));
    }

}

}