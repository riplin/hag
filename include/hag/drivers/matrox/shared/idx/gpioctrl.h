//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Matrox::Shared::Indexed
{

namespace Register
{

enum
{
    GeneralPurposeIOControl = 0x2A
};

}

typedef uint8_t GeneralPurposeIOControl_t;

namespace GeneralPurposeIOControl
{
    enum //Mask
    {
        DDCMask = 0x0F,             // DDC pin output control. Controls the output enable of the driver on pins DDC<3:0>,
                                    // respectively.
                                    // 0 = disable the output driver (tri-state, pulled high)
                                    // 1 = enable the output driver
        DDCPin0Disable = 0x00,
        DDCPin0Enable = 0x01,
        DDCPin1Disable = 0x00,
        DDCPin1Enable = 0x02,
        DDCPin2Disable = 0x00,
        DDCPin2Enable = 0x04,
        DDCPin3Disable = 0x00,
        DDCPin3Enable = 0x08,
        MISCMask = 0x30,            // MISC pin output control. Controls the output enable of the driver on pins MISC<1:0>,
                                    // respectively.
                                    // 0 = disable the output driver (tri-state, pulled high)
                                    // 1 = enable the output driver
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