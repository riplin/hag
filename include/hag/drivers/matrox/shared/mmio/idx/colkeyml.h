//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/colkeyml.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace ColorKeyMaskLow
{
    inline Shared::Indexed::ColorKeyMaskLow_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::ColorKeyMaskLow);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::ColorKeyMaskLow_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::ColorKeyMaskLow, value);
    }
}

}