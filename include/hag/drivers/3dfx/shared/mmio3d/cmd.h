//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3d/cmd.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace Command
{

    inline void WriteTriangle(uint8_t* baseAddress, Shared::ThreeD::Command::Triangle_t value)
    {
        *((Shared::ThreeD::Command::Triangle_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::TriangleCommand))) = value;
    }

    inline void WriteNop(uint8_t* baseAddress, Shared::ThreeD::Command::Nop_t value)
    {
        *((Shared::ThreeD::Command::Nop_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NopCommand))) = value;
    }

    inline void WriteFastFill(uint8_t* baseAddress, Shared::ThreeD::Command::FastFill_t value)
    {
        *((Shared::ThreeD::Command::FastFill_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FastFillCommand))) = value;
    }

    inline void WriteSwapBuffer(uint8_t* baseAddress, Shared::ThreeD::Command::SwapBuffer_t value)
    {
        *((Shared::ThreeD::Command::SwapBuffer_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::SwapBufferCommand))) = value;
    }

    inline void WriteUserInterrupt(uint8_t* baseAddress, Shared::ThreeD::Command::UserInterrupt_t value)
    {
        *((Shared::ThreeD::Command::UserInterrupt_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::UserInterruptCommand))) = value;
    }

    inline void WriteDrawTriangle(uint8_t* baseAddress, Shared::ThreeD::Command::DrawTriangle_t value)
    {
        *((Shared::ThreeD::Command::DrawTriangle_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::DrawTriangleCommand))) = value;
    }

    inline void WriteBeginTriangle(uint8_t* baseAddress, Shared::ThreeD::Command::BeginTriangle_t value)
    {
        *((Shared::ThreeD::Command::BeginTriangle_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::BeginTriangleCommand))) = value;
    }

}

}