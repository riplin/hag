//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/shared/sqrc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace Sequencer
{

namespace Register
{
    
enum
{
    ClockSynthTestHigh = 0x16,              //SR16
};

}

typedef uint8_t ClockSynthTestHigh_t;

namespace ClockSynthTestHigh
{

    inline ClockSynthTestHigh_t Read()
    {
        VGA::SequencerIndex::Write(Register::ClockSynthTestHigh);
        return ClockSynthTestHigh_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ClockSynthTestHigh_t value)
    {
        VGA::SequencerData::Write(Register::ClockSynthTestHigh, VGA::SequencerData_t(value));
    }

}

}}}}
