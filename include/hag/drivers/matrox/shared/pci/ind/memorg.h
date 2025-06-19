//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/memorg.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace MemoryOrigin
{
    inline void Write(System::PCI::Device_t device, Shared::MemoryOrigin_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MemoryOrigin);
        IndirectData::Write(device, value);
    }
}

}