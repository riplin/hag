//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/dmapad.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace DMAPadding
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::DMAPadding_t value)
    {
        *((Shared::DMAPadding_t volatile *)(baseAddress + Shared::Register::DMAPadding)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::DMAPadding_t value)
    {
        *((Shared::DMAPadding_t volatile *)(baseAddress + Shared::Register::DMAPaddingCommit)) = value;
    }
}

}