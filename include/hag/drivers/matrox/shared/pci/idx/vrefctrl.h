//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/vrefctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indexed
{

namespace VoltageReferenceControl
{

    inline Shared::Indexed::VoltageReferenceControl_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::VoltageReferenceControl);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::VoltageReferenceControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::VoltageReferenceControl, value);
    }
}

}}}}}