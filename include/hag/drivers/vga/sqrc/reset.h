//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/regtype.h>
#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

namespace Register
{
    
enum
{
    Reset = 0x00,                           //RST_SYNC SR0
};

}

typedef uint8_t Reset_t;

namespace Reset
{
    enum //Mask
    {
        AsynchronousReset = 0x01,       //~ASY RST This bit is for VGA software compatibility only.
        SynchronousReset = 0x02         //~SYN RST This bit is for VGA software compatibility only.
    };

    namespace Shift
    {
        enum
        {
            AsynchronousReset = 0x00,
            SynchronousReset = 0x01
        };
    }

    inline Reset_t Read()
    {
        SequencerIndex::Write(Register::Reset); return Reset_t(SequencerData::Read());
    }

    inline void Write(Reset_t value)
    {
        SequencerData::Write(Register::Reset, SequencerData_t(value));
    }
}

}}}
