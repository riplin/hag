//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

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

}}
