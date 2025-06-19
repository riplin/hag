//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/regtype.h>
#include <hag/drivers/vga/crtc/data.h>

namespace Hag::VGA::CRTController
{

namespace Register
{

enum
{
    StartHorizontalSyncPosition = 0x04,                     //S_H_SY_P CR4
};

}

typedef uint8_t StartHorizontalSyncPosition_t;

namespace StartHorizontalSyncPosition
{
    enum //Mask
    {
        StartHorizontalSyncPositionLow = 0xFF   //9 bit value = character clock counter value at which HSYNC becomes active.
                                                //              This register contains the least significant 8 bits of this value.
    };

    namespace Shift
    {
        enum
        {
            StartHorizontalSyncPositionLow = 0x00
        };
    }

    inline StartHorizontalSyncPosition_t Read(VGA::Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, Register::StartHorizontalSyncPosition);
        return StartHorizontalSyncPosition_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, StartHorizontalSyncPosition_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, Register::StartHorizontalSyncPosition, CRTControllerData_t(value));
    }

}

}
