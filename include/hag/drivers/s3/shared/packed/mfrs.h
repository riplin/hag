//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        MultiFunctionAndReadSelect = 0x8144,
    };
}

namespace MMIO::Packed::MultiFunctionAndReadSelect
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::MultiFunctionAndReadSelect).ToPointer<uint32_t>();
    }

    inline void Write(ReadSelect_t readSelect, MultifunctionControlMiscellaneous_t multiFunctionControlMisc)
    {
            Get() = (uint32_t(readSelect) << 16) | multiFunctionControlMisc;
    }
}

}
