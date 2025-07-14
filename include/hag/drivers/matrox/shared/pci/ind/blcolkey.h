//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/blcolkey.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace BlitColorKey
{
    inline void Write(Has::System::PCI::Device_t device, Shared::BlitColorKey_t value)
    {
        IndirectIndex::Write(device, Shared::Register::BlitColorKey);
        IndirectData::Write(device, value);
    }
}

}