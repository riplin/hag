//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/s3/crtc/extrdctl.h>
#include <hag/drivers/s3/crtc/genout.h>
#include <hag/drivers/s3/sqrc/extseqd.h>

namespace Hag { namespace S3
{

    template <VGA::Register_t CrtControllerIndex>
    struct DDCHardware
    {
        //I wish I could have used a struct with a cast operator, but Watcom doesn't return the proper value. :(
        inline static unsigned short CRTCIndex() { return CrtControllerIndex; }
        inline static unsigned short InputStatus1() { return CrtControllerIndex + 0x06; }

        inline static void Begin()
        {
            CRTController::ExtendedRAMDACControl_t value = CRTController::ExtendedRAMDACControl::Read(CRTCIndex());
            value |= CRTController::ExtendedRAMDACControl::EnableGeneralInputPortRead;
            CRTController::ExtendedRAMDACControl::Write(CrtControllerIndex, value);
        }

        inline static void WriteClockAndData(uint2_t clockData) //bit 1 = clock, bit 0 = data
        {
            CRTController::GeneralOutputPort_t value = CRTController::GeneralOutputPort::Read(CRTCIndex());
            value &= 0xfc; //Clear out the bottom two bits.
            value |= clockData;
            CRTController::GeneralOutputPort::Write(CRTCIndex(), value);
            SYS_SpinWait(0x113);
        }

        inline static uint1_t ReadData()//Return a single bit
        {
            return uint1_t(SYS_ReadPortByte(VGA::Register::DACWriteIndex) & 0x1);
        }

        inline static void WaitForVSync()
        {
            uint16_t count = 0xffff;
            while (true)
            {
                if (count == 0)
                    return;
                --count;
                VGA::InputStatus1_t value = VGA::InputStatus1::Read(InputStatus1());
                if ((value & VGA::InputStatus1::VerticalSyncActive) != 0)
                    break;
            }

            while(true)
            {
                VGA::InputStatus1_t value = VGA::InputStatus1::Read(InputStatus1());
                if ((value & VGA::InputStatus1::VerticalSyncActive) == 0)
                    break;
            }
        }

        inline static void VSyncControlBegin()
        {
            VGA::Sequencer::ClockingMode::TurnScreenOff();

            //Set VSync to 0.
            Sequencer::ExtendedSequencerD_t evalue = Sequencer::ExtendedSequencerD::Read();
            evalue = (evalue & ~Sequencer::ExtendedSequencerD::VSYNCControl) | Sequencer::ExtendedSequencerD::SetVSYNCControl(0x01);
            Sequencer::ExtendedSequencerD::Write(evalue);
        }

        inline static void SendVSyncPulse()
        {
            Sequencer::ExtendedSequencerD_t value = Sequencer::ExtendedSequencerD::Read();
            value = (value & ~Sequencer::ExtendedSequencerD::VSYNCControl) | Sequencer::ExtendedSequencerD::SetVSYNCControl(0x02);
            Sequencer::ExtendedSequencerD::Write(value);
            SYS_SpinWait(0x113);
            value = (value & ~Sequencer::ExtendedSequencerD::VSYNCControl) | Sequencer::ExtendedSequencerD::SetVSYNCControl(0x01);
            Sequencer::ExtendedSequencerD::Write(value);
            SYS_SpinWait(0x113);
        }

        inline static void VSyncControlEnd()
        {
            //Set VSync to normal operation.
            Sequencer::ExtendedSequencerD_t evalue = Sequencer::ExtendedSequencerD::Read();
            evalue = (evalue & ~Sequencer::ExtendedSequencerD::VSYNCControl) | Sequencer::ExtendedSequencerD::SetVSYNCControl(0x00);
            Sequencer::ExtendedSequencerD::Write(evalue);

            VGA::Sequencer::ClockingMode::TurnScreenOn();
        }

        inline static void End()
        {
            CRTController::ExtendedRAMDACControl_t value = CRTController::ExtendedRAMDACControl::Read(CRTCIndex());
            value &= CRTController::ExtendedRAMDACControl_t(~CRTController::ExtendedRAMDACControl::EnableGeneralInputPortRead);
            CRTController::ExtendedRAMDACControl::Write(CRTCIndex(), value);
        }
    };

    typedef DDCHardware<VGA::Register::CRTControllerIndexB> DDCMonoHardware;
    typedef DDCHardware<VGA::Register::CRTControllerIndexD> DDCColorHardware;

}}
