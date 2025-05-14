//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/pixplln.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indexed
{

namespace PixelPLLN
{
    inline Shared::Indexed::PixelPLLN_t ReadA(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLNA);
    }
    inline Shared::Indexed::PixelPLLN_t ReadB(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLNB);
    }
    inline Shared::Indexed::PixelPLLN_t ReadC(System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::PixelPLLNC);
    }
    inline void WriteA(System::PCI::Device_t device, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLNA, value);
    }
    inline void WriteB(System::PCI::Device_t device, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLNB, value);
    }
    inline void WriteC(System::PCI::Device_t device, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::PixelPLLNC, value);
    }
}

}}}}}