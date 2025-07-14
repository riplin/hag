//Copyright 2023-Present riplin

#pragma once

#include <has/farptr.h>
#include <has/system/sysasm.h>
#include <hag/drivers/s3/shared/regtype.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    CurrentYPosition2 = 0x82EA,                             //CUR_Y2
};

}

typedef uint16_t CurrentYPosition2_t;

namespace CurrentYPosition2
{

    inline CurrentYPosition2_t Read()
    {
        return CurrentYPosition2_t(SYS_ReadPortShort(Register::CurrentYPosition2));
    }

    inline void Write(CurrentYPosition2_t value)
    {
        SYS_WritePortShort(Register::CurrentYPosition2, value);
    }
}

namespace MMIO
{

    namespace CurrentYPosition2
    {
        inline CurrentYPosition2_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentYPosition2).
                ToPointer<CurrentYPosition2_t>(sizeof(CurrentYPosition2_t));
        }

        inline void Write(CurrentYPosition2_t value)
        {
            *FARPointer(0xA000, Register::CurrentYPosition2).
                ToPointer<CurrentYPosition2_t>(sizeof(CurrentYPosition2_t)) = value;
        }
    }

}

}
