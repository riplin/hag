//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/regs/status.h>
#include <hag/drivers/matrox/shared/pci/ind/indidx.h>
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI { namespace Indirect
{

namespace Status
{
    inline Status_t Read(System::PCI::Device_t device)
    {
        IndirectIndex::Write(device, Shared::Register::Status);
        return Status_t(IndirectData::Read(device));
    }
}

}}}}}