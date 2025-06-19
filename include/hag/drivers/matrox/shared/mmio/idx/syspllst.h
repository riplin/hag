//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/syspllst.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace SystemPLLStatus
{
    inline Shared::Indexed::SystemPLLStatus_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::SystemPLLStatus);
    }
}

}