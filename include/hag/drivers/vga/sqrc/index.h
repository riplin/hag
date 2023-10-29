//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/sqrc/regs.h>

namespace Hag { namespace VGA
{

typedef uint8_t SequencerIndex_t;

namespace SequencerIndex
{

    enum //Mask
    {
        SequencerRegisterIndex = 0x1F        //SEQ ADDRESS 
    };

    namespace Shift
    {
        enum
        {
            SequencerRegisterIndex = 0x00
        };
    }

    inline SequencerIndex_t Read()
    {
        return SequencerIndex_t(SYS_ReadPortByte(Register::SequencerIndex));
    }

    inline void Write(SequencerIndex_t value)
    {
        SYS_WritePortByte(Register::SequencerIndex, value);
    }

    inline uint7_t GetSequenceRegisterIndex(SequencerIndex_t value)
    {
        return (value & SequencerRegisterIndex) >> Shift::SequencerRegisterIndex;
    }

    inline SequencerIndex_t SetSequenceRegisterIndex(uint7_t value)
    {
        return SequencerIndex_t((value << Shift::SequencerRegisterIndex) & SequencerRegisterIndex);
    }

}

}}
