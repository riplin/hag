//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/muxctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace MultiplexControl
{

    inline Shared::Indexed::MultiplexControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::MultiplexControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::MultiplexControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::MultiplexControl, value);
    }

}

}