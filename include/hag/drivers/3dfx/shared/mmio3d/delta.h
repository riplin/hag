//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3d/delta.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Delta
{

    inline void WriteDeltaRDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaRDeltaX))) = value;
    }

    inline void WriteDeltaGDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaGDeltaX))) = value;
    }

    inline void WriteDeltaBDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaBDeltaX))) = value;
    }

    inline void WriteDeltaZDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed20p12_t value)
    {
        *((Shared::ThreeD::Fixed20p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaZDeltaX))) = value;
    }

    inline void WriteDeltaADeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaADeltaX))) = value;
    }

    inline void WriteDeltaSDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaSDeltaX | (int)chip))) = value;
    }

    inline void WriteDeltaTDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaTDeltaX | (int)chip))) = value;
    }

    inline void WriteDeltaWDeltaX(uint8_t* baseAddress, Shared::ThreeD::Fixed2p30_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed2p30_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaWDeltaX | (int)chip))) = value;
    }

    inline void WriteDeltaRDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaRDeltaY))) = value;
    }

    inline void WriteDeltaGDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaGDeltaY))) = value;
    }

    inline void WriteDeltaBDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaBDeltaY))) = value;
    }

    inline void WriteDeltaZDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed20p12_t value)
    {
        *((Shared::ThreeD::Fixed20p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaZDeltaY))) = value;
    }

    inline void WriteDeltaADeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed12p12_t value)
    {
        *((Shared::ThreeD::Fixed12p12_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaADeltaY))) = value;
    }

    inline void WriteDeltaSDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaSDeltaY | (int)chip))) = value;
    }

    inline void WriteDeltaTDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed14p18_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed14p18_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaTDeltaY | (int)chip))) = value;
    }

    inline void WriteDeltaWDeltaY(uint8_t* baseAddress, Shared::ThreeD::Fixed2p30_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::Fixed2p30_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DeltaWDeltaY | (int)chip))) = value;
    }

    inline void WriteFloatDeltaRDeltaX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaRDeltaX))) = value;
    }

    inline void WriteFloatDeltaGDeltaX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaGDeltaX))) = value;
    }

    inline void WriteFloatDeltaBDeltaX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaBDeltaX))) = value;
    }

    inline void WriteFloatDeltaZDeltaX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaZDeltaX))) = value;
    }

    inline void WriteFloatDeltaADeltaX(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaADeltaX))) = value;
    }

    inline void WriteFloatDeltaSDeltaX(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaSDeltaX | chip))) = value;
    }

    inline void WriteFloatDeltaTDeltaX(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaTDeltaX | (int)chip))) = value;
    }

    inline void WriteFloatDeltaWDeltaX(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaWDeltaX | (int)chip))) = value;
    }

    inline void WriteFloatDeltaRDeltaY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaRDeltaY))) = value;
    }

    inline void WriteFloatDeltaGDeltaY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaGDeltaY))) = value;
    }

    inline void WriteFloatDeltaBDeltaY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaBDeltaY))) = value;
    }

    inline void WriteFloatDeltaZDeltaY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaZDeltaY))) = value;
    }

    inline void WriteFloatDeltaADeltaY(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaADeltaY))) = value;
    }

    inline void WriteFloatDeltaSDeltaY(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaSDeltaY | (int)chip))) = value;
    }

    inline void WriteFloatDeltaTDeltaY(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaTDeltaY | (int)chip))) = value;
    }

    inline void WriteFloatDeltaWDeltaY(uint8_t* baseAddress, float value, Shared::ThreeD::Chip_t chip)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatDeltaWDeltaY | (int)chip))) = value;
    }

}

}