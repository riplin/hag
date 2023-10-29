//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/sqrc/regs.h>

namespace Hag { namespace S3 { namespace Sequencer
{

typedef uint8_t ExtendedSequencerD_t;

namespace ExtendedSequencerD
{

    enum //Mask
    {
        EnableFeatureConnector = 0x01,  //EN-FEAT 0 = ~ENFEAT (pin 151) is high. VCLK, HCYNC and VSYNC are outputs. Pin 155
                                        //            (~BGNT/~BLANK) functions as ~BGNT.
                                        //        1 = ~ENFEAT (pin 151) is low. Pin 155 (~BGNT/~BLANK) functions as ~BLANK.
                                        //            The direction of VCLK is controlled by EVCLK and the direction of ~BLANK, HSYNC and
                                        //            VSYNC is controlled by ESYNC. In both cases, assertion (low) specifies an input
                                        //            and a logic high specifies an output.
                                        //
                                        //        This bit is set to 1 to driver pin 151 with a logic 0. This enables the feature connector
                                        //        buffers required when the Trio64 memory configuration is 2MBytes or larger.
                                        //        It also controls the ESYNC and EVCLK functions as explained above for both the Trio32 and Trio64.
        HSYNCControl = 0x30,            //HSY-CTL 00 = Normal operation
                                        //        01 = HSYNC = 0
                                        //        10 = HSYNC = 1
                                        //        11 = Reserved
        VSYNCControl = 0xC0             //VSY-CTL 00 = Normal operation
                                        //        01 = VSYNC = 0
                                        //        10 = VSYNC = 1
                                        //        11 = Reserved
    };

    namespace Shift
    {
        enum
        {
            EnableFeatureConnector = 0x00,
            HSYNCControl = 0x04,
            VSYNCControl = 0x06
        };
    }

    inline ExtendedSequencerD_t Read()
    {
        VGA::SequencerIndex::Write(SequencerRegister::ExtendedSequencerD);
        return ExtendedSequencerD_t(VGA::SequencerData::Read());
    }

    inline void Write(ExtendedSequencerD_t value)
    {
        VGA::SequencerData::Write(SequencerRegister::ExtendedSequencerD, VGA::SequencerData_t(value));
    }

}

}}}
