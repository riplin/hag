//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/shrstrvt.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register { namespace Packed
{
    enum
    {
        ShortStrokeVectorTransfer = 0x811C,
    };
}}

namespace MMIO { namespace Packed { namespace ShortStrokeVectorTransfer
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::ShortStrokeVectorTransfer).ToPointer<uint32_t>();
    }

    inline void Write(ShortStrokeVectorTransfer_t shortStrokeVector)
    {
            Get() = shortStrokeVector;
    }
}}}

}}}
