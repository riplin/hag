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
    MemoryModeControl = 0x04,               //MEM_MODE SR4
};

}

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
        SequencerIndex::Write(Register::MemoryModeControl);
        return MemoryModeControl_t(SequencerData::Read());
    }

    inline void Write(MemoryModeControl_t value)
    {
        SequencerData::Write(Register::MemoryModeControl, SequencerData_t(value));
    }
}

}}}
