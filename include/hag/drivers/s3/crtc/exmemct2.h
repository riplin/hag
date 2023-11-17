//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedMemoryControl2_t;

namespace ExtendedMemoryControl2
{

    enum //Mask
    {
        AccessCycles = 0xF8,
        AccessCycles1 = 0x00,
        AccessCycles2 = 0x08,
        AccessCycles3 = 0x10,
        AccessCycles4 = 0x18,
        AccessCycles5 = 0x20,
        AccessCycles6 = 0x28,
        AccessCycles7 = 0x30,
        AccessCycles8 = 0x38,
        AccessCycles9 = 0x40,
        AccessCycles10 = 0x48,
        AccessCycles11 = 0x50,
        AccessCycles12 = 0x58,
        AccessCycles13 = 0x60,
        AccessCycles14 = 0x68,
        AccessCycles15 = 0x70,
        AccessCycles16 = 0x78,
        AccessCycles17 = 0x80,
        AccessCycles18 = 0x88,
        AccessCycles19 = 0x90,
        AccessCycles20 = 0x98,
        AccessCycles21 = 0xA0,
        AccessCycles22 = 0xA8,
        AccessCycles23 = 0xB0,
        AccessCycles24 = 0xB8,
        AccessCycles25 = 0xC0,
        AccessCycles26 = 0xC8,
        AccessCycles27 = 0xD0,
        AccessCycles28 = 0xD8,
        AccessCycles29 = 0xE0,
        AccessCycles30 = 0xE8,
        AccessCycles31 = 0xF0,
        AccessCycles32 = 0xF8
    };

    namespace Shift
    {
        enum
        {
            AccessCycles = 0x03
        };
    }

    inline ExtendedMemoryControl2_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMemoryControl2);
        return ExtendedMemoryControl2_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedMemoryControl2_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedMemoryControl2, VGA::CRTControllerData_t(value));
    }

}

}}}
