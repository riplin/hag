//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <has/types.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    PatternY = 0xEAE8,                                      //PAT_Y
};

}

typedef uint16_t PatternY_t;

namespace PatternY
{

    inline PatternY_t Read()
    {
        return PatternY_t(SYS_ReadPortShort(Register::PatternY));
    }

    inline void Write(PatternY_t value)
    {
        SYS_WritePortShort(Register::PatternY, value);
    }
}

namespace MMIO
{

    namespace PatternY
    {
        inline PatternY_t Read()
        {
            return *FARPointer(0xA000, Register::PatternY).
                ToPointer<PatternY_t>(sizeof(PatternY_t));
        }

        inline void Write(PatternY_t value)
        {
            *FARPointer(0xA000, Register::PatternY).
                ToPointer<PatternY_t>(sizeof(PatternY_t)) = value;
        }
    }

}

}
