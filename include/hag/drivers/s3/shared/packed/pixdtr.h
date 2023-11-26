//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/pixdtrf.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace MMIO { namespace Packed { namespace PixelDataTransfer
{
    inline uint32_t* Get()
    {
        return FARPointer(0xA000, 0x0000).ToPointer<uint32_t>(0x7FFF);
    }
}}}

}}}
