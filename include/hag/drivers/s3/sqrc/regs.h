//Copyright 2023-Present riplin

#pragma once

namespace Hag { namespace S3 { namespace SequencerRegister
{
    
    enum
    {
        UnlockExtendedSequencer = 0x08,         //UNLK_EXSR SR8
        ExtendedSequencer9 = 0x09,              //SR9
        ExternalBusRequestControl = 0x0A,       //SRA
        MiscellaneousExtendedSequencer = 0x0B,  //SRB
        ExtendedSequencerD = 0x0D,              //SRD
        MClockValueLow = 0x10,                  //UNLK_EXSR SR10
        MClockValueHigh = 0x11,                 //SR11
        DClockValueLow = 0x12,                  //SR12
        DClockValueHigh = 0x13,                 //SR13
        ClockSynthControl1 = 0x14,              //SR14
        ClockSynthControl2 = 0x15,              //SR15
        ClockSynthTestHigh = 0x16,              //SR16
        ClockSynthTestLow = 0x17,               //SR17
        RAMDACClockSynthControl = 0x18,         //SR18
    };

}}}
