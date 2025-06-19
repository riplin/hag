//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/genctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace GeneralControl
{
    inline Shared::Indexed::GeneralControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::GeneralControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::GeneralControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::GeneralControl, value);
    }
}

}