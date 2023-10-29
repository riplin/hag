//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA { namespace Sequencer
{

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
            SelectFontB2 = 0x02,
            SelectFontA2 = 0x03,
        };
    }

    inline CharacterFontSelect_t Read()
    {
        SequencerIndex::Write(SequencerRegister::CharacterFontSelect); return CharacterFontSelect_t(SequencerData::Read());
    }

    inline void Write(CharacterFontSelect_t value)
    {
        SequencerData::Write(SequencerRegister::CharacterFontSelect, SequencerData_t(value));
    }

    inline uint2_t GetSelectFontB1(CharacterFontSelect_t value)
    {
        return (value & SelectFontB1) >> Shift::SelectFontB1;
    }

    inline uint2_t GetSelectFontA1(CharacterFontSelect_t value)
    {
        return (value & SelectFontA1) >> Shift::SelectFontA1;
    }

    inline uint1_t GetSelectFontB2(CharacterFontSelect_t value)
    {
        return (value & SelectFontB2) >> Shift::SelectFontB2;
    }

    inline uint1_t GetSelectFontA2(CharacterFontSelect_t value)
    {
        return (value & SelectFontA2) >> Shift::SelectFontA2;
    }

    inline CharacterFontSelect_t SetSelectFontB1(uint2_t value)
    {
        return CharacterFontSelect_t((value << Shift::SelectFontB1) & SelectFontB1);
    }

    inline CharacterFontSelect_t SetSelectFontA1(uint2_t value)
    {
        return CharacterFontSelect_t((value << Shift::SelectFontA1) & SelectFontA1);
    }

    inline CharacterFontSelect_t SetSelectFontB2(uint1_t value)
    {
        return CharacterFontSelect_t((value << Shift::SelectFontB2) & SelectFontB2);
    }

    inline CharacterFontSelect_t SetSelectFontA2(uint1_t value)
    {
        return CharacterFontSelect_t((value << Shift::SelectFontA2) & SelectFontA2);
    }

}

}}}
