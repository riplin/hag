//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/pixpllm.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace PixelPLLM
{
    inline Shared::Indexed::PixelPLLM_t ReadA(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLMA);
    }
    inline Shared::Indexed::PixelPLLM_t ReadB(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLMB);
    }
    inline Shared::Indexed::PixelPLLM_t ReadC(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLMC);
    }
    inline void WriteA(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLMA, value);
    }
    inline void WriteB(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLMB, value);
    }
    inline void WriteC(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLMC, value);
    }
}

}}}}}