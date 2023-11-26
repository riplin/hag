//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        MultiFunctionAndReadSelect = 0x8144,
    };
}}

namespace MMIO { namespace Packed { namespace MultiFunctionAndReadSelect
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::MultiFunctionAndReadSelect).ToPointer<uint32_t>();
    }

    inline void Write(ReadSelect_t readSelect, MultifunctionControlMiscellaneous_t multiFunctionControlMisc)
    {
            Get() = (uint32_t(readSelect) << 16) | multiFunctionControlMisc;
    }
}}}

}}}
