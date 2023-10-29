//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

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
        SequencerIndex::Write(SequencerRegister::ClockingMode); return ClockingMode_t(SequencerData::Read());
    }

    inline void Write(ClockingMode_t value)
    {
        SequencerData::Write(SequencerRegister::ClockingMode, SequencerData_t(value));
    }

    inline uint1_t GetDotClockSelect(ClockingMode_t value)
    {
        return (value & DotClockSelect) >> Shift::DotClockSelect;
    }

    inline uint1_t GetLoadSerializersEverySecondCharacterClock(ClockingMode_t value)
    {
        return (value & LoadSerializersEverySecondCharacterClock) >> Shift::LoadSerializersEverySecondCharacterClock;
    }

    inline uint1_t GetInternalCharacterClock(ClockingMode_t value)
    {
        return (value & InternalCharacterClock) >> Shift::InternalCharacterClock;
    }
    
    inline uint1_t GetLoadSerializersEveryFourthCharacterClock(ClockingMode_t value)
    {
        return (value & LoadSerializersEveryFourthCaracterClock) >> Shift::LoadSerializersEveryFourthCaracterClock;
    }
    
    inline uint1_t GetScreenOff(ClockingMode_t value)
    {
        return (value & ScreenOff) >> Shift::ScreenOff;
    }

    inline ClockingMode_t SetDotClockSelect(uint1_t value)
    {
        return ClockingMode_t((value << Shift::DotClockSelect) & DotClockSelect);
    }
    
    inline ClockingMode_t SetLoadSerializersEverySecondCharacterClock(uint1_t value)
    {
        return ClockingMode_t((value << Shift::LoadSerializersEverySecondCharacterClock) & LoadSerializersEverySecondCharacterClock);
    }
    
    inline ClockingMode_t SetInternalCharacterClock(uint1_t value)
    {
        return ClockingMode_t((value << Shift::InternalCharacterClock) & InternalCharacterClock);
    }
    
    inline ClockingMode_t SetLoadSerializersEveryFourthCharacterClock(uint1_t value)
    {
        return ClockingMode_t((value << Shift::LoadSerializersEveryFourthCaracterClock) & LoadSerializersEveryFourthCaracterClock);
    }
    
    inline ClockingMode_t SetScreenOff(uint1_t value)
    {
        return ClockingMode_t((value << Shift::ScreenOff) & ScreenOff);
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
