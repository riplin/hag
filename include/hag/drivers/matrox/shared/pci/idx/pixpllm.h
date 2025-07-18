//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/pixpllm.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace PixelPLLM
{
    inline Shared::Indexed::PixelPLLM_t ReadA(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLMA);
    }
    inline Shared::Indexed::PixelPLLM_t ReadB(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLMB);
    }
    inline Shared::Indexed::PixelPLLM_t ReadC(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLMC);
    }
    inline void WriteA(Has::System::PCI::Device_t device, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLMA, value);
    }
    inline void WriteB(Has::System::PCI::Device_t device, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLMB, value);
    }
    inline void WriteC(Has::System::PCI::Device_t device, Shared::Indexed::PixelPLLM_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLMC, value);
    }
}

}