//Copyright 2023-Present riplin

#pragma once

#include <hag/farptr.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    CurrentXPosition2 = 0x86EA,                             //CUR_X2
};

}

typedef uint16_t CurrentXPosition2_t;

namespace CurrentXPosition2
{

    inline CurrentXPosition2_t Read()
    {
        return CurrentXPosition2_t(SYS_ReadPortShort(Register::CurrentXPosition2));
    }

    inline void Write(CurrentXPosition2_t value)
    {
        SYS_WritePortShort(Register::CurrentXPosition2, value);
    }
}

namespace MMIO
{

    namespace CurrentXPosition2
    {
        inline CurrentXPosition2_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentXPosition2).
                ToPointer<CurrentXPosition2_t>(sizeof(CurrentXPosition2_t));
        }

        inline void Write(CurrentXPosition2_t value)
        {
            *FARPointer(0xA000, Register::CurrentXPosition2).
                ToPointer<CurrentXPosition2_t>(sizeof(CurrentXPosition2_t)) = value;
        }
    }

}

}
