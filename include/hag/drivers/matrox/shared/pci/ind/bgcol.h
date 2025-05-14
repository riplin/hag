//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/bgcol.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace BackgroundColor
{
    inline void Write(System::PCI::Device_t device, Shared::BackgroundColor_t value)
    {
        IndirectIndex::Write(device, Shared::Register::BackgroundColor);
        IndirectData::Write(device, value);
    }
}

}}}}}