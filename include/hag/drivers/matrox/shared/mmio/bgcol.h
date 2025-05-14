//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/bgcol.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace BackgroundColor
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::BackgroundColor_t value)
    {
        *((Shared::BackgroundColor_t volatile *)(baseAddress + Shared::Register::BackgroundColor)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::BackgroundColor_t value)
    {
        *((Shared::BackgroundColor_t volatile *)(baseAddress + Shared::Register::BackgroundColorCommit)) = value;
    }
}

}}}}