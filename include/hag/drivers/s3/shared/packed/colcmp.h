//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/colcomp.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        ColorCompare = 0x8130,
    };
}

namespace MMIO::Packed::ColorCompare
{
    inline ColorCompare_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ColorCompare).ToPointer<ColorCompare_t>();
    }

    inline void Write(ColorCompare_t colorCompare)
    {
            Get() = colorCompare;
    }
}

}