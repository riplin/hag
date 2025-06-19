//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/curctrl.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace CursorControl
{
    inline Shared::Indexed::CursorControl_t Read(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorControl);
    }
    inline void Write(System::PCI::Device_t device, Shared::Indexed::CursorControl_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorControl, value);
    }
}

}