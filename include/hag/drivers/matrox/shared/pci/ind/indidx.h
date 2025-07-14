//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    IndirectIndex = 0x44
};

}

typedef Has::System::PCI::Register_t IndirectIndex_t;

namespace IndirectIndex
{
    enum //Mask
    {
        Index = 0x00003FFC          // Dword index. Used to reach any of the registers that are mapped into the MGA control
                                    // aperture through the configuration space. This mechanism should be used for initial-
                                    // ization purposes only, since it is inefficient. This ‘back door’ access to the control reg-
                                    // ister can be useful when the control aperture cannot be mapped below the 1 MByte
                                    // limit of the real mode of an x86 processor (during BIOS execution, for example).
    };

    namespace Shift
    {
        enum
        {
            Index = 0x02
        };
    }

    inline IndirectIndex_t Read(Has::System::PCI::Device_t device) { return Has::System::PCI::Read32(device, Register::IndirectIndex); }
    inline void Write(Has::System::PCI::Device_t device, IndirectIndex_t value) { Has::System::PCI::Write32(device, Register::IndirectIndex, value); }
}

}