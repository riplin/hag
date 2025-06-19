//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/mmio/idxdata.h>
#include <hag/drivers/matrox/shared/idx/sensetst.h>

namespace Hag::Matrox::Shared::MMIO::Indexed
{

namespace SenseTest
{
    inline Shared::Indexed::SenseTest_t Read(Shared::PCI::ControlAperture_t baseaddress)
    {
        return IndexedData::Read(baseaddress, Shared::Indexed::Register::SenseTest);
    }
    inline void Write(Shared::PCI::ControlAperture_t baseaddress, Shared::Indexed::SenseTest_t value)
    {
        IndexedData::Write(baseaddress, Shared::Indexed::Register::SenseTest, value);
    }
}

}