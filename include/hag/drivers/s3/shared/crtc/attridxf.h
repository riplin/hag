//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/regtype.h>

namespace Hag { namespace S3 { namespace Shared { namespace CRTController
{

namespace Register
{

enum
{
    AttributeIndexF = 0x24,                                 //ATC_F CR24 //Same as AttributeIndexI
};

}

//This register is used to read the value of the Attribute Controller Index register
//and its associated internal address flip-flop (AFF). It can be read at either index 24H or 26H.

typedef uint8_t AttributeIndexF_t;

namespace AttributeIndexF
{
    enum //Mask
    {
        AttributeControllerIndex = 0x1F,    //This value is the Attribute Controller Index Data at I/O port 3COH.
        EnableVideoDisplay = 0x20,          //This is the setting of bit 5 of 3COH, indicating video display enabled status (1 = enabled).
        AFF = 0x80                          //Inverted Internal Address flip-flop
    };

    namespace Shift
    {
        enum
        {
            AttributeControllerIndex = 0x00,
            EnableVideoDisplay = 0x05,
            AFF = 0x07
        };
    }

    inline AttributeIndexF_t Read(VGA::Register_t controllerIndexRegister)
    {
        VGA::CRTControllerIndex::Write(controllerIndexRegister, Register::AttributeIndexF);
        return AttributeIndexF_t(VGA::CRTControllerData::Read(controllerIndexRegister + 1));
    }

}

}}}}
