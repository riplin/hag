//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/chrkey.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace ChromaKey
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::ChromaKey_t value)
    {
        *((Shared::ThreeD::ChromaKey_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ChromaKey))) = value;
    }

}

}