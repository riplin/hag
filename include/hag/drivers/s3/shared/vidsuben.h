//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    VideoSubsystemEnable = 0x46E8,                          //SETUP_MD // Original VGA this is 0x3C3
};

}

typedef uint8_t VideoSubsystemEnable_t;

namespace VideoSubsystemEnable
{
    enum
    {
        EnableAddressDecoding = 0x08,   //0 = Video I/O and memory address decoding are disabled
                                        //1 = Video I/O and memory address decoders are enabled
        EnableSetup = 0x10              //0 = The Trio32/Trio64 is in operational mode
                                        //1 = The Trio32/Trio64 is placed in setup mode
    };

    namespace Shift
    {
        enum
        {
            EnableAddressDecoding = 0x03,
            EnableSetup = 0x04
        };
    }

    inline VideoSubsystemEnable_t Read()
    {
        return VideoSubsystemEnable_t(SYS_ReadPortByte(Register::VideoSubsystemEnable));
    }

    inline void Write(VideoSubsystemEnable_t value)
    {
        SYS_WritePortByte(Register::VideoSubsystemEnable, value);
    }
}

}
