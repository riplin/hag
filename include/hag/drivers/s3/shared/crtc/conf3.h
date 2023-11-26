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
    Configuration3 = 0x68,                                  //CNFG-REG-3 CR68
};

}

//This is the third byte (along with CR36 and CR37) of the power-on strapping bits.
//PD[23:16] are sampled on power-on reset and their states are written to bits 7-0 of this register.
//A5H must be written to CR39 to provide read/write access to this register.

typedef uint8_t Configuration3_t;

namespace Configuration3
{
    enum //Mask
    {
        CASOEStretchTime = 0x03,        //~CAS,~OE Stretch Time, ~WE delay
                                        //00 = approximately 6.S ns stretch (nominal), 2 units WE delay
                                        //01 = approximately 5 ns stretch (nominal), 1 unit WE delay
                                        //10 = approximately 3.5 ns stretch (nominal), no WE delay
                                        //11 = no stretch or delay
                                        //This parameter adjusts the timing for the rising edges of
                                        //the ~CAS and ~OE signals and both falling and rising edges of ~WE.
                                        //This allows stretching of the signal active time for ~CAS and ~OE
                                        //and delaying the entire ~WE signal to allow more time for valid
                                        //pixel data to be available. The delay time shown above is an
                                        //approximation. It is affected by both process and signal loading
                                        //and must be measured for each design.
        RASLowTimingSelect = 0x04,      //0 = 4.S MCLKs
                                        //1 = 3.S MCLKs
                                        //This parameter specifies the length of the RAS active time for a
                                        //single row/column access. RAS may be held low longer to accommodate
                                        //additional page mode accesses to the same row.
        RASPrechargeTiminSelect = 0x08, //0 = 3.S MCLKs
                                        //1 = 2.S MCLKs
                                        //When RAS goes high to end a memory cycle, this parameter specifies
                                        //the minimum period it must be held high before beginning another
                                        //memory access cycle.
        MonitorInformation = 0x70,      //The S3 BIOS uses these bits for monitor information.
        UpperAddressDecode = 0x80       //Upper Address Decode/Memory Data Bus Size
                                        //0 = All 32 bits of the system address bus are decoded (VL-Bus);
                                        //    Trio64 memory data bus is 32 bits.
                                        //1 = SAUP input is decode of upper address lines (VL-Bus);
                                        //    Trio64 memory data bus is 32 bits (1 MByte of memory) or
                                        //    64 bits (2 of more MBytes of memory).
    };

    namespace Shift
    {
        enum
        {
            CASOEStretchTime = 0x00,
            RASLowTimingSelect = 0x02,
            RASPrechargeTiminSelect = 0x03,
            MonitorInformation = 0x04,
            UpperAddressDecode = 0x07
        };
    }

    inline Configuration3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::Configuration3);
        return Configuration3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Configuration3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::Configuration3, VGA::CRTControllerData_t(value));
    }

}

}}}}
