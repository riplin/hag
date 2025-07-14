//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/lfbm.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace LinearFrameBufferMode
{

    inline Shared::ThreeD::LinearFrameBufferMode_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::LinearFrameBufferMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::LinearFrameBufferMode)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::LinearFrameBufferMode_t value)
    {
        *((Shared::ThreeD::LinearFrameBufferMode_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::LinearFrameBufferMode))) = value;
    }

}

}