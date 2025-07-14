//Copyright 2023-Present riplin

#include <hag/system/bda.h>
#include <has/system/pci.h>
#include <has/system/sysasm.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/funcs.h>
#include <hag/drivers/s3/trio32/funcs.h>

namespace Hag::S3::Trio32
{

void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data)
{
    Sequencer::UnlockExtendedSequencer::Unlock();
    Sequencer::ClockSynthControl2_t clockSynthControl2;
    if (idx == 0x22)
    {
        Sequencer::MClockValueLow::Write(data[1]);
        Sequencer::MClockValueHigh::Write(data[0]);
        Sequencer::DClockValueLow::Write(0x49);
        Sequencer::DClockValueHigh::Write(0x55);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                             Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }
    else
    {
        Sequencer::DClockValueLow::Write(data[1]);
        Sequencer::DClockValueHigh::Write(data[0]);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }

    Sequencer::RAMDACClockSynthControl::Write(0);

    Sequencer::ClockSynthControl2_t originalClockSynthControl2 = Sequencer::ClockSynthControl2::Read();

    originalClockSynthControl2 &= ~(Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::DivideDClockByTwo);
    
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2);
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2 | clockSynthControl2);
}

void SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig)
{
    uint32_t offset = (clockConfig & 0x1f) << 1;
    ConfigureDCLKAndMCLK(clockConfig, Data::ClockData + offset);

    CRTController::ModeControl::Write(crtcPort, clockConfig & CRTController::ModeControl::Interlaced);
}

}