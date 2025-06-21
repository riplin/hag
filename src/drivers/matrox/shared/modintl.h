//Copyright 2025-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/vga/extmsapi.h>
#include <hag/drivers/matrox/shared/data.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include <hag/drivers/matrox/shared/crtcext/hrcntext.h>
#include <hag/drivers/matrox/shared/crtcext/vrcntext.h>

namespace Hag::Matrox::Shared::Function::ModeSetting
{

namespace Flags //Internal flags in addition to the public flags.
{
    enum
    {
        ModeType = VGA::ModeSetting::Flags::DriverFlag0,
        VGA = 0x0000,
        Matrox = VGA::ModeSetting::Flags::DriverFlag0,
        MAFCSelection = VGA::ModeSetting::Flags::DriverFlag1,
        MAFCVGA = 0x0000,
        MAFCDisable = VGA::ModeSetting::Flags::DriverFlag1,
    };
}

#pragma pack(push, 1)
struct ResolutionTimings : VGA::ModeSetting::ResolutionTimings
{
    uint32_t FrequencyKHz;
    CRTCExtension::HorizontalCounterExtensions_t HorizontalCounterExtensions;
    CRTCExtension::VerticalCounterExtensions_t VerticalCounterExtensions;    
};
#pragma pack(pop)

}

namespace Hag::Matrox::Shared::Data
{

extern VGA::ModeSetting::ModeDescriptor s_Descriptors[];
extern uint32_t s_NumDescriptors;

}
