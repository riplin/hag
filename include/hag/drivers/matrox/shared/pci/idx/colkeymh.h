//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/colkeymh.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace ColorKeyMaskHigh
{
    inline Shared::Indexed::ColorKeyMaskHigh_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::ColorKeyMaskHigh);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::ColorKeyMaskHigh_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::ColorKeyMaskHigh, value);
    }
}

}