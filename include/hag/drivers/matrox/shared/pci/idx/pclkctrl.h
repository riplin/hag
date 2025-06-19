//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/pclkctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace PixelClockControl
{

    inline Shared::Indexed::PixelClockControl_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelClockControl);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::PixelClockControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelClockControl, value);
    }
}

}