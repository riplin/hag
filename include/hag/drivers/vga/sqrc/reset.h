//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

typedef uint8_t Reset_t;

namespace Reset
{
    enum //Mask
    {
        AsynchronousReset = 0x01,       //~ASY RST This bit is for VGA software compatibility only. It has no function for the Trio32/Trio64.
        SynchronousReset = 0x02         //~SYN RST This bit is for VGA software compatibility only. It has no funciton for the Trio32/Trio64.
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
        SequencerIndex::Write(SequencerRegister::Reset); return Reset_t(SequencerData::Read());
    }

    inline void Write(Reset_t value)
    {
        SequencerData::Write(SequencerRegister::Reset, SequencerData_t(value));
    }
}

}}}
