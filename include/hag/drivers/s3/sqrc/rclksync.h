//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/sqrc/regs.h>

namespace Hag { namespace S3 { namespace Sequencer
{

typedef uint8_t RAMDACClockSynthControl_t;

namespace RAMDACClockSynthControl
{

    enum //Mask
    {
        EnableTestCounter = 0x01,           //TST EN 0 = RAMDAC test counter disabled, 1 = RAMDAC test counter enabled. This bit is used for S3 test purposes only.
        ResetTestCounter = 0x02,            //TST RST 0 = No effect, 1 = Reset the RAMDAC test counter. This bit is used for S3 test purposes only.
        TestRedData = 0x04,                 //TST RED 0 = No effect, 1 = Place red data on internal data bus. This bit is used for S3 test purposes only.
        TestGreenData = 0x08,               //TST GRN 0 = No effect, 1 = Place green data on internal data bus. This bit is used for S3 test purposes only.
        TestBlueData = 0x10,                //TST BLUE 0 = No effect, 1 = Place blue data on internal data bus. This bit is used for S3 test purposes only.
        RAMDACPowerDown = 0x20,             //DAC PD 0 = RAMDAC powered, 1 = RAMDAC powered down. 
                                            //      When the RAMDAC is powered down, the RAMDAC memory retains its data.
        LUTWriteCycleControl = 0x40,        //LUT WR 0 = 2 DCLK LUT write cycle (default), 1 = 1 DCLK LUT write cycle
        EnableClockDoubleMode = 0x80,       //CLKx2 0 = RAMDAC clock doubled mode (0001) disabled, 1 = RAMDAC clock doubled mode (0001) enabled.
                                            //      This bit must be set to 1 when mode 0001 is specified in bits 7-4 of CR67 or SRC.
                                            //      Either bit 4 or bit 6 of SR15 must also be set to 1.
    };

    namespace Shift
    {
        enum
        {
            EnableTestCounter = 0x00,
            ResetTestCounter = 0x01,
            TestRedData = 0x02,
            TestGreenData = 0x03,
            TestBlueData = 0x04,
            RAMDACPowerDown = 0x05,
            LUTWriteCycleControl = 0x06,
            EnableClockDoubleMode = 0x07,
        };
    }

    inline RAMDACClockSynthControl_t Read()
    {
        VGA::SequencerIndex::Write(SequencerRegister::RAMDACClockSynthControl);
        return RAMDACClockSynthControl_t(VGA::SequencerData::Read());
    }
    
    inline void Write(RAMDACClockSynthControl_t value)
    {
        VGA::SequencerData::Write(SequencerRegister::RAMDACClockSynthControl, VGA::SequencerData_t(value));
    }

}

}}}
