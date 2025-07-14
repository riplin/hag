//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/curbadl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace CursorBaseAddressLow
{
    inline Shared::Indexed::CursorBaseAddressLow_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorBaseAddressLow);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::CursorBaseAddressLow_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorBaseAddressLow, value);
    }
}

}