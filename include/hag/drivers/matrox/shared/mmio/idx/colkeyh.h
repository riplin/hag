//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/colkeyh.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace ColorKeyHigh
{
    inline Shared::Indexed::ColorKeyHigh_t Read(Shared::PCI::ControlAperture_t baseAddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::ColorKeyHigh);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::ColorKeyHigh_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::ColorKeyHigh, value);
    }
}

}