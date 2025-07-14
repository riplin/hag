//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/sign.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace Sign
{
    inline void Write(Has::System::PCI::Device_t device, Shared::Sign_t value)
    {
        IndirectIndex::Write(device, Shared::Register::Sign);
        IndirectData::Write(device, value);
    }
}

}