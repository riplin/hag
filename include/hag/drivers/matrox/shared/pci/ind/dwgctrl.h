//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/dwgctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace DrawingControl
{
    inline void Write(System::PCI::Device_t device, Shared::DrawingControl_t value)
    {
        IndirectIndex::Write(device, Shared::Register::DrawingControl);
        IndirectData::Write(device, value);
    }
}

}