//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/zdorg.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace ZDepthOrigin
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::ZDepthOrigin_t value)
    {
        *((Shared::ZDepthOrigin_t volatile *)(baseAddress + Shared::Register::ZDepthOrigin)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryAccess_t value)
    {
        *((Shared::ZDepthOrigin_t volatile *)(baseAddress + Shared::Register::ZDepthOriginCommit)) = value;
    }
}

}