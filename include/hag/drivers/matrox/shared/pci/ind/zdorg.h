//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/zdorg.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace ZDepthOrigin
{
    inline void Write(System::PCI::Device_t device, Shared::ZDepthOrigin_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ZDepthOrigin);
        IndirectData::Write(device, value);
    }
}

}