//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/pclkctrl.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace PixelClockControl
{

    inline Shared::Indexed::PixelClockControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelClockControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelClockControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelClockControl, value);
    }
}

}}}}}