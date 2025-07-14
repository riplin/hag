//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/sysplln.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace SystemPLLN
{
    inline Shared::Indexed::SystemPLLN_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::SystemPLLN);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::SystemPLLN_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::SystemPLLN, value);
    }
}

}