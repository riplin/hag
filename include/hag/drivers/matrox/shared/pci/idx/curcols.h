//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/curcols.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace CursorColor
{

    inline Shared::Indexed::CursorColor_t ReadRed0(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor0Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen0(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor0Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue0(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor0Blue);
    }
    inline Shared::Indexed::CursorColor_t ReadRed1(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor1Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen1(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor1Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue1(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor1Blue);
    }
    inline Shared::Indexed::CursorColor_t ReadRed2(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor2Red);
    }
    inline Shared::Indexed::CursorColor_t ReadGreen2(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor2Green);
    }
    inline Shared::Indexed::CursorColor_t ReadBlue2(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::CursorColor2Blue);
    }

    inline void WriteRed0(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor0Red, value);
    }
    inline void WriteGreen0(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor0Green, value);
    }
    inline void WriteBlue0(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor0Blue, value);
    }
    inline void WriteRed1(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor1Red, value);
    }
    inline void WriteGreen1(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor1Green, value);
    }
    inline void WriteBlue1(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor1Blue, value);
    }
    inline void WriteRed2(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor2Red, value);
    }
    inline void WriteGreen2(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor2Green, value);
    }
    inline void WriteBlue2(Has::System::PCI::Device_t device, Shared::Indexed::CursorColor_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::CursorColor2Blue, value);
    }
}

}