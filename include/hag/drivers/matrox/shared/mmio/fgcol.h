//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/fgcol.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace ForegroundColor
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::ForegroundColor_t value)
    {
        *((Shared::ForegroundColor_t volatile *)(baseAddress + Shared::Register::ForegroundColor)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ForegroundColor_t value)
    {
        *((Shared::ForegroundColor_t volatile *)(baseAddress + Shared::Register::ForegroundColorCommit)) = value;
    }
}

}