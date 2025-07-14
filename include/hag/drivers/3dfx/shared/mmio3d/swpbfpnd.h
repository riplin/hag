//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/swpbfpnd.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace SwapBufferPending
{

    // Don't use this! The SwapBufferPending register is bugged!
    inline void Write(uint8_t* baseAddress, Shared::ThreeD::SwapBufferPending_t value)
    {
        *((Shared::ThreeD::SwapBufferPending_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::SwapBufferPending))) = value;
    }

}

}