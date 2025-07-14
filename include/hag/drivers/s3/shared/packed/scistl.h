//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        ScissorsTopLeft = 0x8138,
    };
}

namespace MMIO::Packed::ScissorsTopLeft
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ScissorsTopLeft).ToPointer<uint32_t>();
    }

    inline void Write(LeftScissors_t leftScissors, TopScissors_t topScissors)
    {
            Get() = (uint32_t(leftScissors) << 16) | topScissors;
    }
}

}