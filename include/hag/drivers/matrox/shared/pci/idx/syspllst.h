//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/syspllst.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace SystemPLLStatus
{
    inline Shared::Indexed::SystemPLLStatus_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::SystemPLLStatus);
    }
}

}