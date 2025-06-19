//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    ExternalSyncControl1 = 0x56,                            //EX_SYNC_1 CR56
};

}

typedef uint8_t ExternalSyncControl1_t;

namespace ExternalSyncControl1
{

    enum //Mask
    {
        RemoteModeOperation = 0x01,                 //0 = Remote Mode operation off
                                                    //1 = Remote Mode operation on. The VSYNC pin becomes the input for genlocking.
        TriStateOffHSYNC = 0x02,                    //0 = HSYNC output buffer tri-stated on
                                                    //1 = HSYNC output buffer tri-stated off
        TriStateOffVSYNC = 0x04,                    //0 = VSYNC output buffer tri-stated on
                                                    //1 = VSYNC output buffer tri-stated off
        ExternalSyncVertCountResetSelect = 0x08,    //0 = HorizontalNertical counter reset sync (Default)
                                                    //1 = Vertical counter-only reset sync with genlocking
                                                    //If bit 0 (Remote Mode) is set to 1, the falling edge of the VSYNC input signal
                                                    //resets the vertical counter (every other frame in the interlaced mode) or both
                                                    //the horizontal and vertical counters.
                                                    //
        PresetFrameSelect = 0x10,                   //0 = Start with the Even Frame after a V-counter reset (Default)
                                                    //1 = Start with the Odd Frame after a V-counter reset
    };

    namespace Shift
    {
        enum
        {
            RemoteModeOperation = 0x00,
            TriStateOffHSYNC = 0x01,
            TriStateOffVSYNC = 0x02,
            ExternalSyncVertCountResetSelect = 0x03,
            PresetFrameSelect = 0x04,
        };
    }

    inline ExternalSyncControl1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExternalSyncControl1);
        return ExternalSyncControl1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExternalSyncControl1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExternalSyncControl1, VGA::CRTControllerData_t(value));
    }

}

}
