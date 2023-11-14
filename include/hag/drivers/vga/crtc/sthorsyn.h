//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>

namespace Hag { namespace VGA { namespace CRTController
{

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

    inline StartHorizontalSyncPosition_t Read(Register_t controllerIndexRegister)
    {
        CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::StartHorizontalSyncPosition);
        return StartHorizontalSyncPosition_t(CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(Register_t controllerIndexRegister, StartHorizontalSyncPosition_t value)
    {
        CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::StartHorizontalSyncPosition, CRTControllerData_t(value));
    }

}

}}}
