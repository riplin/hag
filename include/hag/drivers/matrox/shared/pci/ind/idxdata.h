//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{

enum
{
    IndexedData = 0x3C0A
};

}

typedef uint8_t IndexedData_t;

namespace IndexedData
{
    inline IndirectData_t Read(Has::System::PCI::Device_t device, Hag::VGA::DACWriteIndex_t index)
    {
        VGA::DACWriteIndex::Write(index);
        IndirectIndex::Write(device, Register::IndexedData);
        return IndirectData::ReadByte2(device);
    }
    
    inline void Write(Has::System::PCI::Device_t device, Hag::VGA::DACWriteIndex_t index, IndexedData_t value)
    {
        VGA::DACWriteIndex::Write(index);
        IndirectIndex::Write(device, Register::IndexedData);
        IndirectData::WriteByte2(device, value);
    }
}

}