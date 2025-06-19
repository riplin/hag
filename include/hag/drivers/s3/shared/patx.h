//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/types.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    PatternX = 0xEAEA                                       //PAT_X
};

}

typedef uint16_t PatternX_t;

namespace PatternX
{

    inline PatternX_t Read()
    {
        return PatternX_t(SYS_ReadPortShort(Register::PatternX));
    }

    inline void Write(PatternX_t value)
    {
        SYS_WritePortShort(Register::PatternX, value);
    }
}

namespace MMIO
{

    namespace PatternX
    {
        inline PatternX_t Read()
        {
            return *FARPointer(0xA000, Register::PatternX).
                ToPointer<PatternX_t>(sizeof(PatternX_t));
        }

        inline void Write(PatternX_t value)
        {
            *FARPointer(0xA000, Register::PatternX).
                ToPointer<PatternX_t>(sizeof(PatternX_t)) = value;
        }
    }

}

}
