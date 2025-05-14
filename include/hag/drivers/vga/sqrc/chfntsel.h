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
    CharacterFontSelect = 0x03,             //CH_FONT_SL SR3
};

}

typedef uint8_t CharacterFontSelect_t;

namespace CharacterFontSelect
{
    enum //Mask
    {
        SelectFontB1 = 0x03,    //SLB This value selects the portion of plane 2 used to generate text character fonts when bit 3 of the attribute byte is a logical 1, according to the following table:
                                //Bits 4,1,0    Font Table Location     Bits 4,1,0      Font Table Location
                                // 000          First 8K of plane 2     100             Second 8K of plane 2
                                // 001          Third 8K of plane 2     101             Fourth 8K of plane 2
                                // 010          Fifth 8K of plane 2     110             Sixth 8K of plane 2
                                // 011          Seventh 8K of plane 2   111             Eight 8K of plane 2
        SelectFontA1 = 0x0C,    //SLA This value selects the portion of plane 2 used to generate text character fonts when bit 3 of the attribute byte is a logical 0, according to the same table as the character font select B.
        SelectFontB2 = 0x10,
        SelectFontA2 = 0x20,
    };

    namespace Shift
    {
        enum
        {
            SelectFontB1 = 0x00,
            SelectFontA1 = 0x02,
            SelectFontB2 = 0x04,
            SelectFontA2 = 0x05,
        };
    }

    inline CharacterFontSelect_t Read()
    {
        SequencerIndex::Write(Register::CharacterFontSelect); return CharacterFontSelect_t(SequencerData::Read());
    }

    inline void Write(CharacterFontSelect_t value)
    {
        SequencerData::Write(Register::CharacterFontSelect, SequencerData_t(value));
    }

}

}}}
