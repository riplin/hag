//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/colpth.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace FbzColorPath
{

    inline Shared::ThreeD::FbzColorPath_t Read(uint8_t* baseAddress)
    {
        return *((Shared::ThreeD::FbzColorPath_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbzColorPath)));
    }

    inline void Write(uint8_t* baseAddress, Shared::ThreeD::FbzColorPath_t value)
    {
        *((Shared::ThreeD::FbzColorPath_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::FbzColorPath))) = value;
    }

}

}