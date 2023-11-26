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
    YCoordinate2AxialStepConstant2 = 0x8AEA,                //Y2_AXSTP2
};

}

typedef uint16_t YCoordinate2AxialStepConstant2_t;

namespace YCoordinate2AxialStepConstant2
{

    inline YCoordinate2AxialStepConstant2_t Read()
    {
        return YCoordinate2AxialStepConstant2_t(SYS_ReadPortShort(Register::YCoordinate2AxialStepConstant2));
    }

    inline void Write(YCoordinate2AxialStepConstant2_t value)
    {
        SYS_WritePortShort(Register::YCoordinate2AxialStepConstant2, value);
    }
}

namespace MMIO
{

    namespace YCoordinate2AxialStepConstant2
    {
        inline YCoordinate2AxialStepConstant2_t Read()
        {
            return *FARPointer(0xA000, Register::YCoordinate2AxialStepConstant2).
                ToPointer<YCoordinate2AxialStepConstant2_t>(sizeof(YCoordinate2AxialStepConstant2_t));
        }

        inline void Write(YCoordinate2AxialStepConstant2_t value)
        {
            *FARPointer(0xA000, Register::YCoordinate2AxialStepConstant2).
                ToPointer<YCoordinate2AxialStepConstant2_t>(sizeof(YCoordinate2AxialStepConstant2_t)) = value;
        }
    }

}

}}}
