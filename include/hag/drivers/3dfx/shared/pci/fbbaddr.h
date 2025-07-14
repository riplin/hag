//Copyright 2025-Present riplin

#pragma once

#include <sys/nearptr.h>
#include <has/system/pci.h>

namespace Hag::TDfx::Shared::PCI
{

namespace Register
{
    enum
    {
        FrameBufferBaseAddress = Has::System::PCI::Header0::BaseAddress1
    };
}

typedef uint32_t FrameBufferBaseAddress_t;
namespace FrameBufferBaseAddress
{

    enum
    {
        Address = 0xffffff00,
    };


    inline FrameBufferBaseAddress_t Read(Has::System::PCI::Device_t device)
    {
        return FrameBufferBaseAddress_t(Has::System::PCI::Read32(device, Register::FrameBufferBaseAddress));
    }

    template <typename T>
    T* GetBaseAddressAs(Has::System::PCI::Device_t device)
    {
        return (T*)((Read(device) & Address) + __djgpp_conventional_base);
    }
}

}