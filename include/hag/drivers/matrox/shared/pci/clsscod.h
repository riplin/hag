//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    ClassCode = 0x08
};

}

typedef System::PCI::Register_t ClassCode_t;

namespace ClassCode
{
    enum //Mask
    {
        Revision = 0x000000FF,                      // Holds the current chip revision
        Class = 0xFFFFFF00,                         // Identifies the generic function of the device and a specific register-level programming
                                                    // interface as per the PCI specification. Two values can be read in this field according to
                                                    // the vgaboot strap, which is sampled on hard reset.
        NonSuperVGADisplayController = 0x03800000,
        SuperVGADisplayController = 0x03000000,
    };

    namespace Shift
    {
        enum
        {
            Revision = 0x00,
            Class = 0x08,
        };
    }

    inline ClassCode_t Read(System::PCI::Device_t device) { return System::PCI::Read32(device, Register::ClassCode); }
}

}