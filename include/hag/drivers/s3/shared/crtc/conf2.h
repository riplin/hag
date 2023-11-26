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
    Configuration2 = 0x37,                                  //CONFG_REG2 CR37
};

}

//This register samples the reset state from PO bus pins [15:7]. Other configuration strapping bits are found in CR36 and CR68.

typedef uint8_t Configuration2_t;

namespace Configuration2
{
    enum //Mask
    {
        EnableTrio = 0x01,              //Enable Trio32/Trio64 (VL-Bus)
                                        //0 = Disable Trio32/Trio64 except for video BIOS accesses
                                        //1 = Enable Trio32/Trio64
        TestMode = 0x02,                //0 = Test mode enabled (all outputs tri-stated)
                                        //1 = Test mode disabled (normal operation)
        RomSize = 0x04,                 //Video BIOS ROM Size (VL-Bus)
                                        //0 = 64-KByte video BIOS ROM
                                        //1 = 32-KByte video BIOS ROM
        ClockSelect = 0x08,             //0 = Use external DCLK on XIN pin and external MCLK on STRD pin (test purposes only)
                                        //1 = Use internal DCLK, MCLK
        RAMDACWriteSnooping = 0x10,     //0 = Disable ~LOCA/~SRDY for RAMDAC writes
                                        //1 = Enable ~LOCA/~SRDY for RAMDAC writes
        MonitorType = 0xE0              //The S3 BIOS uses these three bits to determine monitor information.
    };

    namespace Shift
    {
        enum
        {
            EnableTrio = 0x00,
            TestMode = 0x01,
            RomSize = 0x02,
            ClockSelect = 0x03,
            RAMDACWriteSnooping = 0x04,
            MonitorType = 0x05
        };
    }

    inline Configuration2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Configuration2);
        return Configuration2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Configuration2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Configuration2, VGA::CRTControllerData_t(value));
    }

}

}}}}
