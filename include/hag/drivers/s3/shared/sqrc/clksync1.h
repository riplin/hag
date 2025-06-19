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
    ClockSynthControl1 = 0x14,              //SR14
};

}

typedef uint8_t ClockSynthControl1_t;

namespace ClockSynthControl1
{

    enum //Mask
    {
        PowerDownDClockPLL = 0x01,              //0 = DClock PLL powered, 1 = DClock PLL powered down. This bit is used for S3 test purposes only.
        PowerDownMClockPLL = 0x02,              //0 = MClock PLL powered, 1 = MClock PLL powered down. This bit is used for S3 test purposes only.
        EnableClockSynthesizerCounters = 0x04,  //0 = Clock synthesiser counters disabled, 1 = Clock synthesizer counters enabled. This bit is used for S3 test purposes only.
        MClockTest = 0x08,                      //0 Test DClock, 1 = Test MClock. This bit is used for S3 test purposes only.
        ClearClockSynthesizerCounters = 0x10,   //0 = No effect, 1 = Clear the clock synthesizer counters. This bit is used for S3 test purposes only.
        Pin146Select = 0x20,                    //0 = Pin 146 functions as ~STRD, 1 = Pin 146 is tri-staded. 
                                                //  Setting this bit to 1 allows pin 146 to act as an input. This is enabled by setting bit 6
                                                //  of this register to 1.
        ExternalMClockSelect = 0x40,            //0 = MCLK provided by internal PLL, 1 = MCLK is input on pin 146. An external MCLK is only used for S3 Test purposes.
        ExternalDClockSelect = 0x80             //0 = DCLK provided by internal PLL, 1 = DCLK is input on pin 156. PD11 must also be strapped low on power-on reset.
                                                //  An external DCLK is only used for S3 test purposes.
    };

    namespace Shift
    {
        enum
        {
            PowerDownDClockPLL = 0x00,
            PowerDownMClockPLL = 0x01,
            EnableClockSynthesizerCounters = 0x02,
            MClockTest = 0x03,
            ClearClockSynthesizerCounters = 0x04,
            Pin146Select = 0x05,
            ExternalMClockSelect = 0x06,
            ExternalDClockSelect = 0x07,
        };
    }

    inline ClockSynthControl1_t Read()
    {
        VGA::SequencerIndex::Write(Register::ClockSynthControl1);
        return ClockSynthControl1_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ClockSynthControl1_t value)
    {
        VGA::SequencerData::Write(Register::ClockSynthControl1, VGA::SequencerData_t(value));
    }

}

}
