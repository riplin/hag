//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>


namespace Hag { namespace VGA { namespace Sequencer
{

typedef uint8_t MemoryModeControl_t;

namespace MemoryModeControl
{
    enum //Mask
    {
        ExtendedMemoryAddress = 0x02,       //EXT MEM 0 = Memory access restricted to 16/32 KBytes. 1 = Allow complete memory access to 256 KBytes. Required for VGA
        SequentialAddressingMode = 0x04,    //SEQ MODE 0 = CPU writes are odd/even. Even = planes 0 and 2. Odd = planes 1 and 3, 1 = Sequential addressing.
        SelectChain4Mode = 0x08             //CHN 4M 0  = Odd/even mode. 1 = Chain 4 mode. This bit selects modulo 4 addressinf for CPU access to display memory.
    };

    namespace Shift
    {
        enum
        {
            ExtendedMemoryAddress = 0x01,
            SequentialAddressingMode = 0x02,
            SelectChain4Mode = 0x03
        };
    }

    inline MemoryModeControl_t Read()
    {
        SequencerIndex::Write(SequencerRegister::MemoryModeControl);
        return MemoryModeControl_t(SequencerData::Read());
    }

    inline void Write(MemoryModeControl_t value)
    {
        SequencerData::Write(SequencerRegister::MemoryModeControl, SequencerData_t(value));
    }

    inline uint1_t GetExtendedMemoryAddress(MemoryModeControl_t value)
    {
        return (value & ExtendedMemoryAddress) >> Shift::ExtendedMemoryAddress;
    }

    inline uint1_t GetSequentialAddressingMode(MemoryModeControl_t value)
    {
        return (value & SequentialAddressingMode) >> Shift::SequentialAddressingMode;
    }
    
    inline uint1_t GetSelectChain4Mode(MemoryModeControl_t value)
    {
        return (value & SelectChain4Mode) >> Shift::SelectChain4Mode;
    }

    inline MemoryModeControl_t SetExtendedMemoryAddress(uint1_t value)
    {
        return MemoryModeControl_t((value << Shift::ExtendedMemoryAddress) & ExtendedMemoryAddress);
    }
    
    inline MemoryModeControl_t SetSequentialAddressingMode(uint1_t value)
    {
        return MemoryModeControl_t((value << Shift::SequentialAddressingMode) & SequentialAddressingMode);
    }
    
    inline MemoryModeControl_t SetSelectChain4Mode(uint1_t value)
    {
        return MemoryModeControl_t((value << Shift::SelectChain4Mode) & SelectChain4Mode);
    }

}

}}}
