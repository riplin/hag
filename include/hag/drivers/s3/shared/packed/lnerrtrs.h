//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/linerrtr.h>
#include <hag/drivers/s3/shared/linerrt2.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        LineErrorTerms = 0x8110,
    };
}

namespace MMIO::Packed::LineErrorTerms
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::LineErrorTerms).ToPointer<uint32_t>();
    }

    inline void Write(LineErrorTerm2_t errorTerm2, LineErrorTerm_t errorTerm)
    {
            Get() = (uint32_t(errorTerm2) << 16) | errorTerm;
    }
}

}
