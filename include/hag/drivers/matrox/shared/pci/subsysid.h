//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    SubsystemIDR = 0x2C,
    SubsystemIDW = 0x4C
};

}

typedef System::PCI::Register_t SubsystemID_t;

namespace SubsystemID
{
    enum //Mask
    {
        VendorID = 0x0000FFFF,      // Subsystem vendor ID. This field is reset with the value that is found in word location
                                    // 7FF8h of the BIOS ROM (32K ROM used), or at word location FFF8h of the BIOS
                                    // ROM (64K ROM used). It indicates a subsystem vendor ID as provided by the PCI
                                    // Special Interest Group to the manufacturer of the add-in board which contains the
                                    // MGA-1064SG chip.
        SubsystemID = 0xFFFF0000    // Subsystem ID. This field is reset with the value that is found in word location 7FFAh
                                    // of the BIOS ROM (32K ROM used), or at word location FFFAh of the BIOS ROM
                                    // (64K ROM used). It indicates a subsystem ID as determined by the manufacturer of
                                    // the add-in board which contains the MGA-1064SG chip.
                                    // Note: If the biosen strap is ‘0’, the ROM will not be read and the value found in the
                                    // register will be 00000000h. In this case, the driver must write the correct values to
                                    // this register after power-up.
                                    // Note: This register must contain all zeros if the manufacturer of the add-in board does
                                    // not have a subsystem vendor ID, or if the manufacturer does not wish to support the
                                    // SUBSYSID register.
                                    // Note: There may be a delay of up to 500 PCLKs following a hard reset before this reg-
                                    // ister is initialized.
    };

    namespace Shift
    {
        enum
        {
            VendorID = 0x00,
            SubsystemID = 0x10
        };
    }

    inline SubsystemID_t Read(System::PCI::Device_t device) { return SubsystemID_t(System::PCI::Read32(device, Register::SubsystemIDR)); }
    inline void Write(System::PCI::Device_t device, SubsystemID_t value) { System::PCI::Write32(device, Register::SubsystemIDW, value); }
}

}