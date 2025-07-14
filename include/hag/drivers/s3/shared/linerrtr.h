//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    LineErrorTerm = 0x92E8,                                 //ERR_TERM
};

}

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

namespace MMIO
{

    namespace LineErrorTerm
    {
        inline LineErrorTerm_t Read()
        {
            return *FARPointer(0xA000, Register::LineErrorTerm).
                ToPointer<LineErrorTerm_t>(sizeof(LineErrorTerm_t));
        }

        inline void Write(LineErrorTerm_t value)
        {
            *FARPointer(0xA000, Register::LineErrorTerm).
                ToPointer<LineErrorTerm_t>(sizeof(LineErrorTerm_t)) = value;
        }
    }

}

}
