//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/xyaddr.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace XYAddress
{
    inline void WriteStart(System::PCI::Device_t device, Shared::XYAddress_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XYStartAddress);
        IndirectData::Write(device, value);
    }
    inline void WriteEnd(System::PCI::Device_t device, Shared::XYAddress_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XYEndAddress);
        IndirectData::Write(device, value);
    }
}

}}}}}