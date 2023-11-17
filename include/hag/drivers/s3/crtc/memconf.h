//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t MemoryConfiguration_t;

namespace MemoryConfiguration
{
    enum
    {
        EnableBaseAddressOffset = 0x01,     //0 = Address offset bits 3-0 of CR35 and bits 3-2
                                            //    of CR51 or the new address offset bits
                                            //    (5-0 of CR6A) are disabled
                                            //1 = Address offset bits 3-0 CR35 and bits 3-2 of CR51 
                                            //    or the new address offset bits (5-0 of CR6A) are
                                            //    enabled for specifying the 64K page of display memory.
                                            //    Bits 5-0 of CR6A are used if this field contains a
                                            //    non-zero value. This allows access to up to 4 MBytes
                                            //    of display memory through a 64K window. (2 MBytes for
                                            //    the Trio32)
        EnableTwoPageScreenImage = 0x02,    //0 = Normal Mode
                                            //1 = Enable 2K x 1K x 4 map image screen for 1024 x 768 or
                                            //    800 x 600 screen resolution, or 2K x 512 x 8 map image
                                            //    screen for 640 x 480 screen resolution
        EnableVGA16BitMemoryBusWidth = 0x04,//0 = 8-bit memory bus operation
                                            //1 = Enable 16-bit bus VGA memory read/writes
                                            //This is useful in VGA text modes when VGA graphics controller
                                            //functions are typically not used.
                                            //
        UseEnhancedModeMemoryMapping = 0x08,//0 = Force IBM VGA mapping for memory accesses
                                            //1 = Force Enhanced Mode mappings
                                            //Setting this bit to 1 overrides the settings of bit 6 of CR14
                                            //and bit 3 of CR17 and causes the use of doubleword memory
                                            //addressing mode. Also, the function of bits 3- 2 of GR6 is
                                            //overridden with a fixed 64K map at A0000H.
        OldDisplayStartAddressBits = 0x30,  //Bits 17-16 of start address (CRC, CRD) and cursor location (CRE, CRF)
                                            //Bits 1-0 of the Extended System Control 2 register (CR51) are bits
                                            //19-18 of the address and enable access to up to 4 MBytes of display memory.
                                            //If a value is programmed into bits 3-0 of the Extended System Control 3
                                            //register (CR69), this value becomes the upper 4 bits of the display start
                                            //base address and bits 5-4 of CR31 and bits 1-0 of CR51 are ignored.
                                            //The Trio32 only supports 2 MBytes, so the upper bit is not used.
        EnableHighSpeedFontFetchMode = 0x40 //0 = Normal font access mode
                                            //1 = Enable high speed text display
                                            //Setting this bit to 1 is only required for DCLK rates greater
                                            //than 40 MHz. See bit 5 of CR3A.
    };

    namespace Shift
    {
        enum
        {
            EnableBaseAddressOffset = 0x00,
            EnableTwoPageScreenImage = 0x01,
            EnableVGA16BitMemoryBusWidth = 0x02,
            UseEnhancedModeMemoryMapping = 0x03,
            OldDisplayStartAddressBits = 0x04,
            EnableHighSpeedFontFetchMode = 0x06
        };
    }
    inline MemoryConfiguration_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::MemoryConfiguration);
        return MemoryConfiguration_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, MemoryConfiguration_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::MemoryConfiguration, VGA::CRTControllerData_t(value));
    }

};

}}}
