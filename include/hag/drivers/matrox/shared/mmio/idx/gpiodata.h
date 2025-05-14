//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/gpiodata.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace GeneralPurposeIOData
{
    inline Shared::Indexed::GeneralPurposeIOData_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::GeneralPurposeIOData);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::GeneralPurposeIOData_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::GeneralPurposeIOData, value);
    }
}

}}}}}