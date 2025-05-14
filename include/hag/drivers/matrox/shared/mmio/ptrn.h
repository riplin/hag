//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/ptrn.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace Pattern
{
    inline void Write0(Shared::PCI::ControlAperture_t baseAddress, Shared::Pattern_t value)
    {
        *((Shared::Pattern_t volatile *)(baseAddress + Shared::Register::Pattern0)) = value;
    }
    inline void Write1(Shared::PCI::ControlAperture_t baseAddress, Shared::Pattern_t value)
    {
        *((Shared::Pattern_t volatile *)(baseAddress + Shared::Register::Pattern1)) = value;
    }
    inline void Write0Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Pattern_t value)
    {
        *((Shared::Pattern_t volatile *)(baseAddress + Shared::Register::Pattern0Commit)) = value;
    }
    inline void Write1Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Pattern_t value)
    {
        *((Shared::Pattern_t volatile *)(baseAddress + Shared::Register::Pattern1Commit)) = value;
    }
}

}}}}