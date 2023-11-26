//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    VideoSubsystemEnable = 0x3C3,                           //SETUP_MD  //This register is reserved in standard VGA
};

}

typedef uint8_t VideoSubsystemEnable_t;

namespace VideoSubsystemEnable
{

    inline VideoSubsystemEnable_t Read()
    {
        return VideoSubsystemEnable_t(SYS_ReadPortByte(Register::VideoSubsystemEnable));
    }

    inline void Write(VideoSubsystemEnable_t value)
    {
        SYS_WritePortByte(Register::VideoSubsystemEnable, value);
    }
}

}}
