//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/ptrn.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace Pattern
{
    inline void Write0(Has::System::PCI::Device_t device, Shared::Pattern_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Pattern0);
        IndirectData::Write(device, value);
    }
    inline void Write1(Has::System::PCI::Device_t device, Shared::Pattern_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Pattern1);
        IndirectData::Write(device, value);
    }
}

}