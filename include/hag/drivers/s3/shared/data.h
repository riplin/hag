//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/vesa/data.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/drivers/vga/sqrc/memodctl.h>
#include <hag/drivers/s3/shared/advfnctl.h>
#include <hag/drivers/s3/shared/crtc/exhorovf.h>
#include <hag/drivers/s3/shared/crtc/exmscct2.h>
#include <hag/drivers/s3/shared/crtc/exsysct1.h>
#include <hag/drivers/s3/shared/crtc/exsysct2.h>
#include <hag/drivers/s3/shared/crtc/exsysct3.h>
#include <hag/drivers/s3/shared/crtc/exverovf.h>

namespace Hag::S3::Shared
{

typedef uint8_t VESAFlagsAndFilter_t;
namespace VESAFlagsAndFilter
{
    enum
    {
        FilterMask = 0x07,  //Probably refresh rates.
        Filter0 = 0x00,
        Filter1 = 0x01,
        Filter2 = 0x02,
        Filter3 = 0x03,
        Filter4 = 0x04,
        Filter5 = 0x05,
        Filter6 = 0x06,
        Filter7 = 0x07,
        Unknown = 0x40,     //Only used in the highest filter levels of 1280x1024 16 color mode. Interlaced?
        Terminate = 0x80
    };
}

typedef uint5_t ClockConfigurationIndex_t;

typedef uint8_t VESAColorMode_t;

namespace VESAColorMode
{
    enum
    {
        C8bpp1px = 0x00,   //Also for Legacy Mode
        C15bpp1px = 0x01,
        C16bpp1px = 0x02,
        C24bppPacked = 0x03,
        C24bpp1px = 0x04,
        C8bpp2px = 0x05
    };
};

typedef uint8_t VESAVideoModeFlags_t;

namespace VESAVideoModeFlags
{
    enum
    {
        WindowGranularity64KiB = 0x01,
        Color = 0x02,       // 1 = Always 1
        Palette256 = 0x04,  // 2 = unknown
        Unknown3 = 0x30     // 4 = 800x600 and text modes don't have these two bits set
    };
}

struct VESAResolutionVariant
{
    VGA::CRTControllerData_t* CRTCRegisters;
    VESAFlagsAndFilter_t FlagsAndFilter;
    ClockConfigurationIndex_t ClockConfigIndex;
    CRTController::ExtendedSystemControl1_t ExtendedSystemControl1;
    CRTController::ExtendedSystemControl2_t ExtendedMemoryControl2_1MiB;
    CRTController::ExtendedSystemControl2_t ExtendedMemoryControl2;
    CRTController::ExtendedSystemControl3_t ExtendedMemoryControl3_1MiB;
    CRTController::ExtendedSystemControl3_t ExtendedMemoryControl3;
    CRTController::ExtendedHorizontalOverflow_t ExtendedHorizontalOverflow;
    CRTController::ExtendedVerticalOverflow_t ExtendedVerticalOverflow;
    CRTController::ExtendedMiscellaneousControl2_t ExtendedMiscellaneousControl2;
};

struct VESAVideoModeData
{
    VGA::VideoParameters* OverrideTable;
    VESAResolutionVariant* VariantData;
    Vesa::ModeInfo* ModeInfo;
    VGA::VideoMode_t Mode;
    VESAVideoModeFlags_t Flags;    //bit 0 - Window granularity and size: 0 = 32k, 1 = 64k
    AdvancedFunctionControlLower_t AdvancedFunction;
    VESAColorMode_t ColorMode;
    VGA::Sequencer::MemoryModeControl_t MemoryModeControl;
};

typedef uint8_t FirmwareFlag_t;
namespace FirmwareFlag
{
    enum
    {
        Color = 0x01,
        Unknown1 = 0x02,
        Unknown2 = 0x04,
        Unknown3 = 0x08,
        Unknown4 = 0x10,
        Unknown5 = 0x20,
        Unknown6 = 0x40,
        Unknown7 = 0x80
    };
}

struct VESAModeTranslation
{
    Vesa::VideoMode_t VesaMode;
    VGA::VideoMode_t LegacyMode;
};

namespace Data
{

extern VGA::VideoParameters LegacyMode00v1;
extern VGA::VideoParameters LegacyMode02v1;
extern VGA::VideoParameters LegacyMode04v1;
extern VGA::VideoParameters LegacyMode06v1;
extern VGA::VideoParameters LegacyMode07v1;
extern VGA::VideoParameters LegacyMode08v1;
extern VGA::VideoParameters LegacyMode09v1;
extern VGA::VideoParameters LegacyMode0Av1;
extern VGA::VideoParameters LegacyMode0Bv1;
extern VGA::VideoParameters LegacyMode0Cv1;
extern VGA::VideoParameters LegacyMode0Dv1;
extern VGA::VideoParameters LegacyMode0Ev1;
extern VGA::VideoParameters LegacyMode0Fv1;
extern VGA::VideoParameters LegacyMode10v1;
extern VGA::VideoParameters LegacyMode00v2;
extern VGA::VideoParameters LegacyMode02v2;
extern VGA::VideoParameters LegacyMode00v3;
extern VGA::VideoParameters LegacyMode02v3;
extern VGA::VideoParameters LegacyMode07v3;
extern VGA::VideoParameters LegacyMode11v1;
extern VGA::VideoParameters LegacyMode12v1;
extern VGA::VideoParameters LegacyMode13v1;
extern VGA::VideoParameters LegacyMode0FTrio32;

extern Vesa::ModeInfo ModeInfo_49_640x480x8bpp;
extern Vesa::ModeInfo ModeInfo_4A_800x600x4bpp;
extern Vesa::ModeInfo ModeInfo_4B_800x600x8bpp;
extern Vesa::ModeInfo ModeInfo_4C_1024x768x4bpp;
extern Vesa::ModeInfo ModeInfo_4D_1024x768x8bpp;
extern Vesa::ModeInfo ModeInfo_4E_1152x864x8bpp;
extern Vesa::ModeInfo ModeInfo_4F_1280x1024x4bpp;
extern Vesa::ModeInfo ModeInfo_51_640x480x24bpp;
extern Vesa::ModeInfo ModeInfo_52_640x400x32bpp;
extern Vesa::ModeInfo ModeInfo_54_T_132x43x4bpp;
extern Vesa::ModeInfo ModeInfo_55_T_132x25x4bpp;
extern Vesa::ModeInfo ModeInfo_68_G_640x400x8bpp;
extern Vesa::ModeInfo ModeInfo_69_G_640x480x8bpp;
extern Vesa::ModeInfo ModeInfo_6A_G_800x600x4bpp;
extern Vesa::ModeInfo ModeInfo_6B_G_800x600x8bpp;
extern Vesa::ModeInfo ModeInfo_6C_1024x768x4bpp;
extern Vesa::ModeInfo ModeInfo_6D_1024x768x8bpp;
extern Vesa::ModeInfo ModeInfo_6E_1280x1024x4bpp;
extern Vesa::ModeInfo ModeInfo_6F_1280x1024x8bpp;
extern Vesa::ModeInfo ModeInfo_70_640x480x15bpp;
extern Vesa::ModeInfo ModeInfo_71_640x480x16bpp;
extern Vesa::ModeInfo ModeInfo_72_640x480x32bpp;
extern Vesa::ModeInfo ModeInfo_73_800x600x15bpp;
extern Vesa::ModeInfo ModeInfo_74_800x600x16bpp;
extern Vesa::ModeInfo ModeInfo_75_800x600x32bpp;
extern Vesa::ModeInfo ModeInfo_76_1024x768x15bpp;
extern Vesa::ModeInfo ModeInfo_77_1024x768x16bpp;
extern Vesa::ModeInfo ModeInfo_78_1024x768x32bpp;
extern Vesa::ModeInfo ModeInfo_79_1280x1024x15bpp;
extern Vesa::ModeInfo ModeInfo_7A_1280x1024x16bpp;
extern Vesa::ModeInfo ModeInfo_7C_1600x1200x8bpp;

extern VGA::VideoParameters VESAVideoParameters132x43;
extern VGA::VideoParameters VESAVideoParameters132x25;
extern VGA::VideoParameters VESAVideoParameters640x400;
extern VGA::VideoParameters VESAVideoParameters640x480;
extern VGA::VideoParameters VESAVideoParameters800x600;
extern VGA::VideoParameters VESAVideoParameters1024x768;
extern VGA::VideoParameters VESAVideoParameters1280x1024;
extern VGA::VideoParameters VESAVideoParameters1152x864;
extern VGA::VideoParameters VESAVideoParameters1600x1200;

extern VGA::CRTControllerData_t CRTData640x480x1v1[];
extern VGA::CRTControllerData_t CRTData640x480x2v1[];
extern VGA::CRTControllerData_t CRTData800x600x1v1[];
extern VGA::CRTControllerData_t CRTData800x600x2v1[];
extern VGA::CRTControllerData_t CRTData800x600x3v1[];
extern VGA::CRTControllerData_t CRTData1024x768x2v1[];
extern VGA::CRTControllerData_t CRTData1024x768x3v1[];
extern VGA::CRTControllerData_t CRTData1024x768x4v1[];
extern VGA::CRTControllerData_t CRTData1280x1024x2[];
extern VGA::CRTControllerData_t CRTData1280x1024x4[];
extern VGA::CRTControllerData_t CRTData1280x1024x5[];
extern VGA::CRTControllerData_t CRTData1280x1024x6[];
extern VGA::CRTControllerData_t CRTData640x480x0[];
extern VGA::CRTControllerData_t CRTData640x480x1v2[];
extern VGA::CRTControllerData_t CRTData640x480x2v2[];
extern VGA::CRTControllerData_t CRTData800x600x1v2[];
extern VGA::CRTControllerData_t CRTData800x600x2v2[];
extern VGA::CRTControllerData_t CRTData800x600x3v2[];
extern VGA::CRTControllerData_t CRTData1024x768x0[];
extern VGA::CRTControllerData_t CRTData1024x768x2v2[];
extern VGA::CRTControllerData_t CRTData1024x768x3v2[];
extern VGA::CRTControllerData_t CRTData1024x768x4v2[];
extern VGA::CRTControllerData_t CRTData640x480x0v2[];

extern uint8_t CRTControllerInitData[];

extern uint8_t CharacterPatch8x14[];
extern uint8_t CharacterPatch8x16[];
extern uint8_t Characters8x8[];
extern uint8_t Characters8x14[];
extern uint8_t Characters8x16[];

extern uint8_t ColorPalette[];
extern uint8_t MonochromePalette[];
extern uint8_t SecondPalette[];
extern uint8_t Color216Palette[];
extern uint8_t Monochrome64Palette[];
extern uint8_t ColorPalette3[];
extern uint8_t MonochromePalette3[];
extern uint8_t ColorPalette2[];
extern uint8_t MonochromePalette2[];

}

}
