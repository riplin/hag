//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/zoomctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace ZoomControl
{
    inline Shared::Indexed::ZoomControl_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::ZoomControl);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::ZoomControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::ZoomControl, value);
    }
}

}