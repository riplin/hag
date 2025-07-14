//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/fgcol.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace ForegroundColor
{
    inline void Write(Has::System::PCI::Device_t device, Shared::ForegroundColor_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ForegroundColor);
        IndirectData::Write(device, value);
    }
}

}