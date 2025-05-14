//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/crcremh.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indexed
{

namespace CRCRemainderHigh
{
    inline Shared::Indexed::CRCRemainderHigh_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CRCRemainderHigh);
    }
}

}}}}}