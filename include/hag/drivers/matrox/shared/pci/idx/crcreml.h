//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/crcreml.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace CRCRemainderLow
{
    inline Shared::Indexed::CRCRemainderLow_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CRCRemainderLow);
    }
}

}