//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t Miscellaneous1_t;

namespace Miscellaneous1
{
    enum
    {
        AlternateRefreshCountControl = 0x03,            //If enabled by setting bit 2 of this register to 1, these bits
                                                        //override the refresh count in bit 6 of CR11 and specify the 
                                                        //number of refresh cycles per horizontal line.
        RefreshCount0 = 0x00,
        RefreshCount1 = 0x01,
        RefreshCount2 = 0x02,
        RefreshCount3 = 0x03,
        EnableAlternateRefreshCount = 0x04,             //0 = Alternate refresh count control (bits 1-0) is disabled
                                                        //1 = Alternate refresh count control (bits 1-0) is enabled
        EnableTopMemoryAccess = 0x08,                   //0 = Top of memory access disabled
                                                        //1 = Simultaneous VGA text and Enhanced modes are enabled.
                                                        //CPU and CRTC accesses are then directed to the top 32- or
                                                        //64-KByte area of display memory depending on whether address
                                                        //bit 13 is 0 or 1 respectively.
        Enable8bppOrGreaterColorEnhancedMode = 0x10,    //0 = Attribute controller shift registers configured for 4-bit modes
                                                        //1 = Attribute controller shift register configured for 8-, 16- 
                                                        //and 24/32-bit color Enhanced modes
        EnableHighSpeedTextFontWriting = 0x20,          //0 = Disable high speed text font writing
                                                        //1 = Enable high speed text font writing
                                                        //Setting this bit to 1 is only required for DCLK rates greater
                                                        //than 40 MHz. See bit 6 of CR31.
        PCIReadBurstsDisabled = 0x80                    //0 = PCI read burst cycles enabled
                                                        //1 = PCI read burst cycles disabled
                                                        //Note: Bit 7 of CR66 must be set to 1 before this bit is set to 1.
    };

    namespace Shift
    {
        enum
        {
            AlternateRefreshCountControl = 0x00,
            EnableAlternateRefreshCount = 0x02,
            EnableTopMemoryAccess = 0x03,
            Enable8bppOrGreaterColorEnhancedMode = 0x04,
            EnableHighSpeedTextFontWriting = 0x05,
            PCIReadBurstsDisabled = 0x07
        };
    }
    inline Miscellaneous1_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::Miscellaneous1);
        return Miscellaneous1_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, Miscellaneous1_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::Miscellaneous1, VGA::CRTControllerData_t(value));
    }

};

}}}
