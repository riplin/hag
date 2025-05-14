//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/regtype.h>

namespace Hag { namespace VGA
{

namespace Register
{
    
enum
{
    FeatureControlWB = 0x3BA,                               //FCR_WT //Mono
    FeatureControlR = 0x3CA,                                //FCR_AD
    FeatureControlWD = 0x3DA,                               //FCR_WT //Color
};

}

typedef uint8_t FeatureControl_t;

namespace FeatureControl
{
    
    enum //Mask
    {
        FeatureControlBit0 = 0x01,              //Feature control bit 0. VGA. General read/write bit.
        FeatureControlBit1 = 0x02               //Feature control bit 1. VGA. General read/write bit.
    };

    namespace Shift
    {
        enum
        {
            FeatureControlBit0 = 0x00,
            FeatureControlBit1 = 0x01
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
