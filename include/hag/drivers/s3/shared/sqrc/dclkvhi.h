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
    DClockValueHigh = 0x13,                 //SR13
};

}

typedef uint8_t DClockValueHigh_t;

namespace DClockValueHigh
{
    
    enum //Mask
    {
        PLLMDivider = 0x7F  //These bits contain the binary equivalent of the integer (1-127) divider used in the feedback
                            //loop of the DCLK PLL. See Section 9 for a detailed explanation.
    };

    namespace Shift
    {
        enum
        {
            PLLMDivider = 0x00
        };
    }

    inline DClockValueHigh_t Read()
    {
        VGA::SequencerIndex::Write(Register::DClockValueHigh);
        return DClockValueHigh_t(VGA::SequencerData::Read());
    }
    
    inline void Write(DClockValueHigh_t value)
    {
        VGA::SequencerData::Write(Register::DClockValueHigh, VGA::SequencerData_t(value));
    }

}

}}}}
