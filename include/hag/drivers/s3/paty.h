//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

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

}}
