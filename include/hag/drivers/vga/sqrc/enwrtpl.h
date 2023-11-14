//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

typedef uint8_t EnableWritePlane_t;

namespace EnableWritePlane
{
    enum //Mask
    {
        EnableWriteToPlane = 0x0F   //EN.WT.PL. each bit represents a plane.
    };

    namespace Shift
    {
        enum
        {
            EnableWriteToPlane = 0x00
        };
    }

    inline EnableWritePlane_t Read()
    {
        SequencerIndex::Write(SequencerRegister::EnableWritePlane); return EnableWritePlane_t(SequencerData::Read());
    }

    inline void Write(EnableWritePlane_t value)
    {
        SequencerData::Write(SequencerRegister::EnableWritePlane, SequencerData_t(value));
    }

}

}}}
