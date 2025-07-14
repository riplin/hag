//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/buffstrd.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace BufferStride
{

    inline Shared::ThreeD::BufferStride_t ReadColor(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::BufferStride_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ColorBufferStride)));
    }

    inline Shared::ThreeD::BufferStride_t ReadAux(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::BufferStride_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AuxBufferStride)));
    }

    inline void WriteColor(uint8_t* baseAddress, Shared::ThreeD::BufferStride_t value)
    {
        *((Shared::ThreeD::BufferStride_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::ColorBufferStride))) = value;
    }

    inline void WriteAux(uint8_t* baseAddress, Shared::ThreeD::BufferStride_t value)
    {
        *((Shared::ThreeD::BufferStride_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::AuxBufferStride))) = value;
    }

}

}