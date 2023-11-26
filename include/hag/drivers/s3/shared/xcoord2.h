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
    XCoordinate2 = 0x8EEA,                                  //X2_DIASTP2
};

}

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

namespace MMIO
{

    namespace XCoordinate2
    {
        inline XCoordinate2_t Read()
        {
            return *FARPointer(0xA000, Register::XCoordinate2).
                ToPointer<XCoordinate2_t>(sizeof(XCoordinate2_t));
        }

        inline void Write(XCoordinate2_t value)
        {
            *FARPointer(0xA000, Register::XCoordinate2).
                ToPointer<XCoordinate2_t>(sizeof(XCoordinate2_t)) = value;
        }
    }

}

}}}
