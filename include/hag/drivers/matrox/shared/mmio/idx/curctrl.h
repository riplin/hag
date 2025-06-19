//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/curctrl.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace CursorControl
{
    inline Shared::Indexed::CursorControl_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorControl);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorControl_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorControl, value);
    }
}

}