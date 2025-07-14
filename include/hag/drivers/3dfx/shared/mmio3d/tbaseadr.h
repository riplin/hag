//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/tbaseadr.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace TextureBaseAddress
{

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::TextureBaseAddress_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureBaseAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureBaseAddress | chip))) = value;
    }

    inline void Write1(uint8_t* baseAddress, Shared::ThreeD::TextureBaseAddress_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureBaseAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureBaseAddress1 | chip))) = value;
    }

    inline void Write2(uint8_t* baseAddress, Shared::ThreeD::TextureBaseAddress_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureBaseAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureBaseAddress2 | chip))) = value;
    }

    inline void Write3_8(uint8_t* baseAddress, Shared::ThreeD::TextureBaseAddress_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::TextureBaseAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TextureBaseAddress3_8 | chip))) = value;
    }

}

}