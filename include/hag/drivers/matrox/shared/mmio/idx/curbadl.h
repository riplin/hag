//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/curbadl.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace CursorBaseAddressLow
{
    inline Shared::Indexed::CursorBaseAddressLow_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorBaseAddressLow);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorBaseAddressLow_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorBaseAddressLow, value);
    }
}

}}}}}