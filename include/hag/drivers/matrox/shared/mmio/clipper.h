//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/cxbound.h>
#include <hag/drivers/matrox/shared/regs/cxlft.h>
#include <hag/drivers/matrox/shared/regs/cxrgt.h>
#include <hag/drivers/matrox/shared/regs/cytop.h>
#include <hag/drivers/matrox/shared/regs/cybot.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace Clipper
{
    inline void WriteXBoundary(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXBoundary_t value)
    {
        *((Shared::ClipperXBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXBoundary)) = value;
    }
    inline void WriteXLeft(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXLeftBoundary_t value)
    {
        *((Shared::ClipperXLeftBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXLeftBoundary)) = value;
    }
    inline void WriteXRight(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXRightBoundary_t value)
    {
        *((Shared::ClipperXRightBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXRightBoundary)) = value;
    }
    inline void WriteYTop(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperYTopBoundary_t value)
    {
        *((Shared::ClipperYTopBoundary_t volatile *)(baseAddress + Shared::Register::ClipperYTopBoundary)) = value;
    }
    inline void WriteYBottom(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperYBottomBoundary_t value)
    {
        *((Shared::ClipperYBottomBoundary_t volatile *)(baseAddress + Shared::Register::ClipperYBottomBoundary)) = value;
    }
    inline void WriteXBoundaryCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXBoundary_t value)
    {
        *((Shared::ClipperXBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXBoundaryCommit)) = value;
    }
    inline void WriteXLeftCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXLeftBoundary_t value)
    {
        *((Shared::ClipperXLeftBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXLeftBoundaryCommit)) = value;
    }
    inline void WriteXRightCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperXRightBoundary_t value)
    {
        *((Shared::ClipperXRightBoundary_t volatile *)(baseAddress + Shared::Register::ClipperXRightBoundaryCommit)) = value;
    }
    inline void WriteYTopCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperYTopBoundary_t value)
    {
        *((Shared::ClipperYTopBoundary_t volatile *)(baseAddress + Shared::Register::ClipperYTopBoundaryCommit)) = value;
    }
    inline void WriteYBottomCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::ClipperYBottomBoundary_t value)
    {
        *((Shared::ClipperYBottomBoundary_t volatile *)(baseAddress + Shared::Register::ClipperYBottomBoundaryCommit)) = value;
    }
}

}