//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/colkeyml.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace ColorKeyMaskLow
{
    inline Shared::Indexed::ColorKeyMaskLow_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::ColorKeyMaskLow);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::ColorKeyMaskLow_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::ColorKeyMaskLow, value);
    }
}

}