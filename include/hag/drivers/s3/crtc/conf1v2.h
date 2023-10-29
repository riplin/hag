//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t Configuration1V2_t;

namespace Configuration1V2
{
    
    enum //Mask
    {
        MemoryPageModeSelect = 0x06,            //MEM MODE 
                                                //00 = VRAM 1-cycle EDO mode
                                                //01 = Reserved
                                                //10 = VRAM 2-cycle EDO mode
                                                //11 = VRAM Fast page mode
        MemorySize = 0x60,                      //MEM Size
                                                //00 = 2 MBytes
                                                //01 = 4 MBytes
                                                //10 = 6 MBytes
                                                //11 = 8 MBytes
        EightColumnBlockWriteSupport = 0x80     //8-C
                                                //0 = VRAM does not support 8-column block writes
                                                //1 = VRAM supports 8-column block writes
                                                //If this bit is cleared to 0, block writes cannot be enabled.
    };

    namespace Shift
    {
        enum
        {
            MemoryPageModeSelect = 0x02,
            MemorySize = 0x05,
            EightColumnBlockWriteSupport = 0x07
        };
    }

    inline Configuration1V2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::Configuration1);
        return Configuration1V2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, Configuration1V2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::Configuration1, VGA::CRTControllerData_t(value));
    }

    inline uint16_t GetMemorySizeInKiB(VGA::Register_t controllerIndexRegister)
    {
        Configuration1V2_t value = Read(controllerIndexRegister);
        uint3_t memSize = (value & MemorySize) >> Shift::MemorySize;

        return (memSize + 1) * 2 * 1024;
    }

}

}}}
