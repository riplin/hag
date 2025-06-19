//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/mpaddr0.h>
#include <hag/drivers/matrox/shared/regs/mpaddr1.h>
#include <hag/drivers/matrox/shared/regs/mpaddr2.h>
#include <hag/drivers/matrox/shared/regs/mpaddr3.h>
#include <hag/drivers/matrox/shared/regs/mpaddr4.h>
#include <hag/drivers/matrox/shared/regs/mpaddr5.h>
#include <hag/drivers/matrox/shared/regs/mpaddr6.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace MultiPurposeAddress
{
    inline void Write0(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress0_t value)
    {
        *((Shared::MultiPurposeAddress0_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress0)) = value;
    }
    inline void Write1(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress1_t value)
    {
        *((Shared::MultiPurposeAddress1_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress1)) = value;
    }
    inline void Write2(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress2_t value)
    {
        *((Shared::MultiPurposeAddress2_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress2)) = value;
    }
    inline void Write3(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress3_t value)
    {
        *((Shared::MultiPurposeAddress3_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress3)) = value;
    }
    inline void Write4(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress4_t value)
    {
        *((Shared::MultiPurposeAddress4_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress4)) = value;
    }
    inline void Write5(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress5_t value)
    {
        *((Shared::MultiPurposeAddress5_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress5)) = value;
    }
    inline void Write6(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress6_t value)
    {
        *((Shared::MultiPurposeAddress6_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress6)) = value;
    }
    inline void Write0Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress0_t value)
    {
        *((Shared::MultiPurposeAddress0_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress0Commit)) = value;
    }
    inline void Write1Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress1_t value)
    {
        *((Shared::MultiPurposeAddress1_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress1Commit)) = value;
    }
    inline void Write2Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress2_t value)
    {
        *((Shared::MultiPurposeAddress2_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress2Commit)) = value;
    }
    inline void Write3Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress3_t value)
    {
        *((Shared::MultiPurposeAddress3_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress3Commit)) = value;
    }
    inline void Write4Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress4_t value)
    {
        *((Shared::MultiPurposeAddress4_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress4Commit)) = value;
    }
    inline void Write5Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress5_t value)
    {
        *((Shared::MultiPurposeAddress5_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress5Commit)) = value;
    }
    inline void Write6Commit(Shared::PCI::ControlAperture_t baseAddress, Shared::MultiPurposeAddress6_t value)
    {
        *((Shared::MultiPurposeAddress6_t volatile *)(baseAddress + Shared::Register::MultiPurposeAddress6Commit)) = value;
    }
}

}