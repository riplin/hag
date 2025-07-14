//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/svertex.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace sVertex
{

    inline void WriteX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexX))) = value;
    }

    inline void WriteY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexY))) = value;
    }

    inline void WriteARGB(uint8_t* baseAddress, Shared::ThreeD::sARGB_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::sARGB_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexARGB | chip))) = value;
    }

    inline void WriteRed(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexRed))) = value;
    }

    inline void WriteGreen(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexGreen))) = value;
    }

    inline void WriteBlue(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexBlue))) = value;
    }

    inline void WriteAlpha(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexAlpha))) = value;
    }

    inline void WriteZ(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexZ))) = value;
    }

    inline void WriteW(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexW))) = value;
    }

    inline void WriteWTmu0(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexWTmu0 | chip))) = value;
    }

    inline void WriteSdW0(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexSdW0 | chip))) = value;
    }

    inline void WriteTdW0(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexTdW0 | chip))) = value;
    }

    inline void WriteWTmu1(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexWTmu1))) = value;
    }

    inline void WriteSdWTmu1(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexSdWTmu1))) = value;
    }

    inline void WriteTdWTmu1(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::sVertexTdWTmu1))) = value;
    }

}

}