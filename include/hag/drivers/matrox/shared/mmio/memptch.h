//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memptch.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace MemoryPitch
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryPitch_t value)
    {
        *((Shared::MemoryPitch_t volatile *)(baseAddress + Shared::Register::MemoryPitch)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryPitch_t value)
    {
        *((Shared::MemoryPitch_t volatile *)(baseAddress + Shared::Register::MemoryPitchCommit)) = value;
    }
}

}