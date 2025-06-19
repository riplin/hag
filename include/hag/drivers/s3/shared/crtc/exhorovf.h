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
    ExtendedHorizontalOverflow = 0x5D,                      //EXT_H_OVF CR5D
};

}

typedef uint8_t ExtendedHorizontalOverflow_t;

namespace ExtendedHorizontalOverflow
{
    
    enum //Mask
    {
        HorizontalTotalHigh = 0x01,                 //Bit 8 of the HorizontalTotal (CR0)
        HorizontalDisplayEndHigh = 0x02,            //Bit 8 of the HorizontalDisplayEnd (CR1)
        StartHorizontalBlankHigh = 0x04,            //Bit 8 of the StartHorizontalBlank (CR2)
        EndHorizontalBlankPlus64 = 0x08,            //EHB+64 0 = ~BLANK pulse unaffected, 1 = ~BLANK pulse extended by 64 DCLKs.
        StartHorizontalSyncPositionHigh = 0x10,     //Bit 8 of the StartHorizontalSyncPosition (CR4)
        EndHorizontalSyncPlus32 = 0x20,             //EHS+32 0 = HSYNC pulse unaffected, 1 = HSYNC pulse extended by 32 DCLKs.
        StartFIFOFetchHigh = 0x40,                  //Bit 8 of the StartFIFOFetch (CR3B)
        BusGrantTerminatePositionHigh = 0x80,       //Bit 8 of the BusGrantTerminatePosition (CR5F)
    };

    namespace Shift
    {
        enum
        {
            HorizontalTotalHigh = 0x00,
            HorizontalDisplayEndHigh = 0x01,
            StartHorizontalBlankHigh = 0x02,
            EndHorizontalBlankPlus64 = 0x03,
            StartHorizontalSyncPositionHigh = 0x04,
            EndHorizontalSyncPlus32 = 0x05,
            StartFIFOFetchHigh = 0x06,
            BusGrantTerminatePositionHigh = 0x07,
        };
    }

    inline ExtendedHorizontalOverflow_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedHorizontalOverflow);
        return ExtendedHorizontalOverflow_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedHorizontalOverflow_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedHorizontalOverflow, VGA::CRTControllerData_t(value));
    }

}

}
