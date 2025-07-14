//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{

enum
{
    IndirectData = 0x48
};

}

typedef Has::System::PCI::Register_t IndirectData_t;
typedef Has::System::PCI::Register8_t IndirectData8_t;

namespace IndirectData
{
    enum //Mask
    {
        Data = 0xFFFFFFFF          // Data. Will read or write data at the control register address provided by MGA_INDEX.
    };

    namespace Shift
    {
        enum
        {
            Index = 0x00
        };
    }

    inline IndirectData_t Read(Has::System::PCI::Device_t device) { return Has::System::PCI::Read32(device, Register::IndirectData); }
    inline IndirectData8_t ReadByte0(Has::System::PCI::Device_t device) { return Has::System::PCI::Read8(device, Register::IndirectData + 0); }
    inline IndirectData8_t ReadByte1(Has::System::PCI::Device_t device) { return Has::System::PCI::Read8(device, Register::IndirectData + 1); }
    inline IndirectData8_t ReadByte2(Has::System::PCI::Device_t device) { return Has::System::PCI::Read8(device, Register::IndirectData + 2); }
    inline IndirectData8_t ReadByte3(Has::System::PCI::Device_t device) { return Has::System::PCI::Read8(device, Register::IndirectData + 3); }
    inline void Write(Has::System::PCI::Device_t device, IndirectData_t value) { Has::System::PCI::Write32(device, Register::IndirectData, value); }
    inline void WriteByte0(Has::System::PCI::Device_t device, IndirectData8_t value) { Has::System::PCI::Write8(device, Register::IndirectData + 0, value); }
    inline void WriteByte1(Has::System::PCI::Device_t device, IndirectData8_t value) { Has::System::PCI::Write8(device, Register::IndirectData + 1, value); }
    inline void WriteByte2(Has::System::PCI::Device_t device, IndirectData8_t value) { Has::System::PCI::Write8(device, Register::IndirectData + 2, value); }
    inline void WriteByte3(Has::System::PCI::Device_t device, IndirectData8_t value) { Has::System::PCI::Write8(device, Register::IndirectData + 3, value); }
}

}