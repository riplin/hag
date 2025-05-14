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
    inline void WriteStartCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYStartAddressCommit)) = value;
    }
    inline void WriteEndCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::XYAddress_t value)
    {
        *((Shared::XYAddress_t volatile *)(baseAddress + Shared::Register::XYEndAddressCommit)) = value;
    }
}

}}}}