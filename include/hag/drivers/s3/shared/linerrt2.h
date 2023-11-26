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
    LineErrorTerm2 = 0x92EA,                                //ERR_TERM2
};

}

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

namespace MMIO
{

    namespace LineErrorTerm2
    {
        inline LineErrorTerm2_t Read()
        {
            return *FARPointer(0xA000, Register::LineErrorTerm2).
                ToPointer<LineErrorTerm2_t>(sizeof(LineErrorTerm2_t));
        }

        inline void Write(LineErrorTerm2_t value)
        {
            *FARPointer(0xA000, Register::LineErrorTerm2).
                ToPointer<LineErrorTerm2_t>(sizeof(LineErrorTerm2_t)) = value;
        }
    }

}

}}}
