//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/crcremh.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace CRCRemainderHigh
{
    inline Shared::Indexed::CRCRemainderHigh_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CRCRemainderHigh);
    }
}

}