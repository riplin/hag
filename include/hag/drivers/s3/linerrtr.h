//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t LineErrorTerm_t;

namespace LineErrorTerm
{

    inline LineErrorTerm_t Read()
    {
        return LineErrorTerm_t(SYS_ReadPortShort(Register::LineErrorTerm));
    }

    inline void Write(LineErrorTerm_t value)
    {
        SYS_WritePortShort(Register::LineErrorTerm, value);
    }
}

}}
