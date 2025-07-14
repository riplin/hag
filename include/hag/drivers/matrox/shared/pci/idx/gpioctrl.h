//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/gpioctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace GeneralPurposeIOControl
{
    inline Shared::Indexed::GeneralPurposeIOControl_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::GeneralPurposeIOControl);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::GeneralPurposeIOControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::GeneralPurposeIOControl, value);
    }
}

}