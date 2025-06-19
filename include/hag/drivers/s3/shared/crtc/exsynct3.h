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
    ExternalSyncControl3 = 0x63,                            //EX-SYNC-3 CR63
};

}

typedef uint8_t ExternalSyncControl3_t;

namespace ExternalSyncControl3
{

    enum //Mask
    {
        HSYNCResetAdjust = 0x0F,        //This value specifies the number of character clocks the HSYNC reset in
                                        //the slave is delayed from the falling edge of the VSYNC input from the
                                        //master during genlocking. Remote mode (bit 0 of CRS6 set to 1) must be
                                        //enabled for this field to take effect.
        CharacterClockResetDelay = 0xF0 //This specifies the number of DCLKs to delay the resetting of the
                                        //character clock at the end of the scan line. This is used to sync the
                                        //master and slave character clocks during genlocking. Remote mode (bit 0
                                        //of CR56 set to 1) must be enabled for this field to take effect.
    };

    namespace Shift
    {
        enum
        {
            HSYNCResetAdjust = 0x00,
            CharacterClockResetDelay = 0x04
        };
    }

    inline ExternalSyncControl3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExternalSyncControl3);
        return ExternalSyncControl3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExternalSyncControl3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExternalSyncControl3, VGA::CRTControllerData_t(value));
    }

}

}
