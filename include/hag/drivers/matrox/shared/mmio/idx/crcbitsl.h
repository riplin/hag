//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/crcbitsl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace CRCBitSelect
{
    inline Shared::Indexed::CRCBitSelect_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CRCBitSelect);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CRCBitSelect_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CRCBitSelect, value);
    }
}

}