//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t MajorAxisPixelCount2_t;

namespace MajorAxisPixelCount2
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };

    inline MajorAxisPixelCount2_t Read()
    {
        return MajorAxisPixelCount2_t(SYS_ReadPortShort(Register::MajorAxisPixelCount2));
    }

    inline void Write(MajorAxisPixelCount2_t value)
    {
        SYS_WritePortShort(Register::MajorAxisPixelCount2, value);
    }
}

}}
