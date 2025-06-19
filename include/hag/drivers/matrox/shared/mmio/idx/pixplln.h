//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/pixplln.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace PixelPLLN
{
    inline Shared::Indexed::PixelPLLN_t ReadA(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLNA);
    }
    inline Shared::Indexed::PixelPLLN_t ReadB(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLNB);
    }
    inline Shared::Indexed::PixelPLLN_t ReadC(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLNC);
    }
    inline void WriteA(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLNA, value);
    }
    inline void WriteB(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLNB, value);
    }
    inline void WriteC(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLN_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLNC, value);
    }
}

}