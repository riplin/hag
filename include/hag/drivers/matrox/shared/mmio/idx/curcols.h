//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/curcols.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO { namespace Indexed
{

namespace CursorColor
{

    inline Shared::Indexed::CursorColor_t ReadRed0(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor0Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen0(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor0Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue0(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor0Blue);
    }
    inline Shared::Indexed::CursorColor_t ReadRed1(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor1Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen1(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor1Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue1(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor1Blue);
    }
    inline Shared::Indexed::CursorColor_t ReadRed2(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor2Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen2(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor2Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue2(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::CursorColor2Blue);
    }

    inline void WriteRed0(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor0Red, value);
    }
    inline void WriteGreen0(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor0Green, value);
    }
    inline void WriteBlue0(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor0Blue, value);
    }
    inline void WriteRed1(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor1Red, value);
    }
    inline void WriteGreen1(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor1Green, value);
    }
    inline void WriteBlue1(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor1Blue, value);
    }
    inline void WriteRed2(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor2Red, value);
    }
    inline void WriteGreen2(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor2Green, value);
    }
    inline void WriteBlue2(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::CursorColor2Blue, value);
    }
}

}}}}}