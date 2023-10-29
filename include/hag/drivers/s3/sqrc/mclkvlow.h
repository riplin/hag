//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/sqrc/regs.h>

namespace Hag { namespace S3 { namespace Sequencer
{

typedef uint8_t MClockValueLow_t;

namespace MClockValueLow
{
    
    enum //Mask
    {
        PLLNDivider = 0x1F,     //These bits contain the binary equivalent of the integer divider used to scale
                                //the input to the MCLK PLL. See Section 9 for detailed explanation.
        PLLRValue = 0x60        //These bits contain the binary equivalent of the integer (1, 2, 4, 8) range value used to
                                //scale the output of the MCLK PLL. See Section 9 for detailed explanation.
    };

    namespace Shift
    {
        enum
        {
            PLLNDivider = 0x00,
            PLLRValue = 0x05
        };
    }

    inline MClockValueLow_t Read()
    {
        VGA::SequencerIndex::Write(SequencerRegister::MClockValueLow);
        return MClockValueLow_t(VGA::SequencerData::Read());
    }
    
    inline void Write(MClockValueLow_t value)
    {
        VGA::SequencerData::Write(SequencerRegister::MClockValueLow, VGA::SequencerData_t(value));
    }

}

}}}
