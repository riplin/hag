//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/pci/ctrlap.h>
//#include <hag/drivers/matrox/shared/mmio/dacwridx.h>

namespace Hag::Matrox::Shared::MMIO
{

namespace Register
{

enum
{
    IndexedData = 0x3C0A
};

}

typedef System::PCI::Register_t IndexedData_t;

namespace IndexedData
{
    inline IndirectData_t Read(Shared::PCI::ControlAperture_t baseAddress, Hag::VGA::DACWriteIndex_t index)
    {
        //DACWriteIndex::Write(baseAddress, index);
        return *((IndirectData_t volatile *)(baseAddress + Shared::Register::IndexedData));
        return 0;
    }
    
    inline void Write(Shared::PCI::ControlAperture_t baseAddress, Hag::VGA::DACWriteIndex_t index, IndexedData_t value)
    {
        //DACWriteIndex::Write(baseAddress, index);
        *((Shared::IndexedData_t volatile *)(baseAddress + Shared::Register::IndexedData)) = value;
    }
}

}