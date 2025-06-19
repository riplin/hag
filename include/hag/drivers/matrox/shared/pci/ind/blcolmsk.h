//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/blcolmsk.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace BlitColorMask
{
    inline void Write(System::PCI::Device_t device, Shared::BlitColorMask_t value)
    {
        IndirectIndex::Write(device, Shared::Register::BlitColorMask);
        IndirectData::Write(device, value);
    }
}

}