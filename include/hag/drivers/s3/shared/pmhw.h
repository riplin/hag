//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/s3/shared/sqrc/unlexseq.h>
#include <hag/drivers/s3/shared/sqrc/extseqd.h>
#include <hag/drivers/s3/shared/sqrc/rclksync.h>

namespace Hag::S3::Shared
{

namespace PM
{

struct Hardware
{
    inline static void GetSyncControl(bool& canControlHSync, bool& canControlVSync)
    {
        canControlHSync = canControlVSync = true;   
    }

    inline static void GetSyncStatus(bool& statusHSync, bool& statusVSync)
    {
        Sequencer::UnlockExtendedSequencer::SoftUnlock softUnlock;

        Sequencer::ExtendedSequencerD_t value = Sequencer::ExtendedSequencerD::Read();
        statusHSync = (value & Sequencer::ExtendedSequencerD::HSYNCControl) == 0;
        statusVSync = (value & Sequencer::ExtendedSequencerD::VSYNCControl) == 0;
    }

    inline static void SetSyncStatus(bool statusHSync, bool statusVSync)
    {
        Sequencer::UnlockExtendedSequencer::SoftUnlock softUnlock;

        if (!statusHSync || !statusVSync)
        {
            VGA::Sequencer::ClockingMode::TurnScreenOff();

            Sequencer::RAMDACClockSynthControl_t rvalue = Sequencer::RAMDACClockSynthControl::Read();
            rvalue |= Sequencer::RAMDACClockSynthControl::RAMDACPowerDown;
            Sequencer::RAMDACClockSynthControl::Write(rvalue);
            
            Sequencer::ExtendedSequencerD_t evalue =  Sequencer::ExtendedSequencerD::Read();
            evalue &= Sequencer::ExtendedSequencerD_t(~(Sequencer::ExtendedSequencerD::HSYNCControl | Sequencer::ExtendedSequencerD::VSYNCControl));
            if (!statusHSync)
            {
                evalue |= 1 << Sequencer::ExtendedSequencerD::Shift::HSYNCControl;
            }
            if (!statusVSync)
            {
                evalue |= 1 << Sequencer::ExtendedSequencerD::Shift::VSYNCControl;
            }
            Sequencer::ExtendedSequencerD::Write(evalue);
        }

        if (statusHSync && statusVSync)
        {
            Sequencer::ExtendedSequencerD_t evalue =  Sequencer::ExtendedSequencerD::Read();
            evalue &= Sequencer::ExtendedSequencerD_t(~(Sequencer::ExtendedSequencerD::HSYNCControl | Sequencer::ExtendedSequencerD::VSYNCControl));
            Sequencer::ExtendedSequencerD::Write(evalue);
            
            Sequencer::RAMDACClockSynthControl_t rvalue = Sequencer::RAMDACClockSynthControl::Read();
            rvalue &= Sequencer::RAMDACClockSynthControl_t(~Sequencer::RAMDACClockSynthControl::RAMDACPowerDown);
            Sequencer::RAMDACClockSynthControl::Write(rvalue);

            VGA::Sequencer::ClockingMode::TurnScreenOn();
        }
    }
};

}

}
