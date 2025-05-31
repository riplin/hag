//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI
{

namespace Register
{
    
enum
{
    FrameBufferAperture = 0x14
};

}

typedef System::PCI::Register_t FrameBufferAperture_t;

namespace FrameBufferAperture
{
    enum //Mask
    {
        MemorySpace = 0x00000001,           // RO The hard coded ‘0’ indicates that the map is in the memory space.
        Type = 0x00000006,                  // RO The hard coded ‘00’ instructs the configuration program to locate the aperture
                                            // anywhere within the 32-bit address space.
        Prefetchable = 0x00000008,          // RO A ‘1’ indicates that this space can be prefetchable (better system performance can be
                                            // achieved when the bridge enables prefetching into that range).
                                            // The state of this field depends on the unimem strap, as shown below:
                                            // unimem   prefetchable
                                            //   0           1
                                            //   1           0
        BaseAddress = 0xFF800000,           // Specifies the PCI start address of the 8 MBytes of MGA memory space in the PCI map.
                                            // In situations where the MGA control aperture overlaps the MGA frame buffer aperture
                                            // and/or the ROM aperture, the following precedence order will be used, listed
                                            // from highest to lowest:
                                            // 1. BIOS EPROM (highest precedence)
                                            // 2. MGA control aperture
                                            // 3. 8 MByte Pseudo-DMA window
                                            // 4. VGA frame buffer aperture
                                            // 5. MGA frame buffer aperture (lowest precedence)
                                            // When mgamode = 0 (CRTCEXT3<7>), the full frame buffer aperture is not available.
    };

    namespace Shift
    {
        enum
        {
            MemorySpace = 0x00,
            Type = 0x01,
            Prefetchable = 0x03,
            BaseAddress = 0x17
        };
    }

    inline FrameBufferAperture_t Read(System::PCI::Device_t device)
    {
        return FrameBufferAperture_t(System::PCI::Read32(device, Register::FrameBufferAperture));
    }

    inline void Write(System::PCI::Device_t device, FrameBufferAperture_t value)
    {
        System::PCI::Write32(device, Register::FrameBufferAperture, value);
    }

    template<typename T>
    T* GetAddress(System::PCI::Device_t device)
    {
        return (T*)(Read(device) & BaseAddress);
    }
}

}}}}