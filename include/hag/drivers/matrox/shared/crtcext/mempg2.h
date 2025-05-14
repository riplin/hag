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
    MemoryPage = 0x04,                         //CER4
};

}

typedef Shared::CRTCExtensionData_t MemoryPage_t;

namespace MemoryPage
{
    
    enum //Mask
    {
        Page = 0xFF             //This register provides the extra bits required to address the full frame buffer through
                                //the VGA memory aperture in Power Graphic Mode. This field must be programmed
                                //to zero in VGA Mode. Up to 32 megabytes of memory can be addressed (with the
                                //page8 bit of CRTCEXT8<4> (if available). The page register can be used instead of or in
                                //conjunction with the MGA frame buffer aperture.
    };

    namespace Shift
    {
        enum
        {
            Page = 0x00
        };
    }

    inline MemoryPage_t Read()
    {
        Shared::CRTCExtensionIndex::Write(Register::MemoryPage);
        return MemoryPage_t(Shared::CRTCExtensionData::Read());
    }

    inline void Write(MemoryPage_t value)
    {
        Shared::CRTCExtensionData::Write(Register::MemoryPage, Shared::CRTCExtensionData_t(value));
    }

}

}}}}