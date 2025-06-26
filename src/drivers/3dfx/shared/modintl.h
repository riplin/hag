//Copyright 2025-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/vga/extmsapi.h>

#include <hag/drivers/3dfx/shared/regs/pllctrl0.h>
#include <hag/drivers/3dfx/shared/crtc/horext.h>
#include <hag/drivers/3dfx/shared/crtc/verext.h>

namespace Hag::TDfx::Shared::Function::ModeSetting
{

namespace Flags //Internal flags in addition to the public flags.
{
    enum
    {
        ModeType = VGA::ModeSetting::Flags::DriverFlag0,
        VGA = 0x0000,
        TDfx = VGA::ModeSetting::Flags::DriverFlag0,
    };
}

typedef uint8_t Configuration_t;
namespace Configuration
{
    enum
    {
        ExtendedVideoShiftOut = 0x01,
        Interlace = 0x2,
        HalfMode = 0x4
    };
}

#pragma pack(push, 1)
struct ResolutionTimings : VGA::ModeSetting::ResolutionTimings
{
    PLLControl0_t PLLControl0;
    Configuration_t Config;
    CRTController::HorizontalExtension_t HorizontalExtensions;
    CRTController::VerticalExtension_t VerticalExtensions;
};

//I really wish I didn't have to do this, but the offset calculation for 400x300 and 512x384 broke me.
struct ModeDescriptor : VGA::ModeSetting::ModeDescriptor
{
    VGA::CRTController::ScreenOffset_t Offset;
};
#pragma pack(pop)

}

namespace Hag::TDfx::Shared::Data
{

extern Function::ModeSetting::ModeDescriptor s_Descriptors[];
extern uint32_t s_NumDescriptors;

}
