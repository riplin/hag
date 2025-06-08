//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI
{

namespace Register
{
    
enum
{
    ControlAperture = 0x10
};

}

typedef System::PCI::Register_t ControlAperture_t;

namespace ControlAperture
{
    enum //Mask
    {
        MemorySpace = 0x00000001,           // RO The hard coded ‘0’ indicates that the map is in the memory space.
        Type = 0x00000006,                  // RO The hard coded ‘00’instructs the configuration program to locate the aperture
                                            // anywhere within the 32-bit address space.
        Prefetchable = 0x00000008,          // RO The hard coded ‘0’ indicates that this space is not prefetchable.
        BaseAddress = 0xFFFFC000,           // Specifies the base address of the MGA memory mapped control registers (16 KByte
                                            // control aperture).
                                            // In situations where the MGA control aperture overlaps the MGA frame buffer aper-
                                            // ture and/or the ROM aperture, the following precedence order will be used, listed
                                            // from highest to lowest:
                                            // 1. BIOS EPROM (highest precedence)
                                            // 2. MGA control aperture
                                            // 3. 8 MByte Pseudo-DMA window
                                            // 4. VGA frame buffer aperture
                                            // 5. MGA frame buffer aperture (lowest precedence)
    };

    namespace Shift
    {
        enum
        {
            MemorySpace = 0x00,
            Type = 0x01,
            Prefetchable = 0x03,
            BaseAddress = 0x0E
        };
    }

    inline ControlAperture_t Read(System::PCI::Device_t device)
    {
        return ControlAperture_t(System::PCI::Read32(device, Register::ControlAperture));
    }
    inline void Write(System::PCI::Device_t device, ControlAperture_t value)
    {
        System::PCI::Write32(device, Register::ControlAperture, value);
    }

    inline ControlAperture_t GetAddress(System::PCI::Device_t device)
    {
        return Read(device) & BaseAddress;
    }
}

}}}}