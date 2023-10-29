//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace VGA { namespace SequencerRegister
{
    
    enum
    {
        Reset = 0x00,                           //RST_SYNC SR0
        ClockingMode = 0x01,                    //CLK_MODE SR1
        EnableWritePlane = 0x02,                //EN_WT_PL SR2
        CharacterFontSelect = 0x03,             //CH_FONT_SL SR3
        MemoryModeControl = 0x04,               //MEM_MODE SR4
    };

}}}
