//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedSystemControl1_t;

namespace ExtendedSystemControl1
{

    enum //Mask
    {
        ScreenWidthDefMsb = 0x01,                   //Extension bit 2 of the screen width definition. See bits 7-6 below.
        Unknown = 0x02,                             //Undocumented flag that's used.
        EnableBREQFunction = 0x04,                  //0 = ~BREQ, ~BGNT functions disabled.
                                                    //1 = ~BREQ, ~BGNT functions enabled.
        PixelLengthSelect = 0x30,                   //00 = 1 byte (default). This corresponds to a pixel length of 4 or
        Length4Or8Bpp = 0x00,                       //     8 bits/pixel in bit 7 of the Subsystem Status register (0x42E8).
        Length16Bpp = 0x10,                         //01 = 2 bytes. 16 bits/pixel
                                                    //10 = Reserved
        Length32Bpp = 0x30,                         //11 = 4 bytes. 32 bits/pixel
        GfxEngingeCommandScreenPixelWidthLsb = 0xC0,//Graphics Engine Command Screen Pixel Width
                                                    //Bit 0 of this register is the most significan bit of this definition.
        Width1024 = 0x00,                           //000 = 1024 (or 2048 if bit 1 of CR31 = 1) (Default)
        Width640  = 0x40,                           //001 = 640
        Width800  = 0x80,                           //010 = 800
        Width1280 = 0xC0,                           //011 = 1280
        Width1152 = 0x01,                           //100 = 1152
                                                    //101 = Reserved
        Width1600 = 0x81                            //110 = 1600
                                                    //111 = Reserved
    };

    namespace Shift
    {
        enum
        {
            ScreenWidthDefMsb = 0x00,
            EnableBREQFunction = 0x02,
            PixelLengthSelect = 0x04,
            GfxEngingeCommandScreenPixelWidthLsb = 0x06
        };
    }

    inline ExtendedSystemControl1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedSystemControl2);
        return ExtendedSystemControl1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedSystemControl1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedSystemControl2, VGA::CRTControllerData_t(value));
    }

}

}}}
