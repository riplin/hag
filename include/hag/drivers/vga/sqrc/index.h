//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>
#include <hag/drivers/vga/sqrc/regtype.h>

namespace Hag::VGA
{

namespace Register
{
    
enum
{
    SequencerIndex = 0x3C4,                                 //SEQX
};

}

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

    inline Sequencer::Register_t Read()
    {
        return Sequencer::Register_t(SYS_ReadPortByte(Register::SequencerIndex));
    }

    inline void Write(Sequencer::Register_t value)
    {
        SYS_WritePortByte(Register::SequencerIndex, value);
    }

}

}
