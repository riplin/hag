//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/length.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace Length
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::Length_t value)
    {
        *((Shared::Length_t volatile *)(baseAddress + Shared::Register::Length)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::Length_t value)
    {
        *((Shared::Length_t volatile *)(baseAddress + Shared::Register::LengthCommit)) = value;
    }
}

}