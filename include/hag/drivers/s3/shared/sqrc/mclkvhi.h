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
    MClockValueHigh = 0x11,                 //SR11
};

}

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
        VGA::SequencerIndex::Write(Register::MClockValueHigh);
        return MClockValueHigh_t(VGA::SequencerData::Read());
    }
    
    inline void Write(MClockValueHigh_t value)
    {
        VGA::SequencerData::Write(Register::MClockValueHigh, VGA::SequencerData_t(value));
    }

}

}}}}
