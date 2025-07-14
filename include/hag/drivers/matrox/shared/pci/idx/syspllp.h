//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/syspllp.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace SystemPLLP
{
    inline Shared::Indexed::SystemPLLP_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::SystemPLLP);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::SystemPLLP_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::SystemPLLP, value);
    }
}

}