//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/clip.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace Clip
{

    inline Shared::TwoD::Clip_t ReadClip0Min(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip0Min));
    }

    inline Shared::TwoD::Clip_t ReadClip0Max(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip0Max));
    }

    inline Shared::TwoD::Clip_t ReadClip1Min(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip1Min));
    }

    inline Shared::TwoD::Clip_t ReadClip1Max(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip1Max));
    }

    inline void WriteClip0Min(uint8_t* baseAddress, Shared::TwoD::Clip_t value)
    {
        *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip0Min)) = value;
    }

    inline void WriteClip0Max(uint8_t* baseAddress, Shared::TwoD::Clip_t value)
    {
        *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip0Max)) = value;
    }

    inline void WriteClip1Min(uint8_t* baseAddress, Shared::TwoD::Clip_t value)
    {
        *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip1Min)) = value;
    }

    inline void WriteClip1Max(uint8_t* baseAddress, Shared::TwoD::Clip_t value)
    {
        *((Shared::TwoD::Clip_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::Clip1Max)) = value;
    }

}

}