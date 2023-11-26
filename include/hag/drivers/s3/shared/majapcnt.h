//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    MajorAxisPixelCount = 0x96E8,                           //MAJ_AXIS_PCNT
};

}

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

namespace MMIO
{

    namespace MajorAxisPixelCount
    {
        inline MajorAxisPixelCount_t Read()
        {
            return *FARPointer(0xA000, Register::MajorAxisPixelCount).
                ToPointer<MajorAxisPixelCount_t>(sizeof(MajorAxisPixelCount_t));
        }

        inline void Write(MajorAxisPixelCount_t value)
        {
            *FARPointer(0xA000, Register::MajorAxisPixelCount).
                ToPointer<MajorAxisPixelCount_t>(sizeof(MajorAxisPixelCount_t)) = value;
        }
    }

}

}}}
