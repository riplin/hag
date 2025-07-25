//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/shift.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace FunnelShifterControl
{
    inline void Write(Has::System::PCI::Device_t device, Shared::FunnelShifterControl_t value)
    {
        IndirectIndex::Write(device, Shared::Register::FunnelShifterControl);
        IndirectData::Write(device, value);
    }
}

}