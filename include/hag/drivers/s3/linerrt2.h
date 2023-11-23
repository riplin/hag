//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t LineErrorTerm2_t;

namespace LineErrorTerm2
{

    inline LineErrorTerm2_t Read()
    {
        return LineErrorTerm2_t(SYS_ReadPortShort(Register::LineErrorTerm2));
    }

    inline void Write(LineErrorTerm2_t value)
    {
        SYS_WritePortShort(Register::LineErrorTerm2, value);
    }
}

}}
