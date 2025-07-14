//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    GeneralPurposeIOData = 0x2B
};

}
    
typedef uint8_t GeneralPurposeIOData_t;

namespace GeneralPurposeIOData
{
    enum //Mask
    {
        DDCMask = 0x0F,             // DDC pin output state. Controls the output state of the driver on pins DDC<3:0>,
                                    // respectively. On read, this field returns the state of the DDC<3:0> pins.
        DDCPin0Disable = 0x00,
        DDCPin0Enable = 0x01,
        DDCPin1Disable = 0x00,
        DDCPin1Enable = 0x02,
        DDCPin2Disable = 0x00,
        DDCPin2Enable = 0x04,
        DDCPin3Disable = 0x00,
        DDCPin3Enable = 0x08,
        MISCMask = 0x30,            // MISC pin output state. Controls the output state of the driver on pins MISC<1:0>
                                    // during a write operation. On read, this field returns the state of the MISC<1:0> pins.
        MISCPin0Disable = 0x00,
        MISCPin0Enable = 0x10,
        MISCPin1Disable = 0x00,
        MISCPin1Enable = 0x20
    };

    namespace Shift
    {
        enum
        {
            DDCMask = 0x00,
            MISCMask = 0x04,
        };
    }
}

}