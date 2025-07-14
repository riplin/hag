//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3d/start.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Start
{

    inline void WriteStartR(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartR))) = value;
    }

    inline void WriteStartG(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartG))) = value;
    }

    inline void WriteStartB(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartB))) = value;
    }

    inline void WriteStartZ(uint8_t* baseAddress, Shared::ThreeD::Fixed20p12_t value)
    {
        *((Shared::ThreeD::Fixed20p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartZ))) = value;
    }

    inline void WriteStartA(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartA))) = value;
    }

    inline void WriteStartS(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartS | chip))) = value;
    }

    inline void WriteStartT(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartT | chip))) = value;
    }

    inline void WriteStartW(uint8_t* baseAddress, Shared::ThreeD::Fixed2p30_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed2p30_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::StartW | chip))) = value;
    }

    inline void WriteFloatStartR(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartR))) = value;
    }

    inline void WriteFloatStartG(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartG))) = value;
    }

    inline void WriteFloatStartB(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartB))) = value;
    }

    inline void WriteFloatStartZ(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartZ))) = value;
    }

    inline void WriteFloatStartA(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartA))) = value;
    }

    inline void WriteFloatStartS(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartS | chip))) = value;
    }

    inline void WriteFloatStartT(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartT | chip))) = value;
    }

    inline void WriteFloatStartW(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatStartW | chip))) = value;
    }

}

}