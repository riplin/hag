//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/clip.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Clip
{

    inline Shared::ThreeD::ClipLeftRight_t ReadLeftRight(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::ClipLeftRight_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipLeftRight)));
    }

    inline Shared::ThreeD::ClipTopBottom_t ReadTopBottom(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::ClipTopBottom_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipTopBottom)));
    }

    inline Shared::ThreeD::ClipLeftRight1_t ReadLeftRight1(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::ClipLeftRight1_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipLeftRight1)));
    }

    inline Shared::ThreeD::ClipTopBottom1_t ReadTopBottom1(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::ClipTopBottom1_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipTopBottom1)));
    }

    inline void WriteLeftRight(uint8_t* baseAddress, Shared::ThreeD::ClipLeftRight_t value)
    {
        *((Shared::ThreeD::ClipLeftRight_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipLeftRight))) = value;
    }

    inline void WriteTopBottom(uint8_t* baseAddress, Shared::ThreeD::ClipTopBottom_t value)
    {
        *((Shared::ThreeD::ClipTopBottom_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipTopBottom))) = value;
    }

    inline void WriteLeftRight1(uint8_t* baseAddress, Shared::ThreeD::ClipLeftRight1_t value)
    {
        *((Shared::ThreeD::ClipLeftRight1_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipLeftRight1))) = value;
    }

    inline void WriteTopBottom1(uint8_t* baseAddress, Shared::ThreeD::ClipTopBottom1_t value)
    {
        *((Shared::ThreeD::ClipTopBottom1_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ClipTopBottom1))) = value;
    }

}

}