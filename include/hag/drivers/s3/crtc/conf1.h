//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t Configuration1_t;

namespace Configuration1
{
    
    enum //Mask
    {
        SystemBusSelect = 0x03,         //00 = Reserved
        VesaLocalBus = 0x01,            //01 = VESA local bus
        PCIBus = 0x02,                  //10 = PCI local bus
                                        //11 = Reserved
        MemoryPageModeSelect = 0xC0,    //00 = Reserved
                                        //01 = Reserved
                                        //10 = Extended Data Out (EDO) Mode
                                        //11 = Fast Page Mode
        EnableVideoBIOSAccesses = 0x10, //VL-Bus only
                                        //0 = Disable video BIOS accesses
                                        //1 = Enable video BIOS accesses
        DisplayMemorySize = 0xE0,       //
        Size4MiB = 0x00,                //000 = 4MBytes
                                        //001 = Reserved
        Size3MiB = 0x40,                //010 = 3MBytes
                                        //011 = Reserved
        Size2MiB = 0x80,                //100 = 2MBytes
        Size1_5MiB = 0xA,               //101 = 1.5MBytes
        Size1MiB = 0xC0,                //110 = 1MByte
        Size05MiB = 0xE0                //111 = 0.5MByte

    };

    namespace Shift
    {
        enum
        {
            SystemBusSelect = 0x00,
            MemoryPageModeSelect = 0x02,
            EnableVideoBIOSAccesses = 0x04,
            DisplayMemorySize = 0x05
        };
    }

    inline Configuration1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::Configuration1);
        return Configuration1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, Configuration1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::Configuration1, VGA::CRTControllerData_t(value));
    }

    inline uint16_t GetDisplayMemorySizeInKiB(VGA::Register_t controllerIndexRegister)
    {
        Configuration1_t value = Read(controllerIndexRegister);
        uint3_t memSize = (value & DisplayMemorySize) >> Shift::DisplayMemorySize;

        //Negate + 1 makes the range 1 ... 8. Multiplied by 512KiB makes it 0.5KiB ... 4MiB.
        return uint16_t(((~memSize) & 0x07) + 1) << 9;
    }

}

}}}
