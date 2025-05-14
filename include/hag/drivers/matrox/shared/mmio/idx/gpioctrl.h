//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/gpioctrl.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace GeneralPurposeIOControl
{
    inline Shared::Indexed::GeneralPurposeIOControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::GeneralPurposeIOControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::GeneralPurposeIOControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::GeneralPurposeIOControl, value);
    }
}

}}}}}