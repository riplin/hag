//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/regtype.h>
#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

namespace Register
{
    
enum
{
    ClockingMode = 0x01,                    //CLK_MODE SR1
};

}

typedef uint8_t ClockingMode_t;

namespace ClockingMode
{
    enum //Mask
    {
        DotClockSelect = 0x01,                              //8DC 0 = Character clocks 9 dots wide, 1 = Character clocks 8 dots wide
        LoadSerializersEverySecondCharacterClock = 0x04,    //SHF LD 0 = Load the video serializer every character clock, 1 = every other character clock
        InternalCharacterClock = 0x08,                      //DCK 1/2 0 = Internal character clock = DCLK, 1 = internal character clock is 1/2 DCLK
        LoadSerializersEveryFourthCaracterClock = 0x10,     //SHF 4 0 = Load serializers every character clock cycle, 1 = Load serializers every fourth character clock cycle
        ScreenOff = 0x20                                    //SCRN OFF 0 = Screen on, 1 = Screen off
    };

    namespace Shift
    {
        enum
        {
            DotClockSelect = 0x00,
            LoadSerializersEverySecondCharacterClock = 0x02,
            InternalCharacterClock = 0x03,
            LoadSerializersEveryFourthCaracterClock = 0x04,
            ScreenOff = 0x05
        };
    }

    inline ClockingMode_t Read()
    {
        SequencerIndex::Write(Register::ClockingMode); return ClockingMode_t(SequencerData::Read());
    }

    inline void Write(ClockingMode_t value)
    {
        SequencerData::Write(Register::ClockingMode, SequencerData_t(value));
    }

    inline void TurnScreenOff()
    {
        ClockingMode_t value = Read();
        value |= ClockingMode_t(ScreenOff);
        Write(value);
    }

    inline void TurnScreenOn()
    {
        ClockingMode_t value = Read();
        value &= ClockingMode_t(~ScreenOff);
        Write(value);
    }

}

}}}
