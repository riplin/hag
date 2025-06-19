//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/sysplln.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace SystemPLLN
{
    inline Shared::Indexed::SystemPLLN_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::SystemPLLN);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::SystemPLLN_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::SystemPLLN, value);
    }
}

}