//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/crtc/regs.h>

namespace Hag { namespace S3 { namespace CRTController
{

typedef uint8_t ExtendedSystemControl3_t;

namespace ExtendedSystemControl3
{

    enum //Mask
    {
        DisplayStartAddressHigh = 0x0F      //This field contains the upper 4 bits (19-16) of the display start address, allowing
                                            //addressing of up to 4 MBytes of display memory. When a non-zero value is programmed
                                            //in this field, bits 5-4 of CR31 and 1-0 of CR51 (the old display start address bits) are ignored.
                                            //The Trio32 only supports 2MBytes of memory, so the upper bit is not used.
    };

    namespace Shift
    {
        enum
        {
            DisplayStartAddressHigh = 0x00
        };
    }

    inline ExtendedSystemControl3_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, CRTControllerRegister::ExtendedSystemControl3);
        return ExtendedSystemControl3_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedSystemControl3_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, CRTControllerRegister::ExtendedSystemControl3, VGA::CRTControllerData_t(value));
    }

}

}}}
