//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/bgmix.h>
#include <hag/drivers/s3/shared/fgmix.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        Mixers = 0x8134,
    };
}

namespace MMIO::Packed::Mixers
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::Mixers).ToPointer<uint32_t>();
    }

    inline void Write(ForegroundMix_t foregroundMix, BackgroundMix_t backgroundMix)
    {
            Get() = (uint32_t(foregroundMix) << 16) | backgroundMix;
    }
}

}
