//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{

enum
{
    SubsystemControl = 0x42E8,                              //SUBSYS_CNTL
};

}

typedef uint16_t SubsystemControl_t;

namespace SubsystemControl
{
    enum
    {
        ClearVerticalSyncInterruptStatus = 0x01,        //0 = No change
                                                        //1 = Clear
        ClearGraphicsEngineBusyInterruptStatus = 0x02,  //0 = No change
                                                        //1 = Clear
        ClearCommandFIFOOverflowInterruptStatus = 0x04, //0 = No change
                                                        //1 = Clear
        ClearCommandFIFOEmptyInterruptStatus = 0x08,    //0 = No change
                                                        //1 = Clear
        VerticalSyncInterruptEnable = 0x100,            //0 = Disable
                                                        //1 = Enable
        GraphicsEngineBusyInterruptEnable = 0x200,      //0 = Disable
                                                        //1 = Enable
        CommandFIFOOverflowInterruptEnable = 0x400,     //0 = Disable
                                                        //1 = Enable
        CommandFIFOEmptyInterruptEnable = 0x800,        //0 = Disable
                                                        //1 = Enable
        GraphicsEngineSoftwareReset = 0xC000            //00 = No change
                                                        //01 = Graphics Engine enabled
                                                        //10 = Reset
                                                        //11 = Reserved
    };

    namespace Shift
    {
        enum
        {
            ClearVerticalSyncInterruptStatus = 0x00,
            ClearGraphicsEngineBusyInterruptStatus = 0x01,
            ClearCommandFIFOOverflowInterruptStatus = 0x02,
            ClearCommandFIFOEmptyInterruptStatus = 0x03,
            VerticalSyncInterruptEnable = 0x08,
            GraphicsEngineBusyInterruptEnable = 0x09,
            CommandFIFOOverflowInterruptEnable = 0x0A,
            CommandFIFOEmptyInterruptEnable = 0x0B,
            GraphicsEngineSoftwareReset = 0x0E
        };
    }

    inline void Write(SubsystemControl_t value)
    {
        SYS_WritePortShort(Register::SubsystemControl, value);
    }
}

}}}
