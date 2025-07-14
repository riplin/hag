//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    RomBaseAddress = 0x30
};

}

typedef Has::System::PCI::Register_t RomBaseAddress_t;

namespace RomBaseAddress
{
    enum //Mask
    {
        Enable = 0x00000001,        // ROM enable. This field can assume different attributes, depending on the contents of
                                    // the biosen field. This allows booting with or without the BIOS EPROM (typically, a
                                    // motherboard implementation will boot the MGA without the BIOS, while an add-on
                                    // adapter will boot the MGA with the BIOS EPROM).
                                    //  biosen      romen(Enable)
                                    //    0           RO (read as 0)
                                    //    1           R/W
        BaseAddress = 0xFFFF0000    // ROM base address. Specifies the base address of the EPROM. This field can assume
                                    // different attributes, depending on the contents of biosen.
                                    //  biosen      rombase(BaseAddres)
                                    //    0            RO (read as 0)
                                    //    1            R/W
                                    // Note: the exact size of the EPROM used is application-specific (could be 32K or 64K).
                                    // In situations where the MGA control aperture overlaps the MGA frame buffer aperture
                                    // and/or the ROM aperture, the following precedence order will be used, listed
                                    // from highest to lowest:
                                    // 1. BIOS EPROM (highest precedence)
                                    // 2. MGA control aperture
                                    // 3. 8 MByte Pseudo-DMA window
                                    // 4. VGA frame buffer aperture
                                    // 5. MGA frame buffer aperture (lowest precedence)
                                    // Even if MGA supports only an 8-bit-wide EPROM, this does not constitute a system
                                    // performance limitation, since the PCI specification requires the configuration soft-
                                    // ware to move the EPROM contents into shadow memory and execute the code at that
                                    // location.
    };

    namespace Shift
    {
        enum
        {
            Enable = 0x00,
            BaseAddress = 0x10
        };
    }

    inline RomBaseAddress_t Read(Has::System::PCI::Device_t device) { return RomBaseAddress_t(Has::System::PCI::Read32(device, Register::RomBaseAddress)); }
    inline void Write(Has::System::PCI::Device_t device, RomBaseAddress_t value) { Has::System::PCI::Write32(device, Register::RomBaseAddress, value); }
}

}