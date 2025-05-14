//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/memctlws.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace PlaneWriteMask
{
    inline void Write(System::PCI::Device_t device, Shared::PlaneWriteMask_t value)
    {
        IndirectIndex::Write(device, Shared::Register::PlaneWriteMask);
        IndirectData::Write(device, value);
    }
}

}}}}}