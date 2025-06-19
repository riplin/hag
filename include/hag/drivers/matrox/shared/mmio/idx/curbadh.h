//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/curbadh.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace CursorBaseAddressHigh
{
    inline Shared::Indexed::CursorBaseAddressHigh_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorBaseAddressHigh);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorBaseAddressHigh_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorBaseAddressHigh, value);
    }
}

}