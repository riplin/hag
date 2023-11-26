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
    MClockValueLow = 0x10,                  //UNLK_EXSR SR10
};

}

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
        VGA::SequencerIndex::Write(Register::MClockValueLow);
        return MClockValueLow_t(VGA::SequencerData::Read());
    }
    
    inline void Write(MClockValueLow_t value)
    {
        VGA::SequencerData::Write(Register::MClockValueLow, VGA::SequencerData_t(value));
    }

}

}}}}
