//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/mpaddr0.h>
#include <hag/drivers/matrox/shared/regs/mpaddr1.h>
#include <hag/drivers/matrox/shared/regs/mpaddr2.h>
#include <hag/drivers/matrox/shared/regs/mpaddr3.h>
#include <hag/drivers/matrox/shared/regs/mpaddr4.h>
#include <hag/drivers/matrox/shared/regs/mpaddr5.h>
#include <hag/drivers/matrox/shared/regs/mpaddr6.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace MultiPurposeAddress
{
    inline void Write0(System::PCI::Device_t device, Shared::MultiPurposeAddress0_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress0);
        IndirectData::Write(device, value);
    }
    inline void Write1(System::PCI::Device_t device, Shared::MultiPurposeAddress1_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress1);
        IndirectData::Write(device, value);
    }
    inline void Write2(System::PCI::Device_t device, Shared::MultiPurposeAddress2_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress2);
        IndirectData::Write(device, value);
    }
    inline void Write3(System::PCI::Device_t device, Shared::MultiPurposeAddress3_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress3);
        IndirectData::Write(device, value);
    }
    inline void Write4(System::PCI::Device_t device, Shared::MultiPurposeAddress4_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress4);
        IndirectData::Write(device, value);
    }
    inline void Write5(System::PCI::Device_t device, Shared::MultiPurposeAddress5_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress5);
        IndirectData::Write(device, value);
    }
    inline void Write6(System::PCI::Device_t device, Shared::MultiPurposeAddress6_t value)
    {
        IndirectIndex::Write(device, Shared::Register::MultiPurposeAddress6);
        IndirectData::Write(device, value);
    }
}

}