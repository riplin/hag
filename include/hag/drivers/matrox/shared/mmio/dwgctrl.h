//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/dwgctrl.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace DrawingControl
{
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Shared::DrawingControl_t value)
    {
        *((Shared::DrawingControl_t volatile *)(baseAddress + Shared::Register::DrawingControl)) = value;
    }
    inline void WriteCommit(Shared::PCI::ControlAperture_t baseAddress, Shared::DrawingControl_t value)
    {
        *((Shared::DrawingControl_t volatile *)(baseAddress + Shared::Register::DrawingControlCommit)) = value;
    }
}

}}}}