//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memorg.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace MemoryOrigin
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryOrigin_t value)
    {
        *((Shared::MemoryOrigin_t volatile *)(baseAddress + Shared::Register::MemoryOrigin)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryOrigin_t value)
    {
        *((Shared::MemoryOrigin_t volatile *)(baseAddress + Shared::Register::MemoryOriginCommit)) = value;
    }
}

}