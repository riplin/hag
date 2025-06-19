//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/data.h>

namespace Hag::Matrox::Shared::CRTCExtension
{

namespace Register
{

enum
{
    VerticalCounterExtensions = 0x02,                         //CER2
};

}

typedef Shared::CRTCExtensionData_t VerticalCounterExtensions_t;

namespace VerticalCounterExtensions
{
    
    enum //Mask
    {
        VerticalTotal11_10 = 0x03,          //Vertical total bits 11-10
        VeticalDisplayEnableEnd10 = 0x04,   //Vertical display enable end bit 10
        VerticalBlankingStart11_10 = 0x18,  //Vertical blanking start bits 11-10
        VerticalRetraceStart11_10 = 0x60,   //Vertical retrace start bits 11-10
        LineCompare10 = 0x80                //Line compare bit 10
    };

    namespace Shift
    {
        enum
        {
            VerticalTotal11_10 = 0x00,
            VeticalDisplayEnableEnd10 = 0x02,
            VerticalBlankingStart11_10 = 0x03,
            VerticalRetraceStart11_10 = 0x05,
            LineCompare10 = 0x07
        };
    }

    inline VerticalCounterExtensions_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::VerticalCounterExtensions);
        return VerticalCounterExtensions_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(VerticalCounterExtensions_t value)
    {
        Shared::CRTCExtensionData::Write(Register::VerticalCounterExtensions, Shared::CRTCExtensionData_t(value));
    }

}

}