//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag::Matrox::Shared::PCI
{

namespace Register
{
    
enum
{
    InterruptControl = 0x3C
};

}

typedef System::PCI::Register_t InterruptControl_t;

namespace InterruptControl
{
    enum //Mask
    {
        LineRouting = 0x000000FF,           // Interrupt line routing. The field is read/writable and reset to FFh upon hard reset. It is
                                            // up to the configuration program to determine which interrupt level is tied to the MGA
                                            // interrupt line and program the intline field accordingly (Note: the value ‘FF’ indicates
                                            // either ‘unknown’ or ‘no connection’).
        SelectedPins = 0x0000FF00,          // Selected interrupt pins. Read as 1h to indicate that one PCI interrupt line is used (PCI
                                            // specifies that if there is one interrupt line, it must be connected to the PINTA/ signal).
        MinBurstLength = 0x00FF0000,        // This field specifies the PCI device’s required burst length, assuming a clock rate of 33 MHz.
                                            // Values of ‘0’ indicate that the PCI device (the MGA-1064SG board) has no major
                                            // requirements for setting the latency timer.
        MaxLatency = 0xFF000000,            // This field specifies how often the PCI device must gain access to the PCI bus.
                                            // Values of ‘0’ indicate that the PCI device (the MGA-1064SG board) has no major
                                            // requirements for setting the latency timer.
    };

    namespace Shift
    {
        enum
        {
            LineRouting = 0x00,
            SelectedPins = 0x08,
            MinBurstLength = 0x10,
            MaxLatency = 0x18
        };
    }

    inline InterruptControl_t Read(System::PCI::Device_t device) { return InterruptControl_t(System::PCI::Read32(device, Register::InterruptControl)); }
    inline void Write(System::PCI::Device_t device, InterruptControl_t value) { System::PCI::Write32(device, Register::InterruptControl, value); }
}

}