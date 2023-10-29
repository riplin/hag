//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/sqrc/regs.h>

namespace Hag { namespace S3 { namespace Sequencer
{

typedef uint8_t ExternalBusRequestControl_t;

namespace ExternalBusRequestControl
{

    enum //Mask
    {
        PValue = 0x1F,                  //Integer equivalent of the binary value in this field is the number of 2-MCLK units
                                        //a secondary memory controller is allowed to retain control of the memory bus before
                                        //the Trio32/Trio64 drops its bus grant. Detailed explanation in section 7.5 of Trio32/Trio64 programming manual.
        PDNotTristated = 0x20,          //PD[63:0] not tri-stated (PD[31:0] for Trio32) 0 = tri-stated, 1 = not tri-stated.
                                        //Default value 0 reduces power consumption The pins are enabled for output only as needed.
                                        //Note that output pads for PD[63:24] also latch the most recent output.
        Pin50FunctionSelect = 0x40,     //P50 SEL 0 = If bit 2 of CR36 is set to 1 to indicate fast page memory, pin 50 outputs a signal equivalent to ~OE0.
                                        //        1 = If bit 2 of CR36 is set to 1 to indicate fast page memory, pin 50 outputs ~RAS1.
                                        //This bit must be set to 1 for 4-MByte fast page mode memory configurations.
                                        //This bit has no function if bit 2 of CR36 is cleared to 0 to indicate EDO memory.
        TwoMCLKCPUWrites = 0x80         //2MCLK 0 = 3 MCLK memory writes
                                        //      1 = 2 MCLK memory writes
                                        //Setting this bit to 1 improves performance for systems using an MCLK less than 57MHz.
                                        //For MCLK frequencies between 55 and 57MHz, bit 7 of SR15 should also be set to 1 if linear addressing is being used.
    };

    namespace Shift
    {
        enum
        {
            PValue = 0x00,
            PDNotTristated = 0x05,
            Pin50FunctionSelect = 0x06,
            TwoMCLKCPUWrites = 0x07
        };
    }

    inline ExternalBusRequestControl_t Read()
    {
        VGA::SequencerIndex::Write(SequencerRegister::ExternalBusRequestControl);
        return ExternalBusRequestControl_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ExternalBusRequestControl_t value)
    {
        VGA::SequencerData::Write(SequencerRegister::ExternalBusRequestControl, VGA::SequencerData_t(value));
    }

}

}}}
