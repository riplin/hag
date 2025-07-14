//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/memacc.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace MemoryAccess
{
    inline void Write(Has::System::PCI::Device_t device, Shared::MemoryAccess_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MemoryAccess);
        IndirectData::Write(device, value);
    }
}

}