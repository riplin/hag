//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/sqrc/regs.h>

namespace Hag { namespace S3 { namespace Sequencer
{

typedef uint8_t MClockValueHigh_t;

namespace MClockValueHigh
{

    enum //Mask
    {
        PLLMDivider = 0x7F  //These bits contain the binary equivalent of the integer (1-127) divider used in the feedback
                            //loop of the MCLK PLL. See Section 9 for a detailed explanation.
    };

    namespace Shift
    {
        enum
        {
            PLLMDivider = 0x00
        };
    }

    inline MClockValueHigh_t Read()
    {
        VGA::SequencerIndex::Write(SequencerRegister::MClockValueHigh);
        return MClockValueHigh_t(VGA::SequencerData::Read());
    }
    
    inline void Write(MClockValueHigh_t value)
    {
        VGA::SequencerData::Write(SequencerRegister::MClockValueHigh, VGA::SequencerData_t(value));
    }

}

}}}
