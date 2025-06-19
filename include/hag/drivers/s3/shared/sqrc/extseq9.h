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
    ExtendedSequencer9 = 0x09,              //SR9
};

}

typedef uint8_t ExtendedSequencer9_t;

namespace ExtendedSequencer9
{

    enum //Mask
    {
        MemoryMappedIOOnly = 0x80       //MMIO-ONLY 0 = when MMIO is enabled, both programmed IO and memory=mapped IO register access is allowed.
                                        //          1 = When MMIO is enabled, only memory-mapped IO register accesses are allowed.
    };

    namespace Shift
    {
        enum
        {
            MemoryMappedIOOnly = 0x07
        };
    }

    inline ExtendedSequencer9_t Read()
    {
        VGA::SequencerIndex::Write(Register::ExtendedSequencer9);
        return ExtendedSequencer9_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ExtendedSequencer9_t value)
    {
        VGA::SequencerData::Write(Register::ExtendedSequencer9, VGA::SequencerData_t(value));
    }

}

}
