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
    MiscellaneousExtendedSequencer = 0x0B,  //SRB
};

}

typedef uint8_t MiscellaneousExtendedSequencer_t;

namespace MiscellaneousExtendedSequencer
{
    
    enum //Mask
    {
        DotClockVCLKI = 0x01,   //0 = User internal dot clock, 1 = use VCLKI input for all internal dot clock functions
        VAFCVCLKI = 0x02,       //0 = Pixel data from pass-through feature connector latched by incoming VCLK
                                //1 = Pixel data from VAFC latched by VCLKI input
        TwentyFourBPP = 0x08,   //24BPP (Trio32 only)
                                //0 = 24 bits/pixel (Mode 12) disabled
                                //1 = 24 bits/pixel (Mode 12) enabled
                                //In addition, bits 7-4 of this register must be programmed to 0111b and bits 7-4
                                //of CR67 must be programmed to 0000b to enable 24 bits/pixel operation for the Trio32.
        AltColorMode = 0xF0     //ALT COLOR MODE - Color Mode for feature connector input
                                //0000 = Mode 0: 8-bit color, 1 pixel/VCLK
                                //0001 = Mode 8: 8-bit color, 2 pixels/VCLK
                                //0011 = Mode 9: 15-bit color, 1 pixel/VCLK
                                //0101 = Mode 10: 16-bit color, 1 pixel/VCLK
                                //0111 = Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Trio32 only)
                                //1101 = Mode 13: 24-bit color 1 pixel/VCLK
                                //All other mode values are reserved. Setting mode 0001 (clock doubled mode) also
                                //requires that either bit 4 or bit 6 of SR15 be set to 1 and that bit 7 of SR18 be set to 1.
    };

    namespace Shift
    {
        enum
        {
            DotClockVCLKI = 0x00,
            VAFCVCLKI = 0x01,
            TwentyFourBPP = 0x03,
            AltColorMode = 0x04
        };
    }

    inline MiscellaneousExtendedSequencer_t Read()
    {
        VGA::SequencerIndex::Write(Register::MiscellaneousExtendedSequencer);
        return MiscellaneousExtendedSequencer_t(VGA::SequencerData::Read());
    }
    
    inline void Write(MiscellaneousExtendedSequencer_t value)
    {
        VGA::SequencerData::Write(Register::MiscellaneousExtendedSequencer, VGA::SequencerData_t(value));
    }

}

}
