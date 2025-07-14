//Copyright 2025-Present riplin

#pragma once

#include <has/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    Header = 0x0C
};

}

typedef Has::System::PCI::Register_t Header_t;

namespace Header
{
    enum //Mask
    {
        LatencyTimer = 0x0000FF00,  // Value of the latency timer in PCI clocks. The count starts when PFRAME/ is asserted.
                                    // Once the count expires, the master must initiate transaction termination as soon as its
                                    // PGNT/ signal is removed.
        Header = 0x00FF0000         // This field specifies the layout of bytes 10h through 3Fh in the configuration space and
                                    // also indicates that the current device is a single function device. This field is read as
                                    // 00h.
    };

    namespace Shift
    {
        enum
        {
            LatencyTimer = 0x08,
            Header = 0x10
        };
    }

    inline Header_t Read(Has::System::PCI::Device_t device) { return Has::System::PCI::Read32(device, Register::Header); }
    inline void Write(Has::System::PCI::Device_t device, Header_t value) { Has::System::PCI::Write32(device, Register::Header, value); }
}

}