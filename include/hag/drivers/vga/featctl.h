//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/regs.h>
#include <hag/system/sysasm.h>


namespace Hag { namespace VGA
{

typedef uint8_t FeatureControl_t;

namespace FeatureControl
{
    
    enum //Mask
    {
        VerticalSyncTypeSelect = 0x08 //VSSL 0 = normal vsync, 1 = vsync ORed with vertical active display enable
    };

    namespace Shift
    {
        enum
        {
            VerticalSyncTypeSelect = 0x03
        };
    }

    inline FeatureControl_t Read()
    {
        return FeatureControl_t(SYS_ReadPortByte(Register::FeatureControlR));
    }
    
    inline void Write(Register_t featureControl, FeatureControl_t value)
    {
        SYS_WritePortByte(featureControl, value);
    }

}

}}
