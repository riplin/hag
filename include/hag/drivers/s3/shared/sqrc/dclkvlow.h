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
    DClockValueLow = 0x12,                  //SR12
};

}

typedef uint8_t DClockValueLow_t;

namespace DClockValueLow
{
    
    enum //Mask
    {
        PLLNDivider = 0x1F,     //These bits contain the binary equivalen of the integer (1-31) divider used to scale
                                //the input to the DCLK PLL. See Seqction 9 for a detailed explanation.
        PLLRValue = 0x60        //These bits contain the binary equivalent of the integer (1, 2, 4, 8) range value used
                                //to scale the output of the DCLK PLL. See Section 9 for a detailed explanation.
    };

    namespace Shift
    {
        enum
        {
            PLLNDivider = 0x00,
            PLLRValue = 0x05
        };
    }

    inline DClockValueLow_t Read()
    {
        VGA::SequencerIndex::Write(Register::DClockValueLow);
        return DClockValueLow_t(VGA::SequencerData::Read());
    }
    
    inline void Write(DClockValueLow_t value)
    {
        VGA::SequencerData::Write(Register::DClockValueLow, VGA::SequencerData_t(value));
    }

}

}}}}
