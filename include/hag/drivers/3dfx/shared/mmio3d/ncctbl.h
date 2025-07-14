//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/ncctbl.h>

namespace Hag::TDfx::Shared::MMIO3D
{

namespace NccTable
{
    // Writes all values in table 0.
    inline void WriteYIQ0(uint8_t* baseAddress, Shared::ThreeD::NccTable_t* values, Shared::ThreeD::Chip_t chip)
    {
        using namespace Shared::ThreeD;
        NccTable_t* ptr = (Shared::ThreeD::NccTable_t*)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip));

        for (int i = 0; i < 12; ++i)
            *(ptr++) = *(values++);
    }

    // Writes all values in table 1.
    inline void WriteYIQ1(uint8_t* baseAddress, Shared::ThreeD::NccTable_t* values, Shared::ThreeD::Chip_t chip)
    {
        using namespace Shared::ThreeD;
        NccTable_t* ptr = (Shared::ThreeD::NccTable_t*)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable1 | (int)chip));

        for (int i = 0; i < 12; ++i)
            *(ptr++) = *(values++);
    }

    // Writes only the I and Q values in the table 0.
    inline void WriteIQ(uint8_t* baseAddress, Shared::ThreeD::NccTable_t* values, Shared::ThreeD::Chip_t chip)
    {
        using namespace Shared::ThreeD;
        NccTable_t* ptr = (Shared::ThreeD::NccTable_t*)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 16);
        for (int i = 0; i < 8; ++i)
            *(ptr++) = *(values++);
    }

    inline void WriteY0(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 0)) = value;
    }

    inline void WriteY1(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 4)) = value;
    }

    inline void WriteY2(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 8)) = value;
    }

    inline void WriteY3(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 12)) = value;
    }

    inline void WriteI0(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 16 + 0)) = value;
    }

    inline void WriteI1(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 16 + 4)) = value;
    }

    inline void WriteI2(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 16 + 8)) = value;
    }

    inline void WriteI3(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 16 + 12)) = value;
    }

    inline void WriteQ0(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 32 + 0)) = value;
    }

    inline void WriteQ1(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 32 + 4)) = value;
    }

    inline void WriteQ2(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 32 + 8)) = value;
    }

    inline void WriteQ3(uint8_t* baseAddress, Shared::ThreeD::NccTable_t value, Shared::ThreeD::Chip_t chip)
    {
        *((Shared::ThreeD::NccTable_t volatile *)(baseAddress + ((int)Shared::ThreeD::Register::Base | (int)Shared::ThreeD::Register::NccTable0 | (int)chip) + 32 + 12)) = value;
    }

}

}