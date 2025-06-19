//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/xyaddr.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace XYAddress
{
    inline void WriteStart(System::PCI::Device_t device, Shared::XYAddress_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XYStartAddress);
        IndirectData::Write(device, value);
    }
    inline void WriteEnd(System::PCI::Device_t device, Shared::XYAddress_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XYEndAddress);
        IndirectData::Write(device, value);
    }
    inline void WriteLeftRight(System::PCI::Device_t device, Shared::XLeftRight_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XLeftRight);
        IndirectData::Write(device, value);
    }
    inline void WriteLeft(System::PCI::Device_t device, Shared::XLeft_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XLeft);
        IndirectData::Write(device, value);
    }
    inline void WriteRight(System::PCI::Device_t device, Shared::XRight_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XRight);
        IndirectData::Write(device, value);
    }
    inline void WriteXDestination(System::PCI::Device_t device, Shared::XDestination_t value)
    {
        IndirectIndex::Write(device, Shared::Register::XDestination);
        IndirectData::Write(device, value);
    }
    inline void WriteYDestinationAndLength(System::PCI::Device_t device, Shared::YDestinationAndLength_t value)
    {
        IndirectIndex::Write(device, Shared::Register::YDestinationAndLength);
        IndirectData::Write(device, value);
    }
    inline void WriteYDestination(System::PCI::Device_t device, Shared::YDestination_t value)
    {
        IndirectIndex::Write(device, Shared::Register::YDestination);
        IndirectData::Write(device, value);
    }
}

}