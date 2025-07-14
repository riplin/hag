//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3d/vertex.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Vertex
{

    inline void WriteAx(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Ax))) = value;
    }

    inline void WriteAy(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Ay))) = value;
    }

    inline void WriteBx(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Bx))) = value;
    }

    inline void WriteBy(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::By))) = value;
    }

    inline void WriteCx(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Cx))) = value;
    }

    inline void WriteCy(uint8_t* baseAddress, Shared::ThreeD::Fixed12p4_t value)
    {
        *((Shared::ThreeD::Fixed12p4_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::Cy))) = value;
    }

    inline void WriteFloatAx(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatAx))) = value;
    }

    inline void WriteFloatAy(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatAy))) = value;
    }

    inline void WriteFloatBx(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatBx))) = value;
    }

    inline void WriteFloatBy(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatBy))) = value;
    }

    inline void WriteFloatCx(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatCx))) = value;
    }

    inline void WriteFloatCy(uint8_t* baseAddress, float value)
    {
        *((float volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FloatCy))) = value;
    }

}

}