//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/regtype.h>
#include <hag/drivers/vga/sqrc/data.h>

namespace Hag::VGA::Sequencer
{

namespace Register
{
    
enum
{
    EnableWritePlane = 0x02,                //EN_WT_PL SR2
};

}

typedef uint8_t EnableWritePlane_t;

namespace EnableWritePlane
{
    enum //Mask
    {
        EnableWriteToPlane = 0x0F,  //EN.WT.PL. each bit represents a plane.
        Plane1 = 0x01,
        Plane2 = 0x02,
        Plane3 = 0x04,
        Plane4 = 0x08
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
        SequencerIndex::Write(Register::EnableWritePlane); return EnableWritePlane_t(SequencerData::Read());
    }

    inline void Write(EnableWritePlane_t value)
    {
        SequencerData::Write(Register::EnableWritePlane, SequencerData_t(value));
    }

}

}
