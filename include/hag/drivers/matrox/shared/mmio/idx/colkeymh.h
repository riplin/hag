//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/colkeymh.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace ColorKeyMaskHigh
{
    inline Shared::Indexed::ColorKeyMaskHigh_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::ColorKeyMaskHigh);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::ColorKeyMaskHigh_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::ColorKeyMaskHigh, value);
    }
}

}}}}}