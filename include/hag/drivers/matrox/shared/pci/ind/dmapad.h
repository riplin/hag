//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/dmapad.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace DMAPadding
{
    inline void Write(System::PCI::Device_t device, Shared::DMAPadding_t value)
    {
        IndirectIndex::Write(device, Shared::Register::DMAPadding);
        IndirectData::Write(device, value);
    }
}

}