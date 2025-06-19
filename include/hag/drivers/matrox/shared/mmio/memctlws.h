//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memctlws.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace MemoryControlWaitState
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryControlWaitState_t value)
    {
        *((Shared::MemoryControlWaitState_t volatile *)(baseAddress + Shared::Register::MemoryControlWaitState)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryControlWaitState_t value)
    {
        *((Shared::MemoryControlWaitState_t volatile *)(baseAddress + Shared::Register::MemoryControlWaitStateCommit)) = value;
    }
}

}