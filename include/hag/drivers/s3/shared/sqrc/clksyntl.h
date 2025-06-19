//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/shared/sqrc/regtype.h>

namespace Hag::S3::Shared::Sequencer
{

namespace Register
{
    
enum
{
    ClockSynthTestLow = 0x17,               //SR17
};

}

typedef uint8_t ClockSynthTestLow_t;

namespace ClockSynthTestLow
{

    inline ClockSynthTestLow_t Read()
    {
        VGA::SequencerIndex::Write(Register::ClockSynthTestLow);
        return ClockSynthTestLow_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ClockSynthTestLow_t value)
    {
        VGA::SequencerData::Write(Register::ClockSynthTestLow, VGA::SequencerData_t(value));
    }

}

}
