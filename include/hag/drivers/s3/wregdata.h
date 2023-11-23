//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/s3/regs.h>
#include <hag/system/sysasm.h>

namespace Hag { namespace S3
{

typedef uint16_t WriteRegisterData_t;

typedef uint16_t ReadSelect_t;

typedef uint16_t MultifunctionControlMiscellaneous2_t;

namespace MultifunctionControlMiscellaneous2
{
    enum
    {
        DestinationBaseAddress = 0x0007,        //000 = First destination memory address is in the 1st MByte of display memory
                                                //001 = First destination memory address is in the 2nd MByte of display memory
                                                //010 = First destination memory address is in the 3rd MByte of display memory
                                                //011 = First destination memory address is in the 4th MByte of display memory
                                                //This field supersedes bits 1-0 of BEE8H Index E 
                                                //(MultifunctionControlMiscellaneous) if any of these 3 bits are 
                                                //set to 1. The 3- and 4-MByte settings apply only to the Tri064.
        SourceBaseAddress = 0x0070,             //000 = First source memory address is in the 1st MByte of display memory
                                                //001 = First source memory address is in the 2nd MByte of display memory
                                                //010 = First source memory address is in the 3rd MByte of display memory
                                                //011 = First source memory address is in the 4th MByte of display memory
                                                //This field supersedes bits 3-2 of BEE8H Index E
                                                //(MultifunctionControlMiscellaneous) if any of these three bits are
                                                //set to 1. The 3- and 4-MByte settings apply only to the Tri064.
    };
}

typedef uint16_t MultifunctionControlMiscellaneous_t;

namespace MultifunctionControlMiscellaneous
{
    enum
    {
        DestinationBaseAddress = 0x0003,        //Destination Base Address Bits 21-20
                                                //00 = First destination memory address is in the 1st MByte of display memory
                                                //01 = First destination memory address is in the 2nd MByte of display memory
                                                //10 = First destination memory address is in the 3rd MByte of display memory
                                                //11 = First destination memory address is in the 4th MByte of display memory
                                                //This field is superseded by bits 2-0 of BEE8H, Index D if any of the BEE8H
                                                //Index D bits is set to 1. The 3- and 4-MByte settings apply only to the Tri064.
        SourceBaseAddress = 0x000C,             //Source Base Address Bits 21-20
                                                //00 = First source memory address is in the 1st MByte of display memory
                                                //01 = First source memory address is in the 2nd MByte of display memory
                                                //10 = First source memory address is in the 3rd MByte of display memory
                                                //11 = First source memory address is in the 4th MByte of display memory
                                                //This field is superseded by bits 6-4 of BEE8H Index D if any of the BEE8H
                                                //Index D bits is set to 1. The 3- and 4-MByte settings apply only to the Tri064.
        SelectUpperWordIn32BppMode = 0x0010,    //Select Upper Word in 32 Bits/Pixel Mode
                                                //0 = Selects lower 16 bits for accesses to 32-bit registers in 32 bpp mode
                                                //1 = Selects upper 16 bits for accesses to 32-bit registers in 32 bpp mode
        EnableExternalClipping = 0x0020,        //0 = Only pixels inside the clipping rectangle are drawn
                                                //1 = Only pixels outside the clipping rectangle are drawn
        SelectSlowReadModifyWriteCycle = 0x0040,//0 = Fast Read/Modify/Write Cycle
                                                //1 = Slow Read/Modify/Write Cycle
                                                //When the Graphics Engine does a raster operation involving current screen data,
                                                //it must do a read/modify/write cycle. For a fast cycle, the read, modify and
                                                //write each take one MCLK. For a slow cycle, a wait state is inserted so that
                                                //the entire process re- quires 4 MCLKs.
        SourceNotEqual = 0x0080,                //Don't Update Bitmap if Source Not Equal to Color Compare Color
                                                //0 = Don't update current bitmap if the Color Compare (B2E8) register value
                                                //is equal to the color value of the source bitmap
                                                //1 = Don't update current bitmap if the Color Compare (B2E8) register value
                                                //is not equal to the color value of the source bitmap
                                                //This bit is only active if bit 8 of this register is set to 1.
        EnableColorCompare = 0x0100,            //0 = Disable color comparison
                                                //1 = Enable color comparison
        Select32BitCommandRegisters = 0x0200    //Select 32-Bit Command Registers
                                                //0 = Command registers (A2E8H, A6E8H, AAE8H, AEE8H, B2E8H) are 16-bit
                                                //1 = Command registers (A2E8H, A6E8H, AAE8H, AEE8H, B2E8H) are 32-bit. Byte and
                                                //    word accesses cannot be made.
                                                //This bit applies to programmed I/O accesses only and is a don't care for MMIO accesses.

    };
}

typedef uint16_t PixelControl_t;

namespace PixelControl
{
    enum
    {
        SelectMixMask = 0x00C0,
        MixForeground = 0x0000,
        MixCpuControlled = 0x0080,
        MixDisplayMemoryControlled = 0x00C0
    };
}

typedef uint16_t MinorAxisPixelCount_t;

namespace MinorAxisPixelCount
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };
}

typedef uint16_t TopScissors_t;

namespace TopScissors
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };
}

typedef uint16_t LeftScissors_t;

namespace LeftScissors
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };
}

typedef uint16_t BottomScissors_t;

namespace BottomScissors
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };
}

typedef uint16_t RightScissors_t;

namespace RightScissors
{
    enum
    {
        MinValue = 0x000,
        MaxValue = 0xFFF
    };
}

namespace WriteRegisterData
{

    enum
    {
        MinorAxisPixelCount = 0x0000,
        TopScissors = 0x1000,
        LeftScissors = 0x2000,
        BottomScissors = 0x3000,
        RightScissors = 0x4000,
        PixelControl = 0xA000,
        MultifunctionControlMisc2 = 0xD000,
        MultifunctionControlMisc = 0xE000,
        ReadRegisterSelect = 0xF000,

        MinValue = 0x000,
        MaxValue = 0xFFF
    };

    inline void Write(WriteRegisterData_t value)
    {
        SYS_WritePortShort(Register::WriteRegisterData, value);
    }

    inline void WriteMinorAxisPixelCount(WriteRegisterData_t value) { Write(value | MinorAxisPixelCount); }
    inline void WriteTopScissors(WriteRegisterData_t value) { Write(value | TopScissors); }
    inline void WriteLeftScissors(WriteRegisterData_t value) { Write(value | LeftScissors); }
    inline void WriteBottomScissors(WriteRegisterData_t value) { Write(value | BottomScissors); }
    inline void WriteRightScissors(WriteRegisterData_t value) { Write(value | RightScissors); }
    inline void WritePixelControl(WriteRegisterData_t value) { Write(value | PixelControl); }
    inline void WriteMultifunctionControlMisc2(WriteRegisterData_t value) { Write(value | MultifunctionControlMisc2); }
    inline void WriteMultifunctionControlMisc(WriteRegisterData_t value) { Write(value | MultifunctionControlMisc); }
    inline void WriteReadRegisterSelect(WriteRegisterData_t value) { Write(value | ReadRegisterSelect); }
}

}}
