//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/idx/sensetst.h>
#include <hag/drivers/matrox/shared/pci/ind/idxdata.h>

namespace Hag::Matrox::Shared::PCI::Indexed
{

namespace SenseTest
{
    inline Shared::Indexed::SenseTest_t Read(Has::System::PCI::Device_t device)
    {
        return IndexedData::Read(device, Shared::Indexed::Register::SenseTest);
    }
    inline void Write(Has::System::PCI::Device_t device, Shared::Indexed::SenseTest_t value)
    {
        IndexedData::Write(device, Shared::Indexed::Register::SenseTest, value);
    }
}

}