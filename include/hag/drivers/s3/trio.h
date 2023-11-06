//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/s3/regs.h>
#include <hag/drivers/s3/advfnctl.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/exsysct1.h>
#include <hag/drivers/s3/crtc/exsysct2.h>
#include <hag/drivers/s3/crtc/exsysct3.h>
#include <hag/drivers/s3/crtc/exhorovf.h>
#include <hag/drivers/s3/crtc/exmscct2.h>
#include <hag/drivers/s3/crtc/exverovf.h>
#include <hag/drivers/s3/vidmodes.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/vga/sqrc/memodctl.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/vesa/vidmodes.h>
#include <hag/driver.h>

#pragma pack(push, 1);

namespace Hag { namespace S3
{

typedef uint8_t VideoModeError_t;

namespace VideoModeError
{
    enum
    {
        Success = 0x00,
        UnknownVideoMode = 0x01,
        AdapterNotActive = 0x02
    };
}

struct VESAVideoParametersTable
{
    uint8_t DisplayedCharacterColumns;
    uint8_t DisplayedRowsMinus1;
    uint8_t CharacterMatrixHeightPoints;
    uint16_t VideoBufferSize;
    VGA::SequencerData_t SequencerRegisters[4];//SR1-SR4
    VGA::MiscellaneousOutput_t MiscOutputRegisterValues;
    VGA::CRTControllerData_t CRTCRegisters[25];//CR0-CR18
    VGA::AttributeControllerData_t AttributeControllerRegs[20];
    VGA::GraphicsControllerData_t GraphicsControllerRegs[9];//GR0-GR8
};

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

typedef uint8_t ColorMode_t;

namespace ColorMode
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
        Unknown1 = 0x02,                // 1 = Always 1
        Unknown2 = 0x04,                 // 2 = unknown
        Unknown3 = 0x30                 // 4 = 800x600 and text modes don't have these two bits set
    };
}

struct VESAVideoModeData
{
    VESAVideoParametersTable* OverrideTable;
    VESAResolutionVariant* VariantData;
    VideoMode_t Mode;
    VESAVideoModeFlags_t Flags;    //bit 0 - Window granularity and size: 0 = 32k, 1 = 64k
    AdvancedFunctionControlLower_t AdvancedFunction;
    ColorMode_t ColorMode;
    VGA::Sequencer::MemoryModeControl_t MemoryModeControl;
};

typedef uint8_t FirmwareFlag_t;
namespace FirmwareFlag
{
    enum
    {
        Unknown0 = 0x01,
        Unknown1 = 0x02,
        Unknown2 = 0x04,
        Unknown3 = 0x08,
        Unknown4 = 0x10,
        Unknown5 = 0x20,
        Unknown6 = 0x40,
        Unknown7 = 0x80
    };
}

class TrioBase : public Hag::Driver
{
public:
    TrioBase(IAllocator& allocator)
        : Hag::Driver(allocator)
    {

    }

    virtual ~TrioBase();

protected:
    VideoMode_t ConvertVesaModeToLegacy(Vesa::VideoMode_t mode) const;
    VideoModeError_t CheckValidVideoMode(VideoMode_t mode) const;
    void SetBDAVideoMode(VideoMode_t mode) const;

    bool FindVideoModeData(VideoMode_t mode, VESAVideoModeData*& videoModeData) const;
    bool GetVideoModeFlags(VideoMode_t mode, VESAVideoModeFlags_t& flags) const;
    void SetupBDAColor(System::BDA::EGAFeatureBitSwitches_t featureBitSwitches) const;


    struct VideoModeTranslation
    {
        Vesa::VideoMode_t VesaMode;
        VideoMode_t LegacyMode;
    };

    static VideoModeTranslation m_VideoModeTranslation[];
    static VESAVideoModeData m_VideoModeData[];
    static FirmwareFlag_t m_FirmwareFlag;
};

template<Register_t CrtControllerIndex>
class Trio : public TrioBase
{
public:
    Trio(IAllocator& allocator)
        : TrioBase(allocator)
    {

    }

    virtual ~Trio()
    {

    }

    VideoModeError_t SetLegacyVideoMode(VideoMode_t mode);
    VideoModeError_t SetVesaVideoMode(Vesa::VideoMode_t mode);

    uint16_t GetDisplayMemoryInKiB();

private:
    Register_t CRTControllerIndex() { return CrtControllerIndex; }
    Register_t CRTControllerData() { return CrtControllerData + 0x01; }
    Register_t FeatureControlW() { return CrtControllerIndex + 0x06; }
    Register_t InputStatus1() { return CrtControllerIndex + 0x06; }
};

template<Register_t CrtControllerIndex>
uint16_t Trio<CrtControllerIndex>::GetDisplayMemoryInKiB()
{
    static uint16_t memorySizeInKB = 0xFFFF;

    //Early out.
    if (memorySizeInKB != 0xFFFF)
        return memorySizeInKB;

    memorySizeInKB = CRTController::Configuration1::GetDisplayMemorySizeInKiB(CRTControllerIndex());

    return memorySizeInKB;
}


template<Register_t CrtControllerIndex>
VideoModeError_t Trio<CrtControllerIndex>::SetLegacyVideoMode(VideoMode_t mode)
{
    CRTController::RegisterLock2::SoftUnlock<CrtControllerIndex> regLock2;
    VGA::VideoMode_t actualMode = mode & VGA::VideoMode_t(~VGA::VideoMode::DontClearDisplay);

    //Check the validity of the video mode.
    VideoModeError ret = CheckValidVideoMode(actualMode)
    if (ret != VideoModeError::Success)
        return ret;

    //Check if the VGA adapter is the active output.
    if ((System::BDA::VideoDisplayDataArea::Get() & System::BDA::VideoDisplayDataArea::VGA) == 0)
        return VideoModeError::AdapterNotActive;

    //Check to see if the active video mode is the same as the one we're trying to set.
    if (System::BDA::DisplayMode::Get() == actualMode)
        return VideoModeError::Success;

    //Prepare the BDA for the new video mode.
    SetBDAVideoMode(actualMode);



    return VideoModeError::Success;
}

template<Register_t CrtControllerIndex>
VideoModeError_t Trio<CrtControllerIndex>::SetVesaVideoMode(Vesa::VideoMode_t mode)
{
    VideoMode_t legacyMode = VideoMode::Invalid;
    if ((mode & Vesa::VideoMode::LegacyMask) == 0)
    {
        legacyMode = VideoMode_t(mode);//Just drop the top byte
    }
    else
    {
        Vesa::VideoMode_t actualMode = mode & Vesa::VideoMode_t(~Vesa::VideoMode::DontClearDisplay);
        legacyMode = ConvertVesaModeToLegacy(actualMode);
    }

    if (legacyMode == VideoMode::Invalid)
        return VideoModeError::UnknownVideoMode;
    
    if ((mode & Vesa::VideoMode::DontClearDisplay) == Vesa::VideoMode::DontClearDisplay)
        legacyMode |= VideoMode::DontClearDisplay;
    
    return SetVideoMode(legacyMode);
}

}}

#pragma pack(pop);