//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/xcoord2.h>
#include <hag/drivers/s3/shared/yc2asc2.h>

namespace Hag::S3::Shared
{

namespace Register::Packed
{
    enum
    {
        XYCoordinate2 = 0x810C,
    };
}

namespace MMIO::Packed::XYCoordinate2
{
    inline uint32_t& Get()
    {
        return *FARPointer(0xA000, Register::Packed::XYCoordinate2).ToPointer<uint32_t>();
    }

    inline void Write(XCoordinate2_t coordinateX, YCoordinate2AxialStepConstant2_t coordinateY)
    {
            Get() = (uint32_t(coordinateX) << 16) | coordinateY;
    }
}

}
