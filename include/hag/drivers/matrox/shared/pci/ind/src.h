//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/src.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace Source
{
    inline void Write0(System::PCI::Device_t device, Shared::Source_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Source0);
        IndirectData::Write(device, value);
    }
    inline void Write1(System::PCI::Device_t device, Shared::Source_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Source1);
        IndirectData::Write(device, value);
    }
    inline void Write2(System::PCI::Device_t device, Shared::Source_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Source2);
        IndirectData::Write(device, value);
    }
    inline void Write3(System::PCI::Device_t device, Shared::Source_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Source3);
        IndirectData::Write(device, value);
    }
}

}}}}}