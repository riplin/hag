//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/sign.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace Sign
{
    inline void Write(System::PCI::Device_t device, Shared::Sign_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Sign);
        IndirectData::Write(device, value);
    }
}

}}}}}