//Copyright 2023-Present riplin

#pragma once

#include <hag/system/sysasm.h>
#include <hag/drivers/s3/shared/wregdata.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    ReadRegisterData = 0xBEE8,                              //RD_REG_DT
};

}

typedef uint16_t ReadRegisterData_t;
typedef uint16_t ReadRegisterIndex_t;

namespace ReadRegisterData
{

    enum
    {
        MinorAxisPixelCount = WriteRegisterData::MinorAxisPixelCount,
        TopScissors = WriteRegisterData::TopScissors,
        LeftScissors = WriteRegisterData::LeftScissors,
        BottomScissors = WriteRegisterData::BottomScissors,
        RightScissors = WriteRegisterData::RightScissors,
        PixelControl = WriteRegisterData::PixelControl,
        MultifunctionControlMisc2 = WriteRegisterData::MultifunctionControlMisc2,
        MultifunctionControlMisc = WriteRegisterData::MultifunctionControlMisc
    };

    inline ReadRegisterData_t Read(ReadRegisterIndex_t index)
    {
        WriteRegisterData::Write(index);
        return ReadRegisterData_t(SYS_ReadPortShort(Register::ReadRegisterData));
    }

    inline ReadRegisterData_t ReadMinorAxisPixelCount() { return Read(MinorAxisPixelCount); }
    inline ReadRegisterData_t ReadTopScissors() { return Read(TopScissors); }
    inline ReadRegisterData_t ReadLeftScissors() { return Read(LeftScissors); }
    inline ReadRegisterData_t ReadBottomScissors() { return Read(BottomScissors); }
    inline ReadRegisterData_t ReadRightScissors() { return Read(RightScissors); }
    inline ReadRegisterData_t ReadPixelControl() { return Read(PixelControl); }
    inline ReadRegisterData_t ReadMultifunctionControlMisc2() { return Read(MultifunctionControlMisc2); }
    inline ReadRegisterData_t ReadMultifunctionControlMisc() { return Read(MultifunctionControlMisc); }

}

}}}
