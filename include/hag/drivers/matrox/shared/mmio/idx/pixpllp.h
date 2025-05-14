//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/pixpllp.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace PixelPLLP
{
    inline Shared::Indexed::PixelPLLP_t ReadA(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLPA);
    }
    inline Shared::Indexed::PixelPLLP_t ReadB(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLPB);
    }
    inline Shared::Indexed::PixelPLLP_t ReadC(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::PixelPLLPC);
    }
    inline void WriteA(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLPA, value);
    }
    inline void WriteB(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLPB, value);
    }
    inline void WriteC(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::PixelPLLP_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::PixelPLLPC, value);
    }
}

}}}}}