//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/syspllp.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace SystemPLLP
{
    inline Shared::Indexed::SystemPLLP_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::SystemPLLP);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::SystemPLLP_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::SystemPLLP, value);
    }
}

}}}}}