//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t MajorAxisPixelCount_t;

namespace MajorAxisPixelCount
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };

    inline MajorAxisPixelCount_t Read()
    {
        return MajorAxisPixelCount_t(SYS_ReadPortShort(Register::MajorAxisPixelCount));
    }

    inline void Write(MajorAxisPixelCount_t value)
    {
        SYS_WritePortShort(Register::MajorAxisPixelCount, value);
    }
}

}}
