//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/majapcn2.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        MajorAxisPixelCount2 = 0x814C,
    };
}

namespace MMIO::Packed::MajorAxisPixelCount2
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::MajorAxisPixelCount2).ToPointer<uint32_t>();
    }

    inline void Write(MajorAxisPixelCount2_t majorAxisPixelCount2)
    {
        Get() = majorAxisPixelCount2;
    }
}

}
