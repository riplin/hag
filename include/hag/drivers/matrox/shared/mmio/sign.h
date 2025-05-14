//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/sign.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace Sign
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::Sign_t value)
    {
        *((Shared::Sign_t volatile *)(baseAddress + Shared::Register::Sign)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::Sign_t value)
    {
        *((Shared::Sign_t volatile *)(baseAddress + Shared::Register::SignCommit)) = value;
    }
}

}}}}