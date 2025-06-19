//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/shift.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace FunnelShifterControl
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::FunnelShifterControl_t value)
    {
        *((Shared::FunnelShifterControl_t volatile *)(baseAddress + Shared::Register::FunnelShifterControl)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::FunnelShifterControl_t value)
    {
        *((Shared::FunnelShifterControl_t volatile *)(baseAddress + Shared::Register::FunnelShifterControlCommit)) = value;
    }
}

}