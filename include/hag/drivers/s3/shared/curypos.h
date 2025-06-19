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
    CurrentYPosition = 0x82E8,                              //CUR_Y
};

}

typedef uint16_t CurrentYPosition_t;

namespace CurrentYPosition
{

    inline CurrentYPosition_t Read()
    {
        return CurrentYPosition_t(SYS_ReadPortShort(Register::CurrentYPosition));
    }

    inline void Write(CurrentYPosition_t value)
    {
        SYS_WritePortShort(Register::CurrentYPosition, value);
    }
}

namespace MMIO
{

    namespace CurrentYPosition
    {
        inline CurrentYPosition_t Read()
        {
            return *FARPointer(0xA000, Register::CurrentYPosition).
                ToPointer<CurrentYPosition_t>(sizeof(CurrentYPosition_t));
        }

        inline void Write(CurrentYPosition_t value)
        {
            *FARPointer(0xA000, Register::CurrentYPosition).
                ToPointer<CurrentYPosition_t>(sizeof(CurrentYPosition_t)) = value;
        }
    }

}

}
