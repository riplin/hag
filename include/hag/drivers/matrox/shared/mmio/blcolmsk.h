//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/blcolmsk.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace BlitColorMask
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::BlitColorMask_t value)
    {
        *((Shared::BlitColorMask_t volatile *)(baseAddress + Shared::Register::BlitColorMask)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::BlitColorMask_t value)
    {
        *((Shared::BlitColorMask_t volatile *)(baseAddress + Shared::Register::BlitColorMaskCommit)) = value;
    }
}

}