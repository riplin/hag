//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/colkeyl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace ColorKeyLow
{
    inline Shared::Indexed::ColorKeyLow_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::ColorKeyLow);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::ColorKeyLow_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::ColorKeyLow, value);
    }
}

}