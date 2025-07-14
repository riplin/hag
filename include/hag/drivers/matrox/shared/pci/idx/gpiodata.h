//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/gpiodata.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace GeneralPurposeIOData
{
    inline Shared::Indexed::GeneralPurposeIOData_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::GeneralPurposeIOData);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::GeneralPurposeIOData_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::GeneralPurposeIOData, value);
    }
}

}