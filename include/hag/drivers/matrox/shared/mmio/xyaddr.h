//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/xyaddr.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace XYAddress
{
    inline void WriteStart(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYStartAddress)) = value;
    }
    inline void WriteEnd(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYEndAddress)) = value;
    }
    inline void WriteLeftRight(Shared::PCI::ControlAperture_t baseAddress, Shared::XLeftRight_t value)
    {
        *((Shared::XLeftRight_t volatile *)(baseAddress + Shared::Register::XLeftRight)) = value;       
    }
    inline void WriteLeft(Shared::PCI::ControlAperture_t baseAddress, Shared::XLeft_t value)
    {
        *((Shared::XLeft_t volatile *)(baseAddress + Shared::Register::XLeft)) = value;
    }
    inline void WriteRight(Shared::PCI::ControlAperture_t baseAddress, Shared::XRight_t value)
    {
        *((Shared::XRight_t volatile *)(baseAddress + Shared::Register::XRight)) = value;
    }
    inline void WriteXDestination(Shared::PCI::ControlAperture_t baseAddress, Shared::XDestination_t value)
    {
        *((Shared::XDestination_t volatile *)(baseAddress + Shared::Register::XDestination)) = value;
    }
    inline void WriteYDestinationAndLength(Shared::PCI::ControlAperture_t baseAddress, Shared::YDestinationAndLength_t value)
    {
        *((Shared::YDestinationAndLength_t volatile *)(baseAddress + Shared::Register::YDestinationAndLength)) = value;
    }
    inline void WriteYDestination(Shared::PCI::ControlAperture_t baseAddress, Shared::YDestination_t value)
    {
        *((Shared::YDestination_t volatile *)(baseAddress + Shared::Register::YDestination)) = value;
    }
    inline void WriteStartCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYStartAddressCommit)) = value;
    }
    inline void WriteEndCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYEndAddressCommit)) = value;
    }
    inline void WriteLeftRightCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XLeftRight_t value)
    {
        *((Shared::XLeftRight_t volatile *)(baseAddress + Shared::Register::XLeftRightCommit)) = value;       
    }
    inline void WriteLeftCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XLeft_t value)
    {
        *((uint32_t volatile *)(baseAddress + Shared::Register::XLeftCommit)) = value;
    }
    inline void WriteRightCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XRight_t value)
    {
        *((uint32_t volatile *)(baseAddress + Shared::Register::XRightCommit)) = value;
    }
    inline void WriteXDestinationCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XDestination_t value)
    {
        *((uint32_t volatile *)(baseAddress + Shared::Register::XDestinationCommit)) = value;
    }
    inline void WriteYDestinationAndLengthCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::YDestinationAndLength_t value)
    {
        *((Shared::YDestinationAndLength_t volatile *)(baseAddress + Shared::Register::YDestinationAndLengthCommit)) = value;
    }
    inline void WriteYDestinationCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::YDestination_t value)
    {
        *((Shared::YDestination_t volatile *)(baseAddress + Shared::Register::YDestinationCommit)) = value;
    }
}

}}}}