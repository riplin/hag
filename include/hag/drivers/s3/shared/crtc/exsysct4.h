//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag::S3::Shared::CRTController
{

namespace Register
{

enum
{
    ExtendedSystemControl4 = 0x6A,                          //EXT-SCTL-4 CR6A
};

}

typedef uint8_t ExtendedSystemControl4_t;

namespace ExtendedSystemControl4
{

    enum //Mask
    {
        CPUBaseAddress = 0x3F   //This field contains the upper 6 bits (19-14) of the CPU base address,
                                //allowing accessing of up to 4 MBytes of display memory via 64K pages.
                                //When a non-zero value is programmed in this field, bits 3-0 of CR35
                                //and 3-2 of CR51 (the old CPU base address bits) are ignored. Bit 0 of
                                //CR31 must be set to 1 to enable this field. If linear addressing is
                                //enabled and a 64 KByte window is specified, these bits specify the
                                //64K page to be accessed at the base address specified in CR59 and CR5A.
                                //Otherwise, the base address is normally at AOOOH. The Trio32 only
                                //supports 2 MBytes of memory, so the upper bit must always be O.
    };

    namespace Shift
    {
        enum
        {
            CPUBaseAddress = 0x00
        };
    }

    inline ExtendedSystemControl4_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::ExtendedSystemControl4);
        return ExtendedSystemControl4_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }
    
    inline void Write(VGA::Register_t controllerIndexRegister, ExtendedSystemControl4_t value)
    {
        VGA::CRTControllerData::Write(controllerIndexRegister, Register::ExtendedSystemControl4, VGA::CRTControllerData_t(value));
    }

}

}
