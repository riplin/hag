//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    DeviceIdentification = 0x00
};

}

typedef Has::System::PCI::Register_t DeviceIdentification_t;

namespace DeviceIdentification
{
    enum //Mask
    {
        VendorID = 0x0000FFFF,         //Vendor ID
        DeviceID = 0xFFFF0000          //Device ID
    };

    namespace Shift
    {
        enum
        {
            VendorID = 0x00,
            DeviceID = 0x10
        };
    }

    inline DeviceIdentification_t Read(Has::System::PCI::Device_t device) { return Has::System::PCI::Read32(device, Register::DeviceIdentification); }
}

}