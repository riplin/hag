//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/length.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace Length
{
    inline void Write(Has::System::PCI::Device_t device, Shared::Length_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Length);
        IndirectData::Write(device, value);
    }
}

}