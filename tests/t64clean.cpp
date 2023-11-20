//Copyright 2023-Present riplin

#include <stdio.h>
#include <string.h>

#include <hag/system/bda.h>
#include <hag/system/sysasm.h>

#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/regs.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/crtc/curendsl.h>
#include <hag/drivers/vga/crtc/curstrsl.h>
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/vga/crtc/maxscanl.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/undloc.h>
#include <hag/drivers/vga/crtc/verdisen.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/gfxc/gfxcmode.h>
#include <hag/drivers/vga/gfxc/mmmctrl.h>
#include <hag/drivers/vga/gfxc/rdplnsel.h>
#include <hag/drivers/vga/sqrc/reset.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/memodctl.h>

#include <hag/drivers/s3/advfnctl.h>
#include <hag/drivers/s3/bitplnwm.h>
#include <hag/drivers/s3/curxpos.h>
#include <hag/drivers/s3/curypos.h>
#include <hag/drivers/s3/drawcmd.h>
#include <hag/drivers/s3/fgcolor.h>
#include <hag/drivers/s3/fgmix.h>
#include <hag/drivers/s3/gfxprocs.h>
#include <hag/drivers/s3/majapcnt.h>
#include <hag/drivers/s3/trio.h>
#include <hag/drivers/s3/vidmodes.h>
#include <hag/drivers/s3/wregdata.h>
#include <hag/drivers/s3/crtc/biosflag.h>
#include <hag/drivers/s3/crtc/bkwcomp1.h>
#include <hag/drivers/s3/crtc/bkwcomp3.h>
#include <hag/drivers/s3/crtc/chipidrv.h>
#include <hag/drivers/s3/crtc/conf1.h>
#include <hag/drivers/s3/crtc/devidhi.h>
#include <hag/drivers/s3/crtc/devidlo.h>
#include <hag/drivers/s3/crtc/exbiosf1.h>
#include <hag/drivers/s3/crtc/exbiosf3.h>
#include <hag/drivers/s3/crtc/exbiosf4.h>
#include <hag/drivers/s3/crtc/exhorovf.h>
#include <hag/drivers/s3/crtc/exmemct2.h>
#include <hag/drivers/s3/crtc/exmemct3.h>
#include <hag/drivers/s3/crtc/exmscct2.h>
#include <hag/drivers/s3/crtc/exsysct1.h>
#include <hag/drivers/s3/crtc/exsysct2.h>
#include <hag/drivers/s3/crtc/exsysct3.h>
#include <hag/drivers/s3/crtc/extmode.h>
#include <hag/drivers/s3/crtc/exverovf.h>
#include <hag/drivers/s3/crtc/itlrtst.h>
#include <hag/drivers/s3/crtc/linawctr.h>
#include <hag/drivers/s3/crtc/lnawposh.h>
#include <hag/drivers/s3/crtc/lnawposl.h>
#include <hag/drivers/s3/crtc/memconf.h>
#include <hag/drivers/s3/crtc/misc1.h>
#include <hag/drivers/s3/crtc/modectrl.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/revision.h>
#include <hag/drivers/s3/crtc/stdsfifo.h>
#include <hag/drivers/s3/crtc/sysconf.h>
#include <hag/drivers/s3/sqrc/clksync2.h>
#include <hag/drivers/s3/sqrc/dclkvhi.h>
#include <hag/drivers/s3/sqrc/dclkvlow.h>
#include <hag/drivers/s3/sqrc/extseq9.h>
#include <hag/drivers/s3/sqrc/extseqd.h>
#include <hag/drivers/s3/sqrc/mclkvhi.h>
#include <hag/drivers/s3/sqrc/mclkvlow.h>
#include <hag/drivers/s3/sqrc/rclksync.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>

#if 0
#define LABEL(F, L)         \
L:                          \
printf("%s.%s\n", #F, #L);
#else
#define LABEL(F, L)         \
L:
#endif

#pragma pack(push, 1);

namespace Clean
{

Hag::VGA::Register_t GetCRTControllerIndexRegister()
{
    using namespace Hag::VGA;

    return (MiscellaneousOutput::Read() & 
            MiscellaneousOutput::IOAddressSelect) == 
            MiscellaneousOutput::IOAddressSelect ?
            Register::CRTControllerIndexD :
            Register::CRTControllerIndexB;
}

void ModeSetBDA(Hag::VGA::VideoMode_t& mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::VGA) != VideoDisplayDataArea::VGA) ||
        (DisplayMode::Get() == mode))
        return;

    DetectedHardware_t initialVideoMode = DetectedHardware::Get() & DetectedHardware::InitialVideoModeMask;
    EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

    uint8_t flags = 0;
    if ((mode == VideoMode::T80x25x2M) || (mode == VideoMode::G640x350x2M) ||
        (S3::TrioBase::GetVideoModeFlags(mode, flags) && ((flags & S3::VESAVideoModeFlags::Color) == 0)))
    {
        if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) == 0)
        {
            if ((VideoBaseIOPort::Get() != Register::CRTControllerIndexB) &&
                (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2))
            {
                if (initialVideoMode != DetectedHardware::Monochrome80x25)
                {
                    mode = VideoMode::T40x25x16G;
                }
                else
                {
                    if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25)
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                    }
                    else if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                        VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                        EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                        Hag::System::BDA::EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                        if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
                        {
                            DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                        }
                    }
                    else 
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                        VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                        EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                        EGAFeatureBitSwitches::Get() |= adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2 ?
                                                                  EGAFeatureBitSwitches::CGAMono80x25_2 :
                                                                  EGAFeatureBitSwitches::MDAHiRes80x25_2;
                    }
                }
            }
        }
        else
        {
            if (VideoBaseIOPort::Get() == Register::CRTControllerIndexB)
            {
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
            else if ((adapterType <= EGAFeatureBitSwitches::CGAMono80x25) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2))
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
            else
            {
                if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
                {
                    VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                    VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                    EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                    Hag::System::BDA::EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                    if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
                    {
                        DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                    }
                }
                else if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2)
                {
                    VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
                    EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                    EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | EGAFeatureBitSwitches::MDAHiRes80x25_2;
                    VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

                    if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
                    {
                        DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                        DetectedHardware::Get() |= DetectedHardware::Color80x25;
                    }
                }
            }
        }
    }
    else if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
    {
        if (VideoBaseIOPort::Get() == Register::CRTControllerIndexD)
        {
            DetectedHardware::Get() &= ~DetectedHardware::InitialVideoModeMask;
            DetectedHardware::Get() |= DetectedHardware::Color80x25;
        }
        else if ((adapterType > EGAFeatureBitSwitches::CGAMono80x25) &&
            (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2))
        {
            if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                Hag::System::BDA::EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
                {
                    DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                }
            }
            else if (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
        }
        else
        {
            VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
            EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
            EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | EGAFeatureBitSwitches::MDAHiRes80x25_2;
            VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

            if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
            {
                DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                DetectedHardware::Get() |= DetectedHardware::Color80x25;
            }
        }
    }
    else if (VideoBaseIOPort::Get() != Register::CRTControllerIndexD)
    {
        if (initialVideoMode == DetectedHardware::Monochrome80x25)
        {
            mode = VideoMode::T80x25x2M;
        }
        else if (adapterType > EGAFeatureBitSwitches::CGAMono80x25)
        {
            EGAFeatureBitSwitches_t newAdapterType = 0;
            if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                newAdapterType = EGAFeatureBitSwitches::CGAMono80x25_2 | EGAFeatureBitSwitches::FeatureConnector1;
            }
            else if (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
                newAdapterType = EGAFeatureBitSwitches::CGAMono80x25_2;
            }
            else if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2)
            {
                newAdapterType = EGAFeatureBitSwitches::MDAHiRes80x25_2 | EGAFeatureBitSwitches::FeatureConnector0;
            }

            VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
            EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
            EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | newAdapterType;
            VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

            if ((S3::TrioBase::m_FirmwareFlag & S3::FirmwareFlag::Color) != 0)
            {
                DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                DetectedHardware::Get() |= DetectedHardware::Color80x25;
            }
        }
    }
}

bool VerifyBDAOrDeactivate(Hag::VGA::VideoMode_t& mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    uint8_t flags = 0;
    bool ret = false;
    PointHeightOfCharacterMatrix_t newPointHeight = 0;

    if ((DetectedHardware::Get() & DetectedHardware::InitialVideoModeMask) == DetectedHardware::Monochrome80x25)
    {
        if ((VideoModeOptions::Get() & VideoModeOptions::Monochrome) != 0)
        {
            if ((mode != VideoMode::G640x350x2M) &&
                (mode != VideoMode::T80x25x2M) && 
                (!S3::TrioBase::GetVideoModeFlags(mode, flags) ||
                ((flags & S3::VESAVideoModeFlags::Color) == 0)))
            {                
                mode = VideoMode::T80x25x2M;
                VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
            }
            ret = mode != VideoMode::T40x25x16G;
        }
        else
        {
            VideoModeOptions::Get() |= VideoModeOptions::Inactive;
            newPointHeight = 14;
        }
    }
    else
    {
        if ((Hag::System::BDA::VideoModeOptions::Get() & VideoModeOptions::Monochrome) == 0)
        {
            if ((mode == VideoMode::G640x350x2M) ||
                (mode == VideoMode::T80x25x2M) ||
                (S3::TrioBase::GetVideoModeFlags(mode, flags) &&
                (flags & S3::VESAVideoModeFlags::Color) == 0))
            {
                mode = VideoMode::T40x25x16G;
                VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
            }
            ret = mode != VideoMode::T40x25x16G;
        }
        else
        {
            VideoModeOptions::Get() |= mode == VideoMode::T80x25x16C ?
                                               VideoModeOptions::Inactive | VideoModeOptions::Unknown :
                                               VideoModeOptions::Inactive;
            newPointHeight = 8;
        }
    }

    if (newPointHeight != 0)
    {
        RowsOnScreen::Get() = 24;
        PointHeightOfCharacterMatrix::Get() = newPointHeight;
    }
    return ret;
}

Hag::S3::VideoParameters* GetVideoModeOverrideTable(Hag::VGA::VideoMode_t mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    S3::VideoParameters* overrideTable = NULL;

    if (mode <= VideoMode::MaxValid)
    {
        uint8_t* translationTable = S3::TrioBase::m_VideoModeOverrideTranslationTable1;
        if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0)
        {
            translationTable = S3::TrioBase::m_VideoModeOverrideTranslationTable2;
            EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() &
                                                  EGAFeatureBitSwitches::AdapterTypeMask;

            if ((adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2) &&
                (adapterType != EGAFeatureBitSwitches::MDAColor80x25_2))
            {
                translationTable = S3::TrioBase::m_VideoModeOverrideTranslationTable3;
            }
        }
        overrideTable = &S3::TrioBase::m_LegacyVideoModes[translationTable[mode]];
    }
    else
    {
        S3::VESAVideoModeData* vesaModeData = NULL;
        if (vesaModeData = S3::TrioBase::FindVideoModeData(mode))
            overrideTable = vesaModeData->OverrideTable;
    }
    return overrideTable;
}

Hag::S3::VideoParameters* GetCurrentVideoModeOverrideTable()
{
    return GetVideoModeOverrideTable(Hag::System::BDA::DisplayMode::Get());
}

Hag::S3::VideoParameters* SetTextModeBiosData(uint8_t mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    static uint8_t Mode0_7ControlRegValue[] = { 0x2C, 0x28, 0x2D, 0x29, 0x2A, 0x2E, 0x1E, 0x29 };

    for (int i = 0; i < 8; ++i)
    {
        CursorPositions::Get()[i].Column = 0;
        CursorPositions::Get()[i].Row = 0;
    }

    ActiveDisplayNumber::Get() = 0;
    VideoBufferOffset::Get() = 0;

    if (DisplayMode::Get() <= VideoMode::T80x25x2M)
    {
        CGAColorPaletteMaskSetting::Get() = DisplayMode::Get() == VideoMode::G640x200x2M ?
                                            CGAColorPaletteMaskSetting::Blue |
                                            CGAColorPaletteMaskSetting::Green |
                                            CGAColorPaletteMaskSetting::Red |
                                            CGAColorPaletteMaskSetting::BorderAndBackgroundColorIntensity |
                                            CGAColorPaletteMaskSetting::BackgroundColor |
                                            CGAColorPaletteMaskSetting::ForgroundColorSelect :
                                            CGAColorPaletteMaskSetting::BackgroundColor |
                                            CGAColorPaletteMaskSetting::ForgroundColorSelect;
        CRTModeControlRegValue::Get() = Mode0_7ControlRegValue[DisplayMode::Get()];
    }

    S3::VideoParameters* overrideTable = GetCurrentVideoModeOverrideTable();
    NumberOfScreenColumns::Get() = overrideTable->DisplayedCharacterColumns;
    RowsOnScreen::Get() = overrideTable->DisplayedRowsMinus1;
    PointHeightOfCharacterMatrix::Get() = overrideTable->CharacterMatrixHeightPoints;
    VideoBufferSize::Get() = overrideTable->VideoBufferSize;
    CursorScanLines::Get().End = overrideTable->CRTCRegisters[11];
    CursorScanLines::Get().Start = overrideTable->CRTCRegisters[10];
    return overrideTable;
}

void SaveDynamicParameterData(Hag::S3::VideoParameters* overrideTable)
{
    using namespace Hag::System::BDA;

    uint8_t* savePointer;
    if (!GetVideoParameterBlockElement(1, savePointer, 0x100))
        return;

    memcpy(savePointer, overrideTable->AttributeControllerRegs, 16);
    savePointer += 16;
    *savePointer = overrideTable->AttributeControllerRegs[17];
}

void PrepareAttributeController()
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    InputStatus1::Read(VideoBaseIOPort::Get() + (Register::InputStatus1D - Register::CRTControllerIndexD));
    AttributeControllerIndex::Write(AttributeControllerRegister::Palette0);
}

void ApplyVideoParameters(Hag::S3::VideoParameters* overrideTable)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    PrepareAttributeController();

    SYS_ClearInterrupts();
    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset);
    Sequencer::ClockingMode::Write(Sequencer::ClockingMode::ScreenOff);
    Sequencer::ClockingMode::Write(Sequencer::ClockingMode::ScreenOff | overrideTable->SequencerRegisters[0]);
    MiscellaneousOutput::Write(overrideTable->MiscOutputRegisterValues);
    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset | Sequencer::Reset::SynchronousReset);
    SYS_RestoreInterrupts();

    Sequencer::EnableWritePlane::Write(overrideTable->SequencerRegisters[1]);
    Sequencer::CharacterFontSelect::Write(overrideTable->SequencerRegisters[2]);
    Sequencer::MemoryModeControl::Write(overrideTable->SequencerRegisters[3]);

    Register_t crtRegister = VideoBaseIOPort::Get();

    if ((DisplayMode::Get() < VideoMode::Unknown1) ||
        (DisplayMode::Get() > VideoMode::Reserved2))
    {
        crtRegister = (overrideTable->MiscOutputRegisterValues & MiscellaneousOutput::IOAddressSelect) == 0 ? 
                        Register::CRTControllerIndexB :
                        Register::CRTControllerIndexD;
    }
    VideoBaseIOPort::Get() = crtRegister;
    CRTController::VerticalRetraceEnd::Write(crtRegister, 0);

    for (uint8_t crtIndex = 0; crtIndex < 25; ++crtIndex)
    {
        CRTControllerData::Write(crtRegister, crtIndex, overrideTable->CRTCRegisters[crtIndex]);
    }
    
    //Reset Attribute Controller port 0x3c0 to point to index register
    Register_t inputStatus1 = crtRegister + (Register::InputStatus1D - Register::CRTControllerIndexD);
    InputStatus1::Read(inputStatus1);

    if ((DisplayMode::Get() == VideoMode::Unknown2) ||
        (DisplayMode::Get() != VideoMode::Reserved1))
    {
        InputStatus1::Read(Register::InputStatus1B);
    }

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        for (uint8_t attribIndex = 0; attribIndex < 16; ++attribIndex)
        {
            AttributeControllerData::Write(attribIndex, overrideTable->AttributeControllerRegs[attribIndex]);
        }
    }

    uint8_t attribIndex = 16;
    for (uint8_t attribCount = 0; attribCount < 5; ++attribCount)
    {
        if ((attribIndex != 17) || ((VideoDisplayDataArea::Get() &VideoDisplayDataArea::PaletteLoadingDisabled) == 0))
        {
            AttributeControllerData_t value = attribIndex == 20 ? 0 :overrideTable->AttributeControllerRegs[16 + attribCount];
            AttributeControllerData::Write(attribIndex, value);
        }
        ++attribIndex;
    }

    for (uint8_t gfxIndex = 0; gfxIndex < 9; ++gfxIndex)
    {
        GraphicsControllerData::Write(gfxIndex, overrideTable->GraphicsControllerRegs[gfxIndex]);
    }
}

uint8_t FetchBusSpecificSystemConfig(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag::S3;

    return ((CRTController::Configuration1::Read(crtcPort) & 
            CRTController::Configuration1::SystemBusSelect) == 
            CRTController::Configuration1::PCIBus) ?
            TrioBase::m_PCISystemConfig :
            TrioBase::m_VLBSystemConfig;
}

void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::VGA;

    for (uint8_t crtcCount = 0; crtcCount < CRTCInitData[0]; ++crtcCount)
    {
        CRTControllerData::Write(crtcPort, CRTCInitData[1 + (crtcCount << 1)], CRTCInitData[2 + (crtcCount << 1)]);
    }

    for (uint8_t sequenceCount = 0; sequenceCount < S3::TrioBase::m_SequenceInitData[0]; ++sequenceCount)
    {
        SequencerIndex_t index = S3::TrioBase::m_SequenceInitData[1 + (sequenceCount << 1)];
        SequencerData_t value = S3::TrioBase::m_SequenceInitData[2 + (sequenceCount << 1)];

        //Don't write the hsync / vsync data.
        if (index == S3::SequencerRegister::ExtendedSequencerD)
            value &= ~S3::Sequencer::ExtendedSequencerD::SyncMask;

        SequencerData::Write(index, value);
    }
    S3::CRTController::SystemConfiguration_t sysConf = S3::CRTController::SystemConfiguration::Read(crtcPort);
    sysConf &= S3::CRTController::SystemConfiguration::UpperBitsMask;
    sysConf |= FetchBusSpecificSystemConfig(crtcPort);
    S3::CRTController::SystemConfiguration::Write(crtcPort, sysConf);

    CRTControllerIndex::Write(crtcPort, S3::CRTControllerRegister::ModeControl);
    S3::CRTController::ModeControl_t modeControl = CRTControllerData::Read(crtcPort + 1) & ~S3::CRTController::ModeControl::Interlaced;
    CRTControllerData::Write(crtcPort + 1, modeControl);
}

void WaitGraphicsEngineReady(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::VGA;

    S3::CRTController::SystemConfiguration_t systemConfig = S3::CRTController::SystemConfiguration::Read(crtcPort);
    if (((systemConfig & S3::CRTController::SystemConfiguration::EnableEnhancedRegisterAccess) != 0) &&
        S3::GraphicsProcessorStatus::Read() != S3::GraphicsProcessorStatus::FIFOStatusInvalid)
    {
        S3::GraphicsProcessorStatus_t status = 0;
        do
        {
            status = S3::GraphicsProcessorStatus::Read();
        } while ((status & S3::GraphicsProcessorStatus::GraphicsEngineBusy) != 0);
    }
}

void ClearMemory(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::System::BDA;

    if ((VideoModeOptions::Get() & VideoModeOptions::DontClearDisplay) == 0)
    {
        S3::CRTController::RegisterLock1::Unlock(crtcPort);
        S3::CRTController::RegisterLock2::Unlock(crtcPort);

        S3::CRTController::LinearAddressWindowControl_t linearAddressControl = 
            S3::CRTController::LinearAddressWindowControl::Read(crtcPort);
        S3::CRTController::LinearAddressWindowControl::Write(crtcPort, linearAddressControl &
                                                             ~S3::CRTController::LinearAddressWindowControl::EnableLinearAddressing);

        S3::CRTController::ExtendedSystemControl1_t extendedSystemControl = 
            S3::CRTController::ExtendedSystemControl1::Read(crtcPort) & 
            S3::CRTController::ExtendedSystemControl1::LowerMask;
        S3::CRTController::ExtendedSystemControl1::Write(crtcPort, extendedSystemControl |
                                                         S3::CRTController::ExtendedSystemControl1::Length16Bpp);

        S3::CRTController::ExtendedMode_t extendedMode = S3::CRTController::ExtendedMode::Read(crtcPort);
        S3::CRTController::ExtendedMode::Write(crtcPort, extendedMode &
                                               S3::CRTController::ExtendedMode::UnknownMask);

        S3::CRTController::SystemConfiguration_t systemConfiguration = S3::CRTController::SystemConfiguration::Read(crtcPort);
        S3::CRTController::SystemConfiguration::Unlock(crtcPort);

        S3::AdvancedFunctionControl::WriteLower(S3::AdvancedFunctionControl::EnableEnhancedFunctions |
                                                S3::AdvancedFunctionControl::ReservedAs1 |
                                                S3::AdvancedFunctionControl::EnhancedModePixelLength);

        S3::WriteRegisterData::WriteTopScissors(S3::WriteRegisterData::MinValue);
        S3::WriteRegisterData::WriteLeftScissors(S3::WriteRegisterData::MinValue);
        S3::WriteRegisterData::WriteBottomScissors(S3::WriteRegisterData::MaxValue);
        S3::WriteRegisterData::WriteRightScissors(S3::WriteRegisterData::MaxValue);

        S3::WriteRegisterData::WriteMultifunctionControlMisc(S3::MultifunctionControlMiscellaneous::SelectSlowReadModifyWriteCycle);

        S3::BitplaneWriteMask::Write16x2(S3::BitplaneWriteMask::AllSet);

        S3::ForegroundColor::Write16x2(0);

        S3::ForegroundMix::Write(S3::ForegroundMix::MixNew | S3::ForegroundMix::SelectForegroundColor);

        S3::WriteRegisterData::WritePixelControl(S3::PixelControl::MixForeground);

        S3::CurrentXPosition::Write(0);
        S3::CurrentYPosition::Write(0);

        S3::MajorAxisPixelCount::Write(S3::MajorAxisPixelCount::MaxValue);

        S3::WriteRegisterData::WriteMinorAxisPixelCount(S3::MinorAxisPixelCount::MaxValue);

        S3::DrawingCommand::Write(S3::DrawingCommand::MustBe1 |
                                  S3::DrawingCommand::AcrossThePlanePixelMode |
                                  S3::DrawingCommand::DrawPixel |
                                  S3::DrawingCommand::PosYXmajPosX |
                                  S3::DrawingCommand::CommandRectangleFill);

        WaitGraphicsEngineReady(crtcPort);
    }
}

void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data)
{
    using namespace Hag;
    using namespace Hag::VGA;

    S3::Sequencer::UnlockExtendedSequencer::Unlock();
    S3::Sequencer::ClockSynthControl2_t clockSynthControl2;
    if (idx == 0x22)
    {
        SequencerData::Write(S3::SequencerRegister::Unknown1B, 0x00);
        SequencerData::Write(S3::SequencerRegister::Unknown1A, 0x46);

        S3::Sequencer::MClockValueLow::Write(0x46);
        S3::Sequencer::MClockValueHigh::Write(0x79);
        S3::Sequencer::DClockValueLow::Write(0x49);
        S3::Sequencer::DClockValueHigh::Write(0x55);

        clockSynthControl2 = S3::Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                             S3::Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }
    else
    {
        S3::Sequencer::DClockValueLow::Write(data[1]);
        S3::Sequencer::DClockValueHigh::Write(data[0]);

        clockSynthControl2 = S3::Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }

    S3::Sequencer::RAMDACClockSynthControl::Write(0);

    S3::Sequencer::ClockSynthControl2_t originalClockSynthControl2 = S3::Sequencer::ClockSynthControl2::Read();

    originalClockSynthControl2 &= ~(S3::Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                                    S3::Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad |
                                    S3::Sequencer::ClockSynthControl2::DivideDClockByTwo);
    
    S3::Sequencer::ClockSynthControl2::Write(originalClockSynthControl2);
    S3::Sequencer::ClockSynthControl2::Write(originalClockSynthControl2 | clockSynthControl2);
}

void SetupClocks(Hag::VGA::Register_t crtcPort, uint8_t clockConfig)
{
    using namespace Hag;

    uint8_t* clockDataPtr = S3::CRTController::Revision::Read(crtcPort) != 0x03 ?
                            S3::TrioBase::m_ClockData :
                            S3::TrioBase::m_ClockDataRev3;

    uint32_t offset = (clockConfig & 0x1f) << 1;
    ConfigureDCLKAndMCLK(clockConfig, clockDataPtr + offset);

    S3::CRTController::ModeControl::Write(crtcPort, clockConfig & S3::CRTController::ModeControl::Interlaced);
}

uint8_t GetMemorySizeInMiB(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;

    S3::CRTController::RegisterLock1::Unlock(crtcPort);
    S3::CRTController::RegisterLock2::Unlock(crtcPort);

    uint8_t size;
    switch (S3::CRTController::Configuration1::Read(crtcPort) & S3::CRTController::Configuration1::DisplayMemorySize)
    {
    case S3::CRTController::Configuration1::Size05MiB:
        size = 0;      //0.5MiB == 0 for some reason.
        break;
    case S3::CRTController::Configuration1::Size1MiB:
        size = 1;
        break;
    case S3::CRTController::Configuration1::Size2MiB:
        size = 2;
        break;
    case S3::CRTController::Configuration1::Size4MiB:
        size = 4;
        break;
    default:
        size = 1;
        break;
    }
    return size;
}

void ConfigureExtraVESAModeSettings(Hag::S3::VideoMode_t mode, Hag::VGA::Register_t crtcPort, Hag::S3::VESAVideoModeData* overrideTable, Hag::S3::VESAResolutionVariant* modeData)
{
    using namespace Hag;
    using namespace Hag::VGA;

    uint16_t logicalScreenWidth = overrideTable->ModeInfo->BytesPerScanline;

    if (overrideTable->ModeInfo->MemoryModelType == S3::VESAModeMemoryModel::Text)
        logicalScreenWidth <<= 1;
    
    if (overrideTable->ModeInfo->MemoryModelType == S3::VESAModeMemoryModel::EGA16ColorGraphics)
        logicalScreenWidth <<= 2;

    logicalScreenWidth >>= 3;
    uint8_t logicalScreenWidthHigh = uint8_t(logicalScreenWidth >> 8);
    CRTController::ScreenOffset::Write(crtcPort, uint8_t(logicalScreenWidth));
    S3::CRTController::ExtendedSystemControl2::Write(crtcPort, logicalScreenWidthHigh <<
                                                     S3::CRTController::ExtendedSystemControl2::Shift::LogicalScreenWidthHigh);

    S3::CRTController::Miscellaneous1_t miscellaneous1 = S3::CRTController::Miscellaneous1::RefreshCount1 |
                                                         S3::CRTController::Miscellaneous1::EnableAlternateRefreshCount;

    S3::CRTController::MemoryConfiguration_t memoryConfiguration = S3::CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                                                                   S3::CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth;    

    if (overrideTable->ModeInfo->BitsPerPixel >= 8)
    {
        miscellaneous1 |= S3::CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode;
        memoryConfiguration = S3::CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              S3::CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    S3::CRTController::Miscellaneous1::Write(crtcPort, miscellaneous1);

    if ((mode == S3::VideoMode::P800x600x16C) ||
        (mode == S3::VideoMode::P1024x768x16C) ||
        (mode == S3::VideoMode::P1280x1024x16C))
    {
        memoryConfiguration = S3::CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              S3::CRTController::MemoryConfiguration::EnableTwoPageScreenImage |
                              S3::CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth |
                              S3::CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    S3::CRTController::MemoryConfiguration::Write(crtcPort, memoryConfiguration);

    S3::CRTController::ExtendedSystemControl1::Write(crtcPort, modeData->ExtendedSystemControl1);

    if ((modeData->ExtendedSystemControl1 & S3::CRTController::ExtendedSystemControl1::Unknown) != 0)
    {
        MiscellaneousOutput_t miscOutput = MiscellaneousOutput::Read() & 
                                           ~(MiscellaneousOutput::SelectNegativeHorizontalSyncPulse |
                                             MiscellaneousOutput::SelectNegativeVerticalSyncPulse);
        MiscellaneousOutput::Write(miscOutput);
    }

    S3::CRTController::BackwardCompatibility3::Write(crtcPort, 
                                                     S3::CRTController::BackwardCompatibility3::EnableStartDisplayFIFOFetch);

    if (GetMemorySizeInMiB(crtcPort) == 1)
    {
        S3::CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2_1MiB);
        S3::CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3_1MiB);
    }
    else
    {
        S3::CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2);
        S3::CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3);
    }

    S3::CRTController::ExtendedHorizontalOverflow::Write(crtcPort, modeData->ExtendedHorizontalOverflow);

    uint16_t horizontalTotal = (uint16_t(modeData->ExtendedHorizontalOverflow &
                                  S3::CRTController::ExtendedHorizontalOverflow::HorizontalTotalHigh) << 8) |
                                  CRTController::HorizontalTotal::Read(crtcPort);

    S3::CRTController::InterlaceRetraceStart::Write(crtcPort, S3::CRTController::InterlaceRetraceStart_t(horizontalTotal >> 1));
    S3::CRTController::StartDisplayFIFO::Write(crtcPort, horizontalTotal - 7);

    S3::CRTController::SystemConfiguration::Unlock(crtcPort);

    if ((mode != S3::VideoMode::T132x25x16C) &&
        (mode != S3::VideoMode::T132x43x16C) &&
        ((S3::CRTController::Miscellaneous1::Read(crtcPort) & 
        S3::CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode) == 0))
    {
        CRTController::VerticalRetraceEnd_t verticalRetraceEnd = CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //I somehow get the feeling that these are tweaks that could also have been applied to the data.
        CRTController::StartHorizontalSyncPosition::Write(crtcPort,
                                                          CRTController::StartHorizontalSyncPosition::Read(crtcPort) + 2);

        CRTController::EndHorizontalSyncPosition::Write(crtcPort,
                                                        CRTController::EndHorizontalSyncPosition::Read(crtcPort) + 2);
        if (mode == S3::VideoMode::P1280x1024x16C)
        {
            CRTController::StartHorizontalBlank::Write(crtcPort,
                                                       CRTController::StartHorizontalBlank::Read(crtcPort) - 1);
        }

        CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }

    S3::CRTController::ExtendedVerticalOverflow::Write(crtcPort, modeData->ExtendedVerticalOverflow);

    S3::CRTController::ExtendedMiscellaneousControl2::Write(crtcPort, modeData->ExtendedMiscellaneousControl2);

    if (modeData->ExtendedMiscellaneousControl2 == S3::CRTController::ExtendedMiscellaneousControl2::ColorMode13)
    {
        CRTController::VerticalRetraceEnd_t verticalRetraceEnd = CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //Another tweak.
        CRTController::StartHorizontalBlank::Write(crtcPort,
                                                   CRTController::StartHorizontalBlank::Read(crtcPort) + 1);

        CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }
}

void Configure256KAddressingAndAddressWindow(Hag::S3::VideoMode_t mode, Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::VGA;

    if (mode > VideoMode::MaxValid)
        S3::CRTController::BackwardCompatibility1::Write(crtcPort,
                                                         S3::CRTController::BackwardCompatibility1::Read(crtcPort) &
                                                         ~S3::CRTController::BackwardCompatibility1::StandardVGAMemoryMapping);

    //TODO: This information should come from the PCI configuration space.
    //Depending on the BIOS flags puts a dependency on the BIOS internal implementation.
    S3::CRTController::ExtendedBIOSFlag3_t biosFlags3 = S3::CRTController::ExtendedBIOSFlag3::Read(crtcPort);
    S3::CRTController::LinearAddressWindowPositionH::Write(crtcPort, biosFlags3);
    S3::CRTController::ExtendedBIOSFlag4_t biosFlags4 = S3::CRTController::ExtendedBIOSFlag4::Read(crtcPort);
    S3::CRTController::LinearAddressWindowPositionL::Write(crtcPort, biosFlags4 & 0x80);
}

void SetColorMode(Hag::S3::VideoMode_t mode, Hag::S3::ColorMode_t colorMode, Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::VGA;

    S3::CRTController::ExtendedMiscellaneousControl2_t cMode = 
        S3::CRTController::ExtendedMiscellaneousControl2::ColorMode0;

    if (mode > VideoMode::MaxValid)
    {
        switch (colorMode)
        {
        case S3::ColorMode::C8bpp2px:
            cMode = S3::CRTController::ExtendedMiscellaneousControl2::ColorMode8;
            break;
        case S3::ColorMode::C24bpp1px:
            cMode = S3::CRTController::ExtendedMiscellaneousControl2::ColorMode13;
            break;
        case S3::ColorMode::C24bppPacked:
            cMode = S3::CRTController::ExtendedMiscellaneousControl2::ColorMode12;
            break;
        case S3::ColorMode::C16bpp1px:
            cMode = S3::CRTController::ExtendedMiscellaneousControl2::ColorMode10;
            break;
        case S3::ColorMode::C15bpp1px:
            cMode = S3::CRTController::ExtendedMiscellaneousControl2::ColorMode9;
            break;
        }
    }

    S3::Sequencer::UnlockExtendedSequencer::Unlock();

    if (cMode == S3::CRTController::ExtendedMiscellaneousControl2::ColorMode8)
    {
        S3::Sequencer::RAMDACClockSynthControl::Write(S3::Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl |
                                                      S3::Sequencer::RAMDACClockSynthControl::EnableClockDoubleMode);

        S3::Sequencer::ClockSynthControl2::Write(S3::Sequencer::ClockSynthControl2::Read() |
                                                 S3::Sequencer::ClockSynthControl2::DivideDClockByTwo);
    }
    else
    {
        S3::Sequencer::RAMDACClockSynthControl::Write(S3::Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl);
        
        S3::Sequencer::ClockSynthControl2::Write(S3::Sequencer::ClockSynthControl2::Read() & 
                                                 ~(S3::Sequencer::ClockSynthControl2::DivideDClockByTwo |
                                                   S3::Sequencer::ClockSynthControl2::InvertDClock));
    }

    S3::CRTController::ExtendedMiscellaneousControl2::Write(crtcPort,
                                                            (S3::CRTController::ExtendedMiscellaneousControl2::Read(crtcPort) &
                                                             ~S3::CRTController::ExtendedMiscellaneousControl2::ColorMode) |
                                                             cMode);
}

void ApplyVESAOverrideData(uint8_t mode, Hag::VGA::Register_t crtcPort, Hag::S3::VESAVideoModeData* overrideTable)
{
    using namespace Hag;
    using namespace Hag::VGA;

    S3::Sequencer::UnlockExtendedSequencer::Unlock();

    S3::Sequencer::ExtendedSequencer9::Write(S3::Sequencer::ExtendedSequencer9::Read() &
                                             ~S3::Sequencer::ExtendedSequencer9::MemoryMappedIOOnly);

    //This is a read-only register according to the spec...
    S3::CRTController::ChipIDRevision::Write(crtcPort, 0x55);
    if (S3::CRTController::ChipIDRevision::Read(crtcPort) != 0x55)
    {
        S3::CRTController::SystemConfiguration::Unlock(crtcPort);

        S3::AdvancedFunctionControl::WriteLower(S3::AdvancedFunctionControl::ReservedAs1);

        InitializeCRTControllerAndSequencer(Hag::S3::TrioBase::m_CRTControllerInitData, crtcPort);
    }

    if (overrideTable != NULL)
    {
        Hag::S3::VESAResolutionVariant* modeData = overrideTable->VariantData;

        ClearMemory(crtcPort);

        //TODO: refresh rate limit should be passed in.
        uint16_t refreshRateCap = (uint16_t(S3::CRTController::BIOSFlag::Read(crtcPort)) << 4) |
                                  (S3::CRTController::ExtendedBIOSFlag1::Read(crtcPort) >> 4);

        for (int i = 0; i < 3; ++i)
        {
            if (overrideTable->OverrideTable == Hag::S3::TrioBase::m_VesaResolutions[i + 3])
                break;

            refreshRateCap >>= 3;
        }
        refreshRateCap &= 7;

        Sequencer::MemoryModeControl::Write(overrideTable->MemoryModeControl);

        S3::CRTController::SystemConfiguration::Unlock(crtcPort);
        CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        S3::AdvancedFunctionControl::WriteLower(overrideTable->AdvancedFunction);

        while (((modeData->FlagsAndFilter & S3::VESAFlagsAndFilter::Terminate) == 0) && 
               ((modeData->FlagsAndFilter & ~S3::VESAFlagsAndFilter::Terminate) > refreshRateCap))
        {
            ++modeData;
        }

        SetupClocks(crtcPort, modeData->ClockConfigIndex);

        for (CRTControllerIndex_t i = 0; i < 25; ++i)
        {
            CRTControllerData::Write(crtcPort, i, modeData->CRTCRegisters[i]);
        }

        ConfigureExtraVESAModeSettings(mode, crtcPort, overrideTable, modeData);
    }

    Configure256KAddressingAndAddressWindow(mode, crtcPort);

    Hag::S3::ColorMode_t colorMode = 0;
    if (overrideTable != NULL)
        colorMode = overrideTable->ColorMode;

    SetColorMode(mode, colorMode, crtcPort);

    Hag::S3::Sequencer::UnlockExtendedSequencer::Lock();
}

uint8_t* DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue)
{
    uint8_t redSettings = *paletteData & 0xC0;//Top 2 bits hold compression information
    red = *paletteData & 0x3F;//6 bit color data
    ++paletteData;

    if (redSettings == 0)
    {
        uint8_t greenSettings = *paletteData & 0x40;
        green = *paletteData & 0x3F;//6 bit color data
        ++paletteData;
        if (greenSettings == 0)
        {
            blue = *paletteData;
            ++paletteData;
        }
        else
        {
            blue = green;
        }
    }
    else if (redSettings == 0x40)
    {
        green = red;
        blue = *paletteData;
        ++paletteData;
    }
    else if (redSettings == 0x80)
    {
        green = blue = red;
    }
    else
    {
        blue = red;
        green = *paletteData;
        ++paletteData;
    }
    return paletteData;
}

uint16_t SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue)
{
    using namespace Hag::VGA;

    SYS_ClearInterrupts();
    DACWriteIndex::Write(colorIndex);
    RAMDACData::Write(red);
    RAMDACData::Write(green);
    RAMDACData::Write(blue);
    SYS_RestoreInterrupts();

    return colorIndex + 1;
}

uint16_t ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint16_t i = 0; i < count; ++i)
    {
        paletteData = DecompressPaletteColor(paletteData, red, green, blue);
        colorIndex = SetPaletteColor(colorIndex, red, green, blue);
    }
    return colorIndex;
}

void Set248ColorPalette()
{
    using namespace Hag;
    using namespace Hag::System::BDA;

    uint16_t colorIndex = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
 
    colorIndex = ApplyPalette(colorIndex, 16, ((VideoDisplayDataArea::Get() &
                                               (VideoDisplayDataArea::GrayScale |
                                                VideoDisplayDataArea::MonochromeMonitor)) == 0) ?
                                                S3::TrioBase::m_ColorPalette : S3::TrioBase::m_MonochromePalette);

    colorIndex = ApplyPalette(colorIndex, 16, S3::TrioBase::m_SecondPalette);

    if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
    {
        ApplyPalette(colorIndex, 216, S3::TrioBase::m_Color216Palette);
    }
    else
    {
        uint8_t* palettePtr = S3::TrioBase::m_Color216Palette;
        for (int i = 0; i < 216; ++i)
        {
            palettePtr = DecompressPaletteColor(palettePtr, red, green, blue);
            red = green = blue = ((blue * 36043 + green * 19333 + red * 9830 + 16384) >> 15) & 0xFF;
            colorIndex = SetPaletteColor(colorIndex, red, green, blue);
        }
    }
}

void SetPalette(Hag::S3::VideoMode_t mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        if (DACMask::Read() != 0xFF)
            DACMask::Write(0xFF);

        bool paletteNotSet = false;
        if ((mode == VideoMode::T80x25x2M) ||
            (mode == VideoMode::G640x350x2M))
        {
            ApplyPalette(0, 64, S3::TrioBase::m_Monochrome64Palette);
        }
        else if (mode == VideoMode::G320x200x256C)
        {
            Set248ColorPalette();
        }
        else if (mode > VideoMode::MaxValid)
        {
            S3::VESAVideoModeFlags_t flags = 0;
            S3::TrioBase::GetVideoModeFlags(mode, flags);

            if ((flags & S3::VESAVideoModeFlags::Color) == 0x00)
            {
                ApplyPalette(0, 64, S3::TrioBase::m_Monochrome64Palette);
            }
            else if ((flags & S3::VESAVideoModeFlags::Palette256) != 0x00)
            {
                Set248ColorPalette();
            }
            else paletteNotSet = true;
        }
        else paletteNotSet = true;

        if (paletteNotSet)
        {
            bool paletteSet3 = false;
            if (mode < VideoMode::G320x200x4C)
            {
                EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

                paletteSet3 = (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2));
            }
            else
            {
                paletteSet3 = ((mode != VideoMode::Unknown1) && (mode <= VideoMode::G640x200x16C));
            }

            if (paletteSet3)
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, S3::TrioBase::m_ColorPalette3);
                }
                else
                {
                    ApplyPalette(0, 64, S3::TrioBase::m_MonochromePalette3);
                }
            }
            else
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, S3::TrioBase::m_ColorPalette2);
                }
                else
                {
                    ApplyPalette(0, 64, S3::TrioBase::m_MonochromePalette2);
                }
            }
        }
    }
}

void UploadFont(uint8_t* src, uint8_t* dst, uint16_t characters, uint8_t bytesPerCharacter, uint16_t destWidth)
{
    for (uint16_t i = 0; i < characters; ++i)
    {
        memcpy(dst, src, bytesPerCharacter);
        dst += destWidth;
        src += bytesPerCharacter;
    }
}

void PatchFont(uint8_t flags)
{
    using namespace Hag;

    //0x40 = 8x14, 0x80 = 8x16, 0x00 = no patch.
    if ((flags & 0xC0) != 0x00)
    {
        bool is8x16 = (flags & 0x80) == 0x00;
        uint16_t count = is8x16 ? 16 : 14;
        uint8_t* src = is8x16 ? S3::TrioBase::m_CharacterPatch8x16 : S3::TrioBase::m_CharacterPatch8x14;

        FARPointer dst(0xA000, 0x0000);

        while (true)
        {
            dst.Offset = uint16_t(*src) << 5;
            ++src;

            if (dst.Offset == 0)
                break;

            uint8_t* dstPtr = dst.ToPointer<uint8_t>(count);

            SYS_ClearInterrupts();
            memcpy(dstPtr, src, count);
            SYS_RestoreInterrupts();
            src += count;
        }
    }
}

void EnablePaletteBasedVideo(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;

    Register_t inputStatus = crtcPort + (Register::InputStatus1D - Register::CRTControllerIndexD);
    AttributeControllerIndex::ResetIndex(inputStatus);
    AttributeControllerIndex::Write(AttributeControllerIndex::EnableVideoDisplay);
}

void SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    static uint16_t RamBankOffset[] =
    {
        0x0000, //(0)
        0x4000, //(1)
        0x8000, //(2)
        0xC000, //(3)
        0x2000, //(4)
        0x6000, //(5)
        0xA000, //(6)
        0xE000  //(7)
    };

    FARPointer dest(0xA000, 0x0000);

    PrepareAttributeController();
    
    Sequencer::EnableWritePlane::Write(Sequencer::EnableWritePlane::Plane2);
    Sequencer::MemoryModeControl::Write(Sequencer::MemoryModeControl::Unknown |
                                        Sequencer::MemoryModeControl::ExtendedMemoryAddress |
                                        Sequencer::MemoryModeControl::SequentialAddressingMode);

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane2);
    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::Mode0);
    GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);

    dest.Offset = RamBankOffset[ramBank & 0x07] + (startCharacter << 5);

    UploadFont(font, dest.ToPointer<uint8_t>(0x2000), numCharacters, charHeight, 32);
    PatchFont(ramBank);

    Sequencer::EnableWritePlane::Write(Sequencer::EnableWritePlane::Plane1 |
                                       Sequencer::EnableWritePlane::Plane2);
    Sequencer::MemoryModeControl::Write(Sequencer::MemoryModeControl::Unknown |
                                        Sequencer::MemoryModeControl::ExtendedMemoryAddress);

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::None);

    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::OddEvenAddressing);

    if (VideoBaseIOPort::Get() != Register::CRTControllerIndexB)
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    }
    else
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
    }
    EnablePaletteBasedVideo(VideoBaseIOPort::Get());
}

void ConfigureCursorPropertiesAndVerticalDisplayEnd(Hag::S3::VideoMode_t mode, uint8_t characterPointHeight)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    Register_t crtcPort = VideoBaseIOPort::Get();

    PointHeightOfCharacterMatrix::Get() = characterPointHeight;

    uint16_t screenHeight = 0;

    if ((mode == VideoMode::G640x480x2M) ||
        (mode == VideoMode::G640x480x16C))
    {
        screenHeight = 400;
    }
    else if ((mode == VideoMode::G320x200x256C) ||
        (mode == VideoMode::G640x200x2M) ||
        (mode == VideoMode::G320x200x4G) ||
        ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x200x16C)))
    {
        screenHeight = 200;
    }
    else if ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x350x4C))
    {
        screenHeight = 350;
    }
    else
    {
        EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

        if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) != 0)
        {
            screenHeight = 400;
        }
        else if (((VideoModeOptions::Get() & 0x02) != 0) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2) ||
                (mode == VideoMode::T80x25x2M))
        {
            screenHeight = 350;
        }
        else
        {
            screenHeight = 200;
        }
    }

    uint8_t rowsOnScreen = uint8_t(screenHeight / characterPointHeight);
    RowsOnScreen::Get() = rowsOnScreen - 1;

    NumberOfScreenColumns_t bytesPerLine = Hag::System::BDA::NumberOfScreenColumns::Get() << 1;

    VideoBufferSize::Get() = bytesPerLine * rowsOnScreen + 256;

    uint8_t charPointHeightMinusOne = characterPointHeight - 1;

    if (mode == VideoMode::T80x25x2M)
        CRTController::UnderlineLocation::Write(crtcPort, charPointHeightMinusOne);

    CRTController::MaximumScanLine::Write(crtcPort, (CRTController::MaximumScanLine::Read(crtcPort) &
                                                    ~CRTController::MaximumScanLine::MaximumScanLineCount) |
                                                    charPointHeightMinusOne);

    uint8_t cursorBottom = charPointHeightMinusOne;
    uint8_t cursorTop = charPointHeightMinusOne - 1;

    if (charPointHeightMinusOne > 12)
    {
        --cursorBottom;
        --cursorTop;
    }

    Hag::System::BDA::CursorScanLines::Get().End = cursorBottom;
    Hag::System::BDA::CursorScanLines::Get().Start = cursorTop;

    CRTController::CursorStartScanLine::Write(crtcPort, cursorTop);
    CRTController::CursorEndScanLine::Write(crtcPort, cursorBottom);

    uint16_t vertDisplayEnd = (rowsOnScreen - 1) * characterPointHeight;

    if (screenHeight == 200)
        vertDisplayEnd <<= 1;

    --vertDisplayEnd;
    
    CRTController::VerticalDisplayEnd::Write(crtcPort, uint8_t(vertDisplayEnd));
}

void SetFont()
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (PointHeightOfCharacterMatrix::Get() == 14)
    {
        SetTextFontAndAddressing(S3::TrioBase::m_Characters8x14, 0, 256, 14, DisplayMode::Get() == VideoMode::T80x25x2M ? 0x80 : 0);
    }
    else if (PointHeightOfCharacterMatrix::Get() == 8)
    {
        SetTextFontAndAddressing(S3::TrioBase::m_Characters8x8, 0, 256, 8, 0);
    }
    else
    {
        SetTextFontAndAddressing(S3::TrioBase::m_Characters8x16, 0, 256, 16, 0x40);
    }
}

void ConfigureFontAndCursor(Hag::S3::VideoMode_t mode, Hag::S3::AlphanumericCharSet* fontDefinition)
{
    using namespace Hag::System::BDA;
    
    uint8_t* font = fontDefinition->FontData.ToPointer<uint8_t>(fontDefinition->CharacterHeight * fontDefinition->NumCharacters);

    SetTextFontAndAddressing(font,
                             fontDefinition->FirstCharacter,
                             fontDefinition->NumCharacters,
                             fontDefinition->CharacterHeight,
                             fontDefinition->RamBank & 0x3F);
    ConfigureCursorPropertiesAndVerticalDisplayEnd(mode, fontDefinition->CharacterHeight);

    if (fontDefinition->Rows != 0xFF)
    {
        RowsOnScreen::Get() = fontDefinition->Rows - 1;
    }
}

void ConfigureFontRamBank(Hag::S3::GraphicsCharSet* fontDefinition)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (fontDefinition->CharacterHeight != PointHeightOfCharacterMatrix::Get())
    {
        uint8_t* font = fontDefinition->FontData.ToPointer<uint8_t>(fontDefinition->CharacterHeight * 256);
        SetTextFontAndAddressing(font, 0, 256, fontDefinition->CharacterHeight, fontDefinition->RamBank & 0x3f);

        //This is the weirdest piece of code so far.
        Sequencer::CharacterFontSelect::Write((Sequencer::MemoryModeControl::Read() &
                                              (Sequencer::MemoryModeControl::Unknown |
                                               Sequencer::MemoryModeControl::ExtendedMemoryAddress |
                                               Sequencer::MemoryModeControl::Unknown2)) |
                                              ((fontDefinition->RamBank & 0x03) << 2) |
                                              ((fontDefinition->RamBank & 0x04) << 3));
    }
}

void ClearScreen(Hag::S3::VideoMode_t mode)
{
    using namespace Hag::VGA;

    uint32_t size =  0x2000;
    uint16_t segment = 0;
    uint32_t value = 0;

    uint8_t flags = 0;
    if ((mode > VideoMode::MaxValid) &&
        Hag::S3::TrioBase::GetVideoModeFlags(mode, flags))
    {
        segment = 0xB800;
        value = 0x07200720;

        if ((flags & Hag::S3::VESAVideoModeFlags::Color) == 0x00)
        {
            segment = 0xB000;
        }
        if ((flags & Hag::S3::VESAVideoModeFlags::WindowGranularity64KiB) == 0x00)
            return;
    }
    else
    {
        if (mode == VideoMode::T80x25x2M)
        {
            segment = 0xB000;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::T80x25x16C)
        {
            segment = 0xB800;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::G640x200x2M)
        {
            segment = 0xB800;
        }
        else
        {
            segment = 0xA000;
            size = 0x4000;
        }
    }

    uint32_t* ptr = FARPointer(segment, 0x0000).ToPointer<uint32_t>(size << 2);
    
    do
    {
        *ptr = value;
        ++ptr;
        --size;
    } while (size != 0);
}

void SetPaletteProfile(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    SecondarySavePointerTable* secondarySavePointerTable = NULL;
    if (GetVideoParameterBlockElementAs<SecondarySavePointerTable>(4, secondarySavePointerTable, 32) &&
        !secondarySavePointerTable->UserPaletteProfileTable.IsNull())
    {
        PaletteProfile* paletteProfile = secondarySavePointerTable->UserPaletteProfileTable.ToPointer<PaletteProfile>(0x14 + 0x14);
        if (CheckValidInCurrentMode(paletteProfile->ApplicableModes))
        {
            if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) != 0)
            {
                InputStatus1::Read(crtcPort + (Register::InputStatus1D - Register::CRTControllerIndexD));

                uint16_t colorCount = (paletteProfile->DACRegisterCount << 1) +
                                       paletteProfile->DACRegisterCount;

                uint8_t* paletteColors = paletteProfile->DACRegisterTable.ToPointer<uint8_t>(colorCount);
                DACWriteIndex::Write(uint8_t(paletteProfile->DACRegisterStartIndex));
                while (colorCount != 0)
                {
                    RAMDACData::Write(*paletteColors);
                    ++paletteColors;
                    --colorCount;
                }

                if (paletteProfile->AttributeRegisterCount != 0)
                {
                    uint8_t* attributeRegisters = paletteProfile->AttributeRegisterTable.ToPointer<uint8_t>(paletteProfile->AttributeRegisterCount);
                    
                    for (uint16_t attrIdx = 0; attrIdx < paletteProfile->AttributeRegisterCount; ++attrIdx)
                    {
                        AttributeControllerData::Write(paletteProfile->AttributeRegisterStartIndex + attrIdx,
                                                    *attributeRegisters);
                        ++attributeRegisters;
                    }
                    AttributeControllerData::Write(paletteProfile->AttributeRegisterCount + 1, *attributeRegisters);
                }
            }

            if (paletteProfile->Underlining != 0)
            {
                CRTController::StartHorizontalSyncPosition_t horizontalSyncPos = 31;
                if (paletteProfile->Underlining > 0)
                    horizontalSyncPos = CRTController::StartHorizontalSyncPosition_t(PointHeightOfCharacterMatrix::Get() - 1);

                CRTController::StartHorizontalSyncPosition::Write(crtcPort, horizontalSyncPos);
            }
        }
    }
}

bool SetVideoMode(Hag::S3::VideoMode_t mode)
{
    using namespace Hag;
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    Hag::VGA::Register_t crtcPort = GetCRTControllerIndexRegister();
    
    S3::CRTController::RegisterLock1::Unlock(crtcPort);
    S3::CRTController::RegisterLock2::Unlock(crtcPort);

    S3::VideoMode_t dontClearDisplay = mode & VideoMode::DontClearDisplay;

    mode &= ~VideoMode::DontClearDisplay;

    S3::VESAVideoModeData* vesaData = NULL;

    if ((mode <= VideoMode::MaxValid) &&
        (vesaData = S3::TrioBase::FindVideoModeData(mode)))
    {
        VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
        VideoModeOptions::Get() |= dontClearDisplay;

        ModeSetBDA(mode);

        if ((mode <= VideoMode::MaxValid) &&
            !VerifyBDAOrDeactivate(mode))
        {
            //Video card deactivated.
            return false;
        }

        DisplayMode::Get() = mode;

        S3::VideoParameters* overrideTable = SetTextModeBiosData(mode);

        VideoModeOptions::Get() &= ~(VideoModeOptions::Unknown |
                                     VideoModeOptions::Inactive);

        SaveDynamicParameterData(overrideTable);
        ApplyVideoParameters(overrideTable);
        ApplyVESAOverrideData(mode, crtcPort, vesaData);
        SetPalette(mode);

        S3::VESAVideoModeFlags_t flags = 0;

        if ((mode < VideoMode::G320x200x4C) ||
            (mode == VideoMode::T80x25x2M) ||
            (S3::TrioBase::GetVideoModeFlags(mode, flags) &&
            ((flags & S3::VESAVideoModeFlags::WindowGranularity64KiB) != 0)))
        {
            SetFont();

            S3::AlphanumericCharSet* fontDefinition = NULL;
            if (GetVideoParameterBlockElementAs<S3::AlphanumericCharSet>(2, fontDefinition, 0x0B + 0x14) &&
                CheckValidInCurrentMode(fontDefinition->ApplicableModes))
                ConfigureFontAndCursor(mode, fontDefinition);

            SecondarySavePointerTable* paramBlock = NULL;
            if (GetVideoParameterBlockElementAs<SecondarySavePointerTable>(4, paramBlock, 0x20) &&
                !paramBlock->SecondaryAlphanumericCharacterSetOverride.IsNull())
            {
                S3::GraphicsCharSet* graphicsFont = paramBlock->SecondaryAlphanumericCharacterSetOverride.
                    ToPointer<S3::GraphicsCharSet>(0x0B + 0x14);

                if (CheckValidInCurrentMode(graphicsFont->ApplicableModes))
                    ConfigureFontRamBank(graphicsFont);
            }
        }
        else
        {
            CursorScanLines::Get().End = 0;
            CursorScanLines::Get().Start = 0;

            GraphicsCharacterSetOverride* graphicsCharacterFontDefinition = NULL;
            if (GetVideoParameterBlockElementAs<GraphicsCharacterSetOverride>(3, graphicsCharacterFontDefinition, 0x07 + 0x14) &&
                CheckValidInCurrentMode(graphicsCharacterFontDefinition->ApplicableVideoModes))
                SetGraphicsCharacterFont(graphicsCharacterFontDefinition);
        }

        if (((VideoModeOptions::Get() & VideoModeOptions::DontClearDisplay) == 0x00) &&
            (VideoBufferSize::Get() != 0))
            ClearScreen(mode);

        SetPaletteProfile(crtcPort);
        EnablePaletteBasedVideo(crtcPort);
        Sequencer::ClockingMode::TurnScreenOn();

        return true;
    }
    return false;//invalid video mode.
}

}

#pragma pack(pop);
