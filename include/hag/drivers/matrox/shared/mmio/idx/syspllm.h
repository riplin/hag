//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/syspllm.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace SystemPLLM
{
    inline Shared::Indexed::SystemPLLM_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::SystemPLLM);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::SystemPLLM_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::SystemPLLM, value);
    }
}

}}}}}