//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/blcolkey.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace BlitColorKey
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::BlitColorKey_t value)
    {
        *((Shared::BlitColorKey_t volatile *)(baseAddress + Shared::Register::BlitColorKey)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::BlitColorKey_t value)
    {
        *((Shared::BlitColorKey_t volatile *)(baseAddress + Shared::Register::BlitColorKeyCommit)) = value;
    }
}

}}}}