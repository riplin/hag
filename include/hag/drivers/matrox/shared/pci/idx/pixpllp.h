//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/pixpllp.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace PixelPLLP
{
    inline Shared::Indexed::PixelPLLP_t ReadA(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLPA);
    }
    inline Shared::Indexed::PixelPLLP_t ReadB(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLPB);
    }
    inline Shared::Indexed::PixelPLLP_t ReadC(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLPC);
    }
    inline void WriteA(System::PCI::Device_t device, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLPA, value);
    }
    inline void WriteB(System::PCI::Device_t device, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLPB, value);
    }
    inline void WriteC(System::PCI::Device_t device, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLPC, value);
    }
}

}