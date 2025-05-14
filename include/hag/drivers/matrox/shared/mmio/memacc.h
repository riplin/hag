//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memacc.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace MemoryAccess
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryAccess_t value)
    {
        *((Shared::MemoryAccess_t volatile *)(baseAddress + Shared::Register::MemoryAccess)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::MemoryAccess_t value)
    {
        *((Shared::MemoryAccess_t volatile *)(baseAddress + Shared::Register::MemoryAccessCommit)) = value;
    }
}

}}}}