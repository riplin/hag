//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
#include <hag/drivers/matrox/shared/regs/status.h>

namespace Hag { namespace Matrox { namespace Shared { namespace MMIO
{

namespace Status
{
    inline Status_t Read(Shared::PCI::ControlAperture_t baseAddress)
    {
        return *((Shared::Status_t volatile *)(baseAddress + Shared::Register::Status));
    }

    inline void WaitDrawingEngineReady(Shared::PCI::ControlAperture_t baseAddress)
    {
        while ((Read(baseAddress) & Shared::Status::DrawingEngineBusy) == Shared::Status::DrawingEngineBusy);
    }
}

}}}}