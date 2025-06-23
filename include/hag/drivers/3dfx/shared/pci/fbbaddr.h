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
        FrameBufferBaseAddress = System::PCI::Header0::BaseAddress1
    };
}

typedef uint32_t FrameBufferBaseAddress_t;
namespace FrameBufferBaseAddress
{

    enum
    {
        Address = 0xffffff00,
    };


    inline FrameBufferBaseAddress_t Read(System::PCI::Device_t device)
    {
        return FrameBufferBaseAddress_t(System::PCI::Read32(device, Register::FrameBufferBaseAddress));
    }

    template <typename T>
    T* GetBaseAddressAs(System::PCI::Device_t device)
    {
        return (T*)((Read(device) & Address) + __djgpp_conventional_base);
    }
}

}