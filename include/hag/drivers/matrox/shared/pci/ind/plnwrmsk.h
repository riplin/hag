//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/memctlws.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace PlaneWriteMask
{
    inline void Write(Has::System::PCI::Device_t device, Shared::PlaneWriteMask_t value)
    {
        IndirectIndex::Write(device, Shared::Register::PlaneWriteMask);
        IndirectData::Write(device, value);
    }
}

}