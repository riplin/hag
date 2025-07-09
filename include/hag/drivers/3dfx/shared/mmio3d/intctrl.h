//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/intctrl.h>
#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace InterruptControl
{

    inline Shared::InterruptControl_t Read(uint8_t* baseAddress)
    {
        return *((Shared::InterruptControl_t volatile *)(baseAddress + Shared::ThreeD::Register::Base + Shared::Register::InterruptControl));
    }

    inline void Write(uint8_t* baseAddress, Shared::InterruptControl_t value)
    {
        *((Shared::InterruptControl_t volatile *)(baseAddress + Shared::ThreeD::Register::Base + Shared::Register::InterruptControl)) = value;
    }

}

}