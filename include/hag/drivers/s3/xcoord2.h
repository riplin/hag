//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t XCoordinate2_t;

namespace XCoordinate2
{

    inline XCoordinate2_t Read()
    {
        return XCoordinate2_t(SYS_ReadPortShort(Register::XCoordinate2));
    }

    inline void Write(XCoordinate2_t value)
    {
        SYS_WritePortShort(Register::XCoordinate2, value);
    }
}

}}
