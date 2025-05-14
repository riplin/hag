//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/src.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace Source
{
    inline void Write0(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source0)) = value;
    }
    inline void Write1(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source1)) = value;
    }
    inline void Write2(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source2)) = value;
    }
    inline void Write3(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source3)) = value;
    }
    inline void Write0Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source0Commit)) = value;
    }
    inline void Write1Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source1Commit)) = value;
    }
    inline void Write2Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source2Commit)) = value;
    }
    inline void Write3Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::Source_t value)
    {
        *((Shared::Source_t volatile *)(baseAddress + Shared::Register::Source3Commit)) = value;
    }
}

}}}}