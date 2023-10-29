//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>


namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t Configuration2V2_t;

namespace Configuration2V2
{
    
    enum //Mask
    {
        TestMode = 0x01,        //TEST 0 = Test mode enabled, 1 = Normal operation
        ClockSelect = 0x04,     //0 = Use external DCLK on PDOWN (C21) and external MCLK on ENFEAT (A16)
                                //1 = Use internal DCLK, MCLK
        DRAM = 0x60             //00 = Reserved
                                //01 = 4MBytes DRAM
                                //10 = 2MBytes DRAM
                                //11 = 0MBytes DRAM
    };

    namespace Shift
    {
        enum
        {
            TestMode = 0x01,
            ClockSelect = 0x03,
            DRAM = 0x05
        };
    }

    inline Configuration2V2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::Configuration2);
        return Configuration2V2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, Configuration2V2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::Configuration2, VGA::CRTControllerData_t(value));
    }

    inline uint16_t GetDRAMSizeInKiB(VGA::Register_t controllerIndexRegister)
    {
        Configuration2V2_t value = Read(controllerIndexRegister);
        uint2_t memSize = (value & DRAM) >> Shift::DRAM;

        return (uint16_t(~memSize) & 0x03) << 11;
    }

}

}}}
