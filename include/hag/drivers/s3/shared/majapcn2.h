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
    MajorAxisPixelCount2 = 0x96EA,                          //MAJ_AXIS_PCNT2
};

}

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

namespace MMIO
{

    namespace MajorAxisPixelCount2
    {
        inline MajorAxisPixelCount2_t Read()
        {
            return *FARPointer(0xA000, Register::MajorAxisPixelCount2).
                ToPointer<MajorAxisPixelCount2_t>(sizeof(MajorAxisPixelCount2_t));
        }

        inline void Write(MajorAxisPixelCount2_t value)
        {
            *FARPointer(0xA000, Register::MajorAxisPixelCount2).
                ToPointer<MajorAxisPixelCount2_t>(sizeof(MajorAxisPixelCount2_t)) = value;
        }
    }

}

}}}
