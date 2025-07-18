//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/colkeyl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace ColorKeyLow
{
    inline Shared::Indexed::ColorKeyLow_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::ColorKeyLow);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::ColorKeyLow_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::ColorKeyLow, value);
    }
}

}