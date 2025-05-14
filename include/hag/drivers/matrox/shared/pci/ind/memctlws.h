//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/memctlws.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace MemoryControlWaitState
{
    inline void Write(System::PCI::Device_t device, Shared::MemoryControlWaitState_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MemoryControlWaitState);
        IndirectData::Write(device, value);
    }
}

}}}}}