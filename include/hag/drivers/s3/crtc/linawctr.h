//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t LinearAddressWindowControl_t;

namespace LinearAddressWindowControl
{
    enum
    {
        WindowSize = 0x03,
        Window64KiB = 0x00,
        Window1MiB = 0x01,
        Window2MiB = 0x02,
        Window4MiB = 0x03,                  //Trio64 only, not a valid setting for the Trio32
        AddressLatchDelayControl = 0x08,    //VL-bus only
                                            //0 = Address latching is delayed by 1 clock (T2 cycle)
                                            //1 = Address latching occurs in the T1 cycle
                                            //This bit is effective only when one decode wait state
                                            //is selected by setting bit 4 of CR40 to 1.
        EnableLinearAddressing = 0x10,      //0 = Disable linear addressing (default)
                                            //1 = Enable linear addressing
                                            //Enabling linear addressing disables access to the
                                            //AOOOH-AFFFH region unless banking is enabled via
                                            //bit 0 of CR31, the window size is set to 64K via
                                            //bits 1-0 of this register and AOOOH is specified
                                            //as the base in CR59-5A.
        RASPreTimeAdjust = 0x80             //0 = RAS pre-charge (high) time is defined by bit 3 of CR68
                                            //1 = RAS pre-charge (high) time is decreased by 0.5 MCLKs
                                            //    over that specified by bit 3 of CR68 and the corresponding
                                            //    RAS low time (bit 2 of CR68) is increased by 0.5 MCLKs.
                                            //    This leaves the total cycle time unchanged.
    };

    namespace Shift
    {
        enum
        {
            WindowSize = 0x00,
            AddressLatchDelayControl = 0x03,
            EnableLinearAddressing = 0x04,
            RASPreTimeAdjust = 0x07
        };
    }
    inline LinearAddressWindowControl_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::LinearAddressWindowControl);
        return LinearAddressWindowControl_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, LinearAddressWindowControl_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::LinearAddressWindowControl, VGA::CRTControllerData_t(value));
    }

};

}}}
