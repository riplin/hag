//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/data.h>
#include <hag/drivers/matrox/shared/crtc/regtype.h>

namespace Hag { namespace Matrox { namespace Shared { namespace CRTCExtension
{

namespace Register
{

enum
{
    HorizontalCounterExtensions = 0x01,                         //CER1
};

}

typedef Shared::CRTCExtensionData_t HorizontalCounterExtensions_t;

namespace HorizontalCounterExtensions
{
    
    enum //Mask
    {
        HorizontalTotal8 = 0x01,            //Horizontal total bit 8
        HorizontalBlankingStart8 = 0x02,    //Horizontal blanking start bit 8
        HorizontalRetraceStart8 = 0x04,     //Horizontal retrace start bit 8
        HorizontalResetEnable = 0x08,       //When at '1', the horizontal counter can be reset by the VIDRST pin.
        HorizontalSyncOff = 0x10,           //0 = HSYNC runs freely, 1 = HSYNC is forced inactive.
        VerticalSyncOff = 0x20,             //0 = VSYNC runs freely, 1 = VSYNC is forced inactive.
        HorizontalBlankingEnd6 = 0x40,      //Horizontal blanking end bit 6. Only used in MGA mode.
        VerticalResetEnable = 0x80          //When at '1', the vertical counter can be reset  by the VIDRST pin.
    };

    namespace Shift
    {
        enum
        {
            HorizontalTotal8 = 0x00,
            HorizontalBlankingStart8 = 0x01,
            HorizontalRetraceStart8 = 0x02,
            HorizontalResetEnable = 0x03,
            HorizontalSyncOff = 0x04,
            VerticalSyncOff = 0x05,
            HorizontalBlankingEnd6 = 0x06,
            VerticalResetEnable = 0x07
        };
    }

    inline HorizontalCounterExtensions_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::HorizontalCounterExtensions);
        return HorizontalCounterExtensions_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(HorizontalCounterExtensions_t value)
    {
        Shared::CRTCExtensionData::Write(Register::HorizontalCounterExtensions, Shared::CRTCExtensionData_t(value));
    }

}

}}}}