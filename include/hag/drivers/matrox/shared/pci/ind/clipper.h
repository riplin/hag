//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/cxbound.h>
#include <hag/drivers/matrox/shared/regs/cytop.h>
#include <hag/drivers/matrox/shared/regs/cybot.h>
#include <hag/drivers/matrox/shared/regs/cxlft.h>
#include <hag/drivers/matrox/shared/regs/cxrgt.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag::Matrox::Shared::PCI::Indirect
{

namespace Clipper
{
    inline void WriteXBoundary(System::PCI::Device_t device, Shared::ClipperXBoundary_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ClipperXBoundary);
        IndirectData::Write(device, value);
    }
    inline void WriteXLeft(System::PCI::Device_t device, Shared::ClipperXLeftBoundary_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ClipperXLeftBoundary);
        IndirectData::Write(device, value);
    }
    inline void WriteXRight(System::PCI::Device_t device, Shared::ClipperXRightBoundary_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ClipperXRightBoundary);
        IndirectData::Write(device, value);
    }
    inline void WriteYTop(System::PCI::Device_t device, Shared::ClipperYTopBoundary_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ClipperYTopBoundary);
        IndirectData::Write(device, value);
    }
    inline void WriteYBottom(System::PCI::Device_t device, Shared::ClipperYBottomBoundary_t value)
    {
        IndirectIndex::Write(device, Shared::Register::ClipperYBottomBoundary);
        IndirectData::Write(device, value);
    }
}

}