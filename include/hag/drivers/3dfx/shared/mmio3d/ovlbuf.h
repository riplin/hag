//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/ovlbuf.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace OverlayBufferAddress
{

    inline void WriteLeft(uint8_t* baseAddress, Shared::ThreeD::OverlayBufferAddress_t value)
    {
        *((Shared::ThreeD::OverlayBufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::LeftOverlayBufferAddress))) = value;
    }

    inline void WriteRight(uint8_t* baseAddress, Shared::ThreeD::OverlayBufferAddress_t value)
    {
        *((Shared::ThreeD::OverlayBufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::RightOverlayBufferAddress))) = value;
    }

}

}