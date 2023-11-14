//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace VGA
{

typedef uint8_t InputStatus1_t;

namespace InputStatus1
{
    enum //Mask
    {
        DisplayModeInactive = 0x01,         //~DTM 0 = The display is in the display mode. 1 = Either horizontal or vertical retrace period is active
        VerticalSyncActive = 0x08,          //VSY 0 Display is in the display mode. 1 = Display is in the vertical retrace mode.
        VideoSignalTest = 0x30,             //TST-VDT Video Data feedback 1,0. 
    };

    namespace Shift
    {
        enum
        {
            DisplayModeInactive = 0x00,
            VerticalSyncActive = 0x03,
            VideoSignalTest = 0x04,
        };
    }

    inline InputStatus1_t Read(Register_t inputStatus1)
    {
        return InputStatus1_t(SYS_ReadPortByte(inputStatus1));
    }

}

}}
