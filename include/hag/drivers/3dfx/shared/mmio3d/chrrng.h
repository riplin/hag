//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/chrrng.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace ChromaRange
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::ChromaRange_t value)
    {
        *((Shared::ThreeD::ChromaRange_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ChromaRange))) = value;
    }

}

}