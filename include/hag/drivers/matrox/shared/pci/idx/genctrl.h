//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/genctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indexed
{

namespace GeneralControl
{
    inline Shared::Indexed::GeneralControl_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::GeneralControl);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::GeneralControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::GeneralControl, value);
    }
}

}}}}}