//Copyright 2023-Present riplin

#pragma once

#include <has/types.h>
#include <hag/drivers/s3/shared/regtype.h>
#include <has/system/sysasm.h>

namespace Hag::S3::Shared
{

namespace Register
{

enum
{
    SubsystemStatus = 0x42E8,                               //SUBSYS_STAT
};

}

//This read-only register provides information on interrupt status, monitor I.D. and the number of bits per pixel.
//See the Subsystem Control (42E8H, Write Only) register for details on enabling and clearing interrupts.

typedef uint16_t SubsystemStatus_t;

namespace SubsystemStatus
{
    enum
    {
        VerticalSyncInterruptStatus = 0x01,         //0 = No interrupt
                                                    //1 = Interrupt generated if enabled
        GraphicsEngineBusyInterruptStatus = 0x02,   //0 = No interrupt
                                                    //1 = Interrupt generated if enabled
        CommandFIFOOverflowInterruptStatus = 0x04,  //0 = No interrupt
                                                    //1 = Interrupt generated if enabled
        CommandFIFOEmptyInterruptStatus = 0x08,     //0 = No interrupt
                                                    //1 = Interrupt generated if enabled
        PixelLength = 0x80                          //0 = 4-bit
                                                    //1 = 8-bit
                                                    //This reflects the number of bit planes when bits 5-4 of the
                                                    //Extended System Control 1 register (CR50) are OOb.
    };

    namespace Shift
    {
        enum
        {
            VerticalSyncInterruptStatus = 0x00,
            GraphicsEngineBusyInterruptStatus = 0x01,
            CommandFIFOOverflowInterruptStatus = 0x02,
            CommandFIFOEmptyInterruptStatus = 0x03,
            PixelLength = 0x07
        };
    }

    inline SubsystemStatus_t Read()
    {
        return SubsystemStatus_t(SYS_ReadPortShort(Register::SubsystemStatus));
    }

}

}
