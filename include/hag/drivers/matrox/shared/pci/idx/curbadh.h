//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/curbadh.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace CursorBaseAddressHigh
{
    inline Shared::Indexed::CursorBaseAddressHigh_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorBaseAddressHigh);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::CursorBaseAddressHigh_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorBaseAddressHigh, value);
    }
}

}