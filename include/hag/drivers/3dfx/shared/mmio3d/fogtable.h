//Copyright 2025-Present riplin

#pragma once

#include <assert.h>
#include <hag/drivers/3dfx/shared/regs/3D/fogtable.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FogTable
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::FogTable_t value, uint8_t index)
    {
        assert(index < 64);
        *((Shared::ThreeD::FogTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FogTable) + (int)index)) = value;
    }

}

}