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
    InputStatus0 = 0x3C2,                                   //STATUS_0
};

}

typedef uint8_t InputStatus0_t;

namespace InputStatus0
{

    enum //Mask
    {
        MonitorSenseStatus = 0x10       //MON SENS 0 = internal ~SENSE signal is logical 0, 1 = internal ~SENSE signal is logical 1
    };

    namespace Shift
    {
        enum
        {
            MonitorSenseStatus = 0x04
        };
    };

    inline InputStatus0_t Read()
    {
        return InputStatus0_t(SYS_ReadPortByte(Register::InputStatus0));
    }

}

}}
