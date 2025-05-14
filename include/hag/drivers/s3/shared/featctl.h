//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/s3/shared/regtype.h>
#include <hag/drivers/vga/featctl.h>

namespace Hag { namespace S3 { namespace Shared
{

namespace Register
{
    
enum
{
    FeatureControlWB = VGA::Register::FeatureControlWB,
    FeatureControlR = VGA::Register::FeatureControlR,
    FeatureControlWD = VGA::Register::FeatureControlWD
};

}

typedef VGA::FeatureControl_t FeatureControl_t;

namespace FeatureControl
{
    using namespace VGA::FeatureControl;

    enum //Mask
    {
        VerticalSyncTypeSelect = 0x08 //VSSL 0 = normal vsync, 1 = vsync ORed with vertical active display enable
    };

    namespace Shift
    {
        using namespace VGA::FeatureControl::Shift;

        enum
        {
            VerticalSyncTypeSelect = 0x03
        };
    }

}

}}}
