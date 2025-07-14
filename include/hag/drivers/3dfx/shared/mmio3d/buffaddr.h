//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/buffaddr.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace BufferAddress
{

    inline Shared::ThreeD::BufferAddress_t ReadColor(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::BufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ColorBufferAddress)));
    }

    inline Shared::ThreeD::BufferAddress_t ReadAux(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::BufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AuxBufferAddress)));
    }

    inline void WriteColor(uint8_t* baseAddress, Shared::ThreeD::BufferAddress_t value)
    {
        *((Shared::ThreeD::BufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ColorBufferAddress))) = value;
    }

    inline void WriteAux(uint8_t* baseAddress, Shared::ThreeD::BufferAddress_t value)
    {
        *((Shared::ThreeD::BufferAddress_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AuxBufferAddress))) = value;
    }

}

}