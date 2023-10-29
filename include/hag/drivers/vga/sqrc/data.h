//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/index.h>
#include <hag/drivers/vga/sqrc/regs.h>

namespace Hag { namespace VGA
{

typedef uint8_t SequencerData_t;

namespace SequencerData
{
    
    inline SequencerData_t Read()
    {
        return SequencerData_t(SYS_ReadPortByte(Register::SequencerData));
    }

    inline void Write(SequencerData_t value)
    {
        SYS_WritePortByte(Register::SequencerData, value);
    }
    
    inline void Write(SequencerIndex_t sequenceIndex, SequencerData_t value)
    {
        SYS_WritePortBytes(Register::SequencerIndex, sequenceIndex, value);
    }

}

}}