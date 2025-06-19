//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/miscctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace MiscellaneousControl
{
    inline Shared::Indexed::MiscellaneousControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::MiscellaneousControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::MiscellaneousControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::MiscellaneousControl, value);
    }
}

}