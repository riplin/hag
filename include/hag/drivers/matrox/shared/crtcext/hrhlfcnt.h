//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/matrox/shared/crtcext/data.h>

namespace Hag::Matrox::Shared::CRTCExtension
{

namespace Register
{

enum
{
    HorizontalHalfCount = 0x05,                         //CER5
};

}

typedef Shared::CRTCExtensionData_t HorizontalHalfCount_t;

namespace HorizontalHalfCount
{
    
    enum //Mask
    {
        HalfCount = 0xFF    //Horizontal video half count
                            //This register specifies the horizontal count at which the vertical counter should be
                            //clocked when in interlaced display in field 1. This register is only used in interlaced
                            //mode. The value to program is:
                            //
                            //Start Horizontal Retrace + End Horizontal Retrace - Horizontal Total
                            //--------------------------------------------------------------------    - 1
                            //                              2
    };

    namespace Shift
    {
        enum
        {
            HalfCount = 0x00
        };
    }

    inline HorizontalHalfCount_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::HorizontalHalfCount);
        return HorizontalHalfCount_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(HorizontalHalfCount_t value)
    {
        Shared::CRTCExtensionData::Write(Register::HorizontalHalfCount, Shared::CRTCExtensionData_t(value));
    }

}

}