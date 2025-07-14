//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::TDfx::Shared::PCI
{

namespace Register
{
    enum
    {
        IOBaseAddress = Has::System::PCI::Header0::BaseAddress2
    };
}

typedef uint32_t IOBaseAddress_t;
namespace IOBaseAddress
{

    enum
    {
        Address = 0x0000ff00,
    };


    inline IOBaseAddress_t Read(Has::System::PCI::Device_t device)
    {
        return IOBaseAddress_t(Has::System::PCI::Read32(device, Register::IOBaseAddress));
    }

    inline uint16_t GetBaseAddress(Has::System::PCI::Device_t device)
    {
        return uint16_t(Read(device) & Address);
    }

}

}