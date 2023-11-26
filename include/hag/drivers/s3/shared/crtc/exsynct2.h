//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    ExternalSyncControl2 = 0x57,                            //EX_SYNC_2 CR57
};

}

//VSYN-RESET-ADJUST
//This specifies the vertical delay line number of the V-counter reset from the
//falling edge of VSYNC. The set value must be not equal zero in Remote mode (bit 0 of CR56 set to 1).

typedef uint8_t ExternalSyncControl2_t;

namespace ExternalSyncControl2
{

    inline ExternalSyncControl2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExternalSyncControl2);
        return ExternalSyncControl2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExternalSyncControl2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExternalSyncControl2, VGA::CRTControllerData_t(value));
    }

}

}}}}
