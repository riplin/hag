//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/colcomp.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        ColorCompare = 0x8130,
    };
}}

namespace MMIO { namespace Packed { namespace ColorCompare
{
    inline ColorCompare_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ColorCompare).ToPointer<ColorCompare_t>();
    }

    inline void Write(ColorCompare_t colorCompare)
    {
            Get() = colorCompare;
    }
}}}

}}}