//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memorg.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
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

}}}}