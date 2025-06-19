//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/patx.h>
#include <hag/drivers/s3/shared/paty.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        PatternXY = 0x8168
    };
}

namespace MMIO::Packed::PatternXY
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::PatternXY).ToPointer<uint32_t>();
    }

    inline void Write(PatternX_t patternX, PatternY_t patternY)
    {
            Get() = (uint32_t(patternX) << 16) | patternY;
    }
}

}
