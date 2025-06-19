//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/zoomctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace ZoomControl
{
    inline Shared::Indexed::ZoomControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::ZoomControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::ZoomControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::ZoomControl, value);
    }
}

}