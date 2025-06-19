//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/vrefctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace VoltageReferenceControl
{

    inline Shared::Indexed::VoltageReferenceControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::VoltageReferenceControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::VoltageReferenceControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::VoltageReferenceControl, value);
    }
}

}