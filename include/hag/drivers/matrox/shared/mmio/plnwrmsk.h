//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/memacc.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace PlaneWriteMask
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::PlaneWriteMask_t value)
    {
        *((Shared::PlaneWriteMask_t volatile *)(baseAddress + Shared::Register::PlaneWriteMask)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::PlaneWriteMask_t value)
    {
        *((Shared::PlaneWriteMask_t volatile *)(baseAddress + Shared::Register::PlaneWriteMaskCommit)) = value;
    }
}

}}}}