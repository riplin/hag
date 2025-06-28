//Copyright 2025-Present riplin

#pragma once

#include <sys/nearptr.h>
#include <hag/system/pci.h>

namespace Hag::TDfx::Shared::PCI
{

namespace Register
{
    enum
    {
        ControlBaseAddress = System::PCI::Header0::BaseAddress0
    };
}

typedef uint32_t ControlBaseAddress_t;
namespace ControlBaseAddress
{

    enum
    {
        Address = 0xffffff00,
    };


    inline ControlBaseAddress_t Read(System::PCI::Device_t device)
    {
        return ControlBaseAddress_t(System::PCI::Read32(device, Register::ControlBaseAddress));
    }

    template <typename T>
    T* GetBaseAddressAs(System::PCI::Device_t device)
    {
        return (T*)((Read(device) & Address) + __djgpp_conventional_base);
    }
}

}