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
    ClockSynthControl2 = 0x15,              //SR15
};

}

typedef uint8_t ClockSynthControl2_t;

namespace ClockSynthControl2
{
    
    enum //Mask
    {
        EnableNewMClockFrequencyLoad = 0x01,            //MFRQ EN 0 = Register bit clear, 1 = Load new MCLK frequency
                                                        //      When new MCLK PLL values are programmed, this bit can be set to 1 to load these values in the PLL.
                                                        //      The loading may be dalayed by a s mall but variable amount of time.
                                                        //      This bit should be cleared to 0 after loading to prevent repeated loading.
                                                        //      Alternately, use bit 5 of this register to produce an immediate load.
        EnableNewDClockFrequencyLoad = 0x02,            //DFRQ EN 0 = Register bit clear, 1 = Load new DCLK frequency
                                                        //      When new DCLK PLL values are programmed, this bit can be set to 1 to load these values in the PLL.
                                                        //      Bits 3-2 of 0x3C2 must also be set to 11b if they are not already at this value.
                                                        //      The loading may be delayed a small but variable amount of time.
                                                        //      This bit should be programmed to 1 at power-up to allow loading of the VGA DCLK value
                                                        //      and then left at this setting. Use bit 5 of this register to produce an immediate load.
        OutputInternallyGeneratedMClock = 0x04,         //MCLK OUT 0 = Pin 147 acts as the STWR strobe, 1 = Pin 147 outputs the internally generated MCLK.
                                                        //      This is used only for testing.
        VClockDirectionDeterminedByEVClock = 0x08,      //VCLK OUT 0 = Pin 148 outputs the internally generated VCLK regardless of the state of ~EVCLK,
                                                        //         1 = VCLK direction is determined by the ~EVCLK signal.
        DivideDClockByTwo = 0x10,                       //DCLK/2 0 = DCLK unchanged, 1 = Divide DCLK by 2.
                                                        //      Either this bit or bit 6 of this register must be set to 1 for clock doubled RAMDAC operation (mode 0001).
        MClockDClockLoad = 0x20,                        //0 = Cloack loading is controlled by bits 0 and 1 of this register, 1 = Load MCLK and DCLK PLL values immediately.
                                                        //      To produce an immediate MCLK and DCLK load, program this bit to 1 and then to 0.
                                                        //      Bits 3-2 of 0x3C2 must also then be programmed to 11b to load the DCLK values if they are not already
                                                        //      programmed to this value. This register must never be left set to 1.
        InvertDClock = 0x40,                            //DCLK INV 0 = DCLK unchanged, 1 = Invert DCLK.
                                                        //      Either this bit or bit 4 of this register must be set to 1 for clock doubled RAMDAC operation (mode 0001).
        EnableTwoCycleMemoryWrite = 0x80                //2 CYC MWR 0 = 3 MCLK memory write, 1 = 2 MCLK memory write.
                                                        //      Setting this bit to 1 bypasses the VGA logic for linear addressing when bit 7 of SRA is set to 1.
                                                        //      This can allow 2 MCLK operation for MCLK frequencies between 55 and 57 MHz.
    };

    namespace Shift
    {
        enum
        {
            EnableNewMClockFrequencyLoad = 0x00,
            EnableNewDClockFrequencyLoad = 0x01,
            OutputInternallyGeneratedMClock = 0x02,
            VClockDirectionDeterminedByEVClock = 0x03,
            DivideDClockByTwo = 0x04,
            MClockDClockLoad = 0x05,
            InvertDClock = 0x06,
            EnableTwoCycleMemoryWrite = 0x07
        };
    }

    inline ClockSynthControl2_t Read()
    {
        VGA::SequencerIndex::Write(Register::ClockSynthControl2);
        return ClockSynthControl2_t(VGA::SequencerData::Read());
    }
    
    inline void Write(ClockSynthControl2_t value)
    {
        VGA::SequencerData::Write(Register::ClockSynthControl2, VGA::SequencerData_t(value));
    }

}

}
