//Copyright 2023-Present riplin

#include <i86.h>
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
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
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

void LockExtendedCRTRegisters()
{
    using namespace Hag;
    using namespace Hag::VGA;

    Register_t crtc = GetCRTControllerIndexRegister();
    S3::CRTController::RegisterLock1::Lock(crtc);
    S3::CRTController::RegisterLock2::Lock(crtc);
}

void UnlockExtendedCRTRegisters()
{
    using namespace Hag;
    using namespace Hag::VGA;

    Register_t crtc = GetCRTControllerIndexRegister();
    S3::CRTController::RegisterLock1::Unlock(crtc);
    S3::CRTController::RegisterLock2::Unlock(crtc);
}

bool UnlockExtendedCRTRegistersSafe()
{
    UnlockExtendedCRTRegisters();
    return true;
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
                (Hag::S3::TrioBase::GetVideoModeFlags(mode, flags) &&
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

bool GetVideoParameterBlockElement(uint16_t index, uint8_t*& returnPointer, uint16_t size = sizeof(FARPointer))
{
    using namespace Hag::System::BDA;

    returnPointer = NULL;

    FARPointer* realControlBlockPointer = VideoParameterControlBlockPointer::Get().ToPointer<FARPointer>(0x1D);
    bool ret = !realControlBlockPointer->IsNull();

    if (!ret)
        returnPointer = realControlBlockPointer[index].ToPointer<uint8_t>(size);

    ret = !realControlBlockPointer[index].IsNull();

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
        uint8_t* translationTable = Hag::S3::TrioBase::m_VideoModeOverrideTranslationTable1;
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

uint8_t ReadDataWithIndexRegister(uint16_t port, uint8_t index)
{
    SYS_WritePortByte(port, index);
    return SYS_ReadPortByte(port + 1);
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
        UnlockExtendedCRTRegisters();

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

    UnlockExtendedCRTRegisters();

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

    if (Hag::System::BDA::VideoBaseIOPort::Get() != 0x03b4)
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    }
    else
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
    }

    //TODO: Remove this BDA reference.
    EnablePaletteBasedVideo(Hag::System::BDA::VideoBaseIOPort::Get());
}

void ConfigureCursorPropertiesAndVerticalDisplayEnd(uint8_t characterPointHeight)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
//     mov  byte ptr ds:[BDA_PointHeightOfCharacterMatrix], bh;Offset 0x485
    Hag::System::BDA::PointHeightOfCharacterMatrix::Get() = characterPointHeight;

//     mov  bx, 0190h                      ;400
    r.w.bx = 0x0190;

//     mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    r.h.al = Hag::System::BDA::DisplayMode::Get();

//     cmp  al, BDA_DM_640x480_BW_Graphics ;11h
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.al == 0x11)
        goto HeightSet;

//     cmp  al, BDA_DM_640x480_16_Color_Graphics;12h
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.al == 0x12)
        goto HeightSet;

//     mov  bx, 0c8h                       ;200
    r.w.bx = 0xC8;

//     cmp  al, BDA_DM_320x200_256_Color_Graphics;13h
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.al == 0x13)
        goto HeightSet;

//     cmp  al, BDA_DM_320x200_4_Color_Graphics1;04h
//     jb   Label0x3040                    ;Offset 0x3040
    if (r.h.al < 0x04)
        goto Label0x3040;

//     cmp  al, BDA_DM_640x200_BW_Graphics ;06h
//     jbe  HeightSet                      ;Offset 0x306c
    if (r.h.al <= 0x06)
        goto HeightSet;

//     cmp  al, BDA_DM_Unknown2            ;09h
//     jb   Label0x3040                    ;Offset 0x3040
    if (r.h.al < 0x09)
        goto Label0x3040;

//     cmp  al, BDA_DM_640x200_16_Color_Graphics;0eh
//     jbe  HeightSet                      ;Offset 0x306c
    if (r.h.al <= 0x0e)
        goto HeightSet;

//     mov  bx, 015eh                      ;350
    r.w.bx = 0x015e;

//     cmp  al, BDA_DM_640x350_16_Color_Graphics;10h
//     jbe  HeightSet                      ;Offset 0x306c
    if (r.h.al <= 0x10)
        goto HeightSet;

// Label0x3040:                            ;Offset 0x3040
LABEL(ConfigureCursorPropertiesAndVerticalDisplayEnd, Label0x3040);

//     mov  bx, 190h                       ;400
    r.w.bx = 0x0190;

//     test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0x489, 0x10
//     jne  HeightSet                      ;Offset 0x306c
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x10) != 0x00)
        goto HeightSet;

//     mov  bx, 015eh                      ;350
    r.w.bx = 0x015e;

//     test byte ptr ds:[BDA_VideoModeOptions], BDA_VDDA_GrayScale;Offset 0x487, 0x2
//     jne  HeightSet                      ;Offset 0x306c
    if ((Hag::System::BDA::VideoModeOptions::Get() & 0x02) != 0x00)
        goto HeightSet;

//     mov  ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0x488
    r.h.ah = Hag::System::BDA::EGAFeatureBitSwitches::Get();

//     and  ah, BDA_EFBS_AdapterTypeMask   ;0x0f
    r.h.ah &= 0x0F;

//     cmp  ah, BDA_EFBS_MDAHiResEnhanced  ;0x03
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.ah == 0x03)
        goto HeightSet;

//     cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x09
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.ah == 0x09)
        goto HeightSet;

//     cmp  al, BDA_DM_80x25_Monochrome_Text;07h
//     je   HeightSet                      ;Offset 0x306c
    if (r.h.al == 0x07)
        goto HeightSet;

//     mov  bx, 0c8h                       ;200
    r.w.bx = 0x00C8;

// HeightSet:                              ;Offset 0x306c
LABEL(ConfigureCursorPropertiesAndVerticalDisplayEnd, HeightSet);

//     mov  ax, bx                         ;ax = screen height
    r.w.ax = r.w.bx;

//     xor  dx, dx                         ;dx:ax = screen height
//     div  word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485 - divide by height of a character
    r.w.ax /= Hag::System::BDA::PointHeightOfCharacterMatrix::Get();

//     dec  al                             ;decrease by 1
    --r.h.al;

//     mov  byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484 - set number of rows on screen minus one.
    Hag::System::BDA::RowsOnScreen::Get() = r.h.al;

//     inc  al                             ;restore to number of rows on screen.
    ++r.h.al;

//     mov  cx, word ptr ds:[BDA_NumberOfScreenColumns];Offset 0x44a - fetch number of screen columns
    r.w.cx = Hag::System::BDA::NumberOfScreenColumns::Get();

//     shl  cx, 01h                        ;two bytes per character (char + attribute)
    r.w.cx <<= 1;

//     xor  ah, ah                         ;ax = number of rows on screen
    r.h.ah = 0x00;

//     mul  cx                             ;dx:ax = screen size in bytes
    r.w.ax *= r.w.cx;

//     add  ax, 100h                       ;screen size + 256
    r.w.ax += 0x0100;

//     mov  word ptr ds:[BDA_VideoBufferSize], ax;Offset 0x44c
    Hag::System::BDA::VideoBufferSize::Get() = r.w.ax;

//     mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463 CRTC register
    r.w.dx = Hag::System::BDA::VideoBaseIOPort::Get();

//     mov  ah, byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    r.h.ah = Hag::System::BDA::PointHeightOfCharacterMatrix::Get();

//     dec  ah                             ;character height minus one
    --r.h.ah;

//     cmp  byte ptr ds:[BDA_DisplayMode], BDA_DM_80x25_Monochrome_Text;Offset 0x449, 0x7
//     jne  DontSetUnderline               ;Offset 0x309f
    if (Hag::System::BDA::DisplayMode::Get() != 0x07)
        goto DontSetUnderline;

//     mov  al, 14h                        ;CR14 - Underline Location register
    r.h.al = 0x14;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

// DontSetUnderline:                       ;Offset 0x309f
LABEL(ConfigureCursorPropertiesAndVerticalDisplayEnd, DontSetUnderline);

//     mov  ch, ah                         ;character height minus one
    r.h.ch = r.h.ah;

//     mov  cl, ah                         ;character height minus one
    r.h.cl = r.h.ah;

//     mov  al, 09h                        ;CR9 - Maximum Scan Line register
    r.h.al = 0x09;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 0e0h                       ;Preserve top 3 bits
    r.h.ah &= 0xe0;

//     or   ah, ch                         ;bits 4-0 - Number of scan lines per character row minus one
    r.h.ah |= r.h.ch;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     dec  ch                             ;character height minus two
    --r.h.ch;

//     mov  ah, cl                         ;character height minus one
    r.h.ah = r.h.cl;

//     cmp  ah, 0ch                        ;if less than or equal to twelve 12
//     jbe  LessThanTwelve                 ;Offset 0x30bb
    if (r.h.ah <= 0x0C)
        goto LessThanTwelve;

//     sub  cx, 0101h                      ;ch = character height minus three, cl = character height minus two
    r.w.cx -= 0x0101;

// LessThanTwelve:                         ;Offset 0x30bb
LABEL(ConfigureCursorPropertiesAndVerticalDisplayEnd, LessThanTwelve);

//     mov  word ptr ds:[BDA_CursorEndScanLine], cx;Offset 0x460 update BDA
    Hag::System::BDA::CursorScanLines::Get().End = r.h.cl;
    Hag::System::BDA::CursorScanLines::Get().Start = r.h.ch;

//     mov  al, 0ah                        ;CRA - Cursor Start Scan Line register
    r.h.al = 0x0A;

//     mov  ah, ch                         ;character height minus two/three
    r.h.ah = r.h.ch;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     inc  al                             ;CRB - Cursor End Scan Line register
    ++r.h.al;

//     mov  ah, cl                         ;character height minus one/two
    r.h.ah = r.h.cl;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484 - rows on screen minus one
    r.h.al = Hag::System::BDA::RowsOnScreen::Get();

//     inc  al                             ;rows on screen
    ++r.h.al;

//     mul  byte ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485 - ax = height in pixels
    r.h.ah = 0x00;
    r.w.ax *= Hag::System::BDA::PointHeightOfCharacterMatrix::Get();

//     cmp  bx, 0c8h                       ;200
//     jne  Not200Height                   ;Offset 0x30da
    if (r.w.bx != 0xC8)
        goto Not200Height;

//     shl  ax, 01h                        ;height in pixels * 2
    r.w.ax <<= 1;

// Not200Height:                           ;Offset 0x30da
LABEL(ConfigureCursorPropertiesAndVerticalDisplayEnd, Not200Height);

//     dec  ax                             ;height in pixels (* 2) minus one
    --r.w.ax;

//     mov  ah, al                         ;lower bits
    r.h.ah = r.h.al;

//     mov  al, 12h                        ;CR12 - Vertical Display End Register
//                                         ;11-bit value = (number of scan lines of active display) - 1. 
//                                         ;This register contains the least significant 8 bits of this value.
    r.h.al = 0x12;
    
//     out  dx, ax                         ;
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     ret
}

void SetFont()
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (PointHeightOfCharacterMatrix::Get() == 14)
    {
        SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x14, 0, 256, 14, DisplayMode::Get() == VideoMode::T80x25x2M ? 0x80 : 0);
    }
    else if (PointHeightOfCharacterMatrix::Get() == 8)
    {
        SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x8, 0, 256, 8, 0);
    }
    else
    {
        SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x16, 0, 256, 16, 0x40);
    }
}

bool CheckValidInCurrentMode(uint8_t* ptr, uint16_t offset)
{
    using namespace Hag::System::BDA;
    ptr += offset;

    uint8_t displayMode = DisplayMode::Get();
    do
    {
        if(*ptr == 0xFF)
            return false;

        if (*ptr == displayMode)
            return true;

        ++ptr;
    } while (true);
}

bool FetchCheckedVideoParameterBlockElement(uint16_t paramTableIdx, uint16_t subIdx, uint8_t*& parameterBlockElement, uint16_t size = sizeof(FARPointer))
{
    if (GetVideoParameterBlockElement(paramTableIdx, parameterBlockElement, size))
        return CheckValidInCurrentMode(parameterBlockElement, subIdx);
    return false;
}

void ConfigureFontAndCursor(uint8_t* fontDefinition)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));

//     mov  al, byte ptr es:[bx + 0ah]     ;number of character rows displayed
    r.h.al = fontDefinition[0x0A];
    uint16_t axtmp = r.w.ax;

//     push ax                             ;save that
//     mov  cx, word ptr es:[bx + 02h]     ;number of characters defined
    r.h.cl = fontDefinition[0x02];
    r.h.ch = fontDefinition[0x03];

//     mov  dx, word ptr es:[bx + 04h]     ;first character code in table
    r.h.dl = fontDefinition[0x04];
    r.h.dh = fontDefinition[0x05];

//These lines sat below the font pointer fetching code, but i needed the character length for the size.
//     mov  bx, word ptr es:[bx]           ;bl = length of each character, bh = character generator ram bank
//     xchg bl, bh                         ;bh = length of each character, bl = character generator ram bank
    r.h.bh = fontDefinition[0x00];
    r.h.bl = fontDefinition[0x01];

//     mov  si, word ptr es:[bx + 06h]     ;offset to character font
//     mov  ax, word ptr es:[bx + 08h]     ;segment to character font
//     mov  es, ax                         ;segment to character font   //I moved this line from further down.
    uint8_t* font = ((FARPointer*)(fontDefinition + 0x06))->ToPointer<uint8_t>(r.h.bh * r.w.cx);

//     and  bl, 3fh                        ;keep the lowest 6 bits
    r.h.bl &= 0x3F;

    SetTextFontAndAddressing(font, r.w.dx, r.w.cx, r.h.bh, r.h.bl);
    ConfigureCursorPropertiesAndVerticalDisplayEnd(r.h.bh);

//     pop  ax
    r.w.ax = axtmp;
//     cmp  al, 0ffh
//     je   Failure                        ;Offset 0x1c18
    if (r.h.al != 0xFF)
    {
//     sub  al, 01h
        --r.h.al;
//     mov  byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
        Hag::System::BDA::RowsOnScreen::Get() = r.h.al;
    }
// Failure:                                ;Offset 0x1c18
//     ret
}

void ConfigureFontRamBank(uint8_t* fontDefinition)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));

//     mov   al, byte ptr es:[bx]          ;
    r.h.al = *fontDefinition;

//     xor   ah, ah                        ;
    r.h.ah = 0x00;

//     cmp   ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
//     jne   AlreadySet                    ;Offset 0x1c64
    if (r.w.ax == Hag::System::BDA::PointHeightOfCharacterMatrix::Get())
        return;

//     mov   cx, 100h
    r.w.cx = 0x0100;

//     xor   dx, dx
    r.w.dx = 0x0000;

//     mov   bx, word ptr es:[bx]
//     xchg  bl, bh
    r.h.bh = fontDefinition[0x00];
    r.h.bl = fontDefinition[0x01];

//     mov   si, word ptr es:[bx + 03h]
//     mov   ax, word ptr es:[bx + 05h]
//     mov   es, ax
    uint8_t* font = ((FARPointer*)(fontDefinition + 0x03))->ToPointer<uint8_t>(r.h.bh * r.w.cx);

//     and   bl, 3fh
    r.h.bl &= 0x3f;

    SetTextFontAndAddressing(font, r.w.dx, r.w.cx, r.h.bh, r.h.bl);

//     mov   dx, 03c4h                     ;port - 0x3c4
    r.w.dx = 0x3c4;

//     mov   al, 03h
    r.h.al = 0x03;

//     call  ReadDataWithIndexRegisterNext ;Offset 0x4649
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and   ah, 13h
    r.h.ah &= 0x13;

//     pop   bx
//     mov   bh, bl
    r.h.bh = r.h.bl;

//     and   bl, 03h
    r.h.bl &= 0x03;

//     mov   cl, 02h
    r.h.cl = 0x02;

//     shl   bl, cl
    r.h.bl <<= r.h.cl;

//     and   bh, 04h
    r.h.bh &= 0x04;

//     inc   cl
    ++r.h.cl;

//     shl   bh, cl
    r.h.bh <<= r.h.cl;

//     or    ah, bl
    r.h.ah |= r.h.bl;

//     or    ah, bh
    r.h.ah |= r.h.bh;

//     mov   al, 03h
    r.h.al = 0x03;

//     out   dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

// AlreadySet :                            ;Offset 0x1c64
//     ret
}

void ClearScreen()
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    uint8_t flags = 0;
    uint16_t* ptr = NULL;

//     mov       cx, 4000h
    r.w.cx = 0x4000;

//     mov       bl, byte ptr ds:[BDA_DisplayMode];Offset 0449h
    r.h.bl = Hag::System::BDA::DisplayMode::Get();

//     cmp       bl, 13h
//     jbe       Label0x1d70
    if (r.h.bl <= 0x13)
        goto Label0x1d70;

//     mov       al, bl
    r.h.al = r.h.bl;

//     call      GetVideoModeFlags
//     jne       Label0x1d70
    if (!Hag::S3::TrioBase::GetVideoModeFlags(r.h.al, flags))
        goto Label0x1d70;

//     mov       bl, al
    r.h.bl = flags;

//     mov       bh, 0b8h
    r.h.bh = 0xB8;

//     mov       ax, 0720h
    r.w.ax = 0x720;

//     test      bl, 02h
//     jne       Label0x1d68
    if ((r.h.bl & 0x02) != 0x00)
        goto Label0x1d68;

//     mov       bh, 0b0h
    r.h.bh = 0xB0;

// Label0x1d68:
LABEL(ClearScreen, Label0x1d68);

//     test      bl, 01h                   ;text
//     jne       Label0x1d8c
    if ((r.h.bl & 0x01) != 0x00)
        goto Label0x1d8c;

//     jmp       EmptyFunction2            ;Offset 0x121d Tail call.
    return;

// Label0x1d70:                            ;Offset 0x1d70
LABEL(ClearScreen, Label0x1d70);

//     mov       bh, 0b0h
    r.h.bh = 0xB0;

//     mov       ax, 0720h
    r.w.ax = 0x720;

//     cmp       bl, 07h
//     je        Label0x1d8c
    if (r.h.bl == 0x07)
        goto Label0x1d8c;
        
//     mov       bh, 0b8h
    r.h.bh = 0xB8;
//     cmp       bl, 03h
//     jbe       Label0x1d8c
    if (r.h.bl <= 0x03)
        goto Label0x1d8c;

//     xor       ax, ax
    r.w.ax = 0x0000;

//     cmp       bl, 06h
//     jbe       Label0x1d8c
    if (r.h.bl <= 0x06)
        goto Label0x1d8c;

//     mov       bh, 0a0h
    r.h.bh = 0xA0;

//     mov       ch, 80h
    r.h.ch = 0x80;

// Label0x1d8c:
LABEL(ClearScreen, Label0x1d8c);
//     xor       bl, bl
    r.h.bl = 0x00;

//     mov       es, bx
    ptr = FARPointer(r.w.bx, 0x0000).ToPointer<uint16_t>(r.w.cx << 1);

//     xor       di, di
//     rep       stosw
    do
    {
        *ptr = r.w.ax;
        ++ptr;
        --r.w.cx;
    } while (r.w.cx != 0x0000);

//     ret
}

void SetPaletteProfile()
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    uint8_t* secondarySavePointerTable = NULL;
    uint8_t* paletteProfile = NULL;
    uint8_t* paletteColors = NULL;
    uint8_t* attributeRegisters = NULL;
    uint8_t t = 0;

    //     mov   bx, 10h                       ;Secondary Save Pointer Table pointer (VGA)
    r.w.bx = 0x10;

    //     call  GetVideoParameterBlockElement ;Offset 0x1d95
    if (!GetVideoParameterBlockElement(r.w.bx, secondarySavePointerTable, 0x20))
        return;

    //     je    NotFound                      ;Offset 0x1cce
    //     les   bx, es:[bx + 0ah]             ;pointer to user palette profile table  (VGA)
    //     mov   ax, es
    //     or    ax, bx
    //     je    NotFound                      ;Offset 0x1cce
    if (((FARPointer*)secondarySavePointerTable + 0x0A)->IsNull())
        return;
    paletteProfile = ((FARPointer*)secondarySavePointerTable + 0x0A)->ToPointer<uint8_t>(0x14 + 0x14);

    //     mov   ax, 14h                       ;array of applicable video modes for this font
    //     call  CheckValidInCurrentMode        ;Offset 0x1bd1
    if (!CheckValidInCurrentMode(paletteProfile, 0x14))
        return;

    //     je    Label0x1ccf                   ;Offset 0x1ccf
    // NotFound:                               ;Offset 0x1cce
    //     ret
    // Label0x1ccf:                            ;Offset 0x1ccf
    //     test  byte ptr ds:[BDA_VideoDisplayDataArea], 08h;Offset 0x489 //BDA_VDDA_PaletteLoadingEnabled
    //     jne   Label0x1d2b                   ;Offset 0x1d2b
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x08) != 0x00)
        goto Label0x1d2b;

    //     mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    r.w.dx = Hag::System::BDA::VideoBaseIOPort::Get();

    //     add   dx, 06h                       ;0x3?A - InputStatus1 - reset attribute controller register flip flop
    r.w.dx += 0x06;

    //     in    al, dx
    r.h.al = SYS_ReadPortByte(r.w.dx);

    //     push  ds
    //     push  bx
    //     mov   ax, word ptr es:[bx + 0eh]    ;first video DAC color register number
    r.h.al = paletteProfile[0x0e];

    //     mov   ah, al                        ;
    r.h.ah = r.h.al;

    //     mov   bx, word ptr es:[bx + 0ch]    ;count of video DAC color registers in table
    r.h.bl = paletteProfile[0x0c];
    r.h.bh = paletteProfile[0x0d];

    //     lds   si, es:[bx + 10h]             ;video DAC color register table pointer
    paletteColors = ((FARPointer*)paletteProfile + 0x10)->ToPointer<uint8_t>(r.w.cx);

    //     or    bx, bx                        ;
    //     je    NoColorData                   ;Offset 0x1d06
    if (r.h.bh == 0x0000)
        goto NoColorData;

    //     mov   dx, DACWriteIndex             ;port - 0x3c8 - DACWriteIndex
    r.w.dx = Hag::VGA::Register::DACWriteIndex;

    // NextColor:                              ;Offset 0x1cf5
LABEL(SetPaletteProfile, NextColor);

    //     mov   al, ah
    r.h.al = r.h.ah;

    //     out   dx, al                        ;index
    SYS_WritePortByte(r.w.dx, r.h.al);

    //     inc   dx                            ;port - 0x3c9 - RAMDACData
    ++r.w.dx;

    //     mov   cx, 03h                       ;3 colors
    r.w.cx = 0x03;

    // WriteColors:                            ;Offset 0x1cfc
LABEL(SetPaletteProfile, WriteColors);

    //     lodsb byte ptr ds:[si]              ;write out R, G, B
    r.h.al = *paletteColors;
    ++paletteColors;

    //     out   dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

    //     loop  WriteColors                   ;Offset 0x1cfc
    --r.w.cx;
    if (r.w.cx != 0x0000)
        goto WriteColors;

    //     inc   ah
    ++r.h.ah;

    //     dec   dx
    --r.w.dx;

    //     dec   bx
    --r.w.bx;

    //     jne   NextColor                     ;Offset 0x1cf5
    if (r.w.bx != 0x0000)
        goto NextColor;

    // NoColorData:                            ;Offset 0x1d06
LABEL(SetPaletteProfile, NoColorData);
    //     pop   bx
    //     mov   ax, word ptr es:[bx + 06h]    ;first attribute controller register number
    r.h.al = paletteProfile[0x06];

    //     mov   ah, al
    r.h.ah = r.h.al;

    //     mov   cx, word ptr es:[bx + 04h]    ;count of attribute controller regs in table
    r.h.cl = paletteProfile[0x04];
    r.h.ch = paletteProfile[0x05];

    //     lds   si, es:[bx + 08h]             ;pointer to attribute controller reg table
    attributeRegisters = ((FARPointer*)paletteProfile + 0x08)->ToPointer<uint8_t>(r.w.cx);

    //     jcxz  NoAttributes                  ;Offset 0x1d2a
    if (r.w.cx == 0x0000)
        goto NoAttributes;
        
    //     mov   dx, AttributeControllerIndex  ;port - 0x3c0
    r.w.dx = 0x03c0;

    // NextAttribute:                          ;Offset 0x1d1a
LABEL(SetPaletteProfile, NextAttribute);

    //     mov   al, ah
    r.h.al = r.h.ah;

    //     out   dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

    //     lodsb byte ptr ds:[si]
    r.h.al = *attributeRegisters;
    ++attributeRegisters;

    //     out   dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

    //     inc   ah
    ++r.h.ah;

    //     loop  NextAttribute                 ;Offset 0x1d1a
    --r.w.cx;
    if (r.w.cx != 0x0000)
        goto NextAttribute;

    //     inc   ah
    ++r.h.ah;

    //     mov   al, ah
    r.h.al = r.h.ah;

    //     out   dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

    //     lodsb byte ptr ds:[si]
    r.h.al = *attributeRegisters;
    ++attributeRegisters;

    //     out   dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);
    
    // NoAttributes:                           ;Offset 0x1d2a
LABEL(SetPaletteProfile, NoAttributes);

    //     pop   ds
    // Label0x1d2b:                            ;Offset 0x1d2b
LABEL(SetPaletteProfile, Label0x1d2b);

    //     mov   al, byte ptr es:[bx]          ;1 - enable underlining in all alphanumeric modes
    // 		                                   ;0 - enable underlining in monochrome alpha modes
    // 		                                   ;-1 - disable underlining in all alpha modes
    r.h.al = paletteProfile[0x00];

    //     or    al, al
    //     je    Label0x1d46                   ;Offset 0x1d46
    if (r.h.al == 0x00)
        goto Label0x1d46;

    //     test  al, 80h
    t = r.h.al;
    r.h.al = 0x1f;

    //     mov   al, 1fh
    //     jne   Label0x1d3d                   ;Offset 0x1d3d
    if ((t & 0x80) != 0x00)
        goto Label0x1d3d;

    //     mov   ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
    r.w.ax = Hag::System::BDA::PointHeightOfCharacterMatrix::Get();

    //     dec   al
    --r.h.al;

    // Label0x1d3d:                            ;Offset 0x1d3d
LABEL(SetPaletteProfile, Label0x1d3d);

    //     mov   dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    r.w.dx = Hag::System::BDA::VideoBaseIOPort::Get();

    //     mov   ah, al
    r.h.ah = r.h.al;

    //     mov   al, 14h
    r.h.al = 0x4;
    
    //     out   dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

    // Label0x1d46:                            ;Offset 0x1d46
LABEL(SetPaletteProfile, Label0x1d46);
    
    //     ret
}

void SetGraphicsCharacterFont(uint8_t* graphicsCharacterFontDefinition)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    // mov   al, byte ptr es:[bx]          ;number of displayed character rows
    r.h.al = graphicsCharacterFontDefinition[0];

    // dec   al
    --r.h.al;

    // mov   byte ptr ds:[BDA_RowsOnScreen], al;Offset 0x484
    Hag::System::BDA::RowsOnScreen::Get() = r.h.al;

    // mov   ax, word ptr es:[bx + 01h]    ;length of character definition in bytes
    r.h.al = graphicsCharacterFontDefinition[1];
    r.h.ah = graphicsCharacterFontDefinition[2];

    // mov   word ptr ds:[BDA_PointHeightOfCharacterMatrix], ax;Offset 0x485
    Hag::System::BDA::PointHeightOfCharacterMatrix::Get() = r.w.ax;
    
    // mov   ax, word ptr es:[bx + 03h]    ;offset of character font
    // mov   bx, word ptr es:[bx + 05h]    ;segment of character font
    // mov   es, word ptr cs:[Data1488]    ;Offset 0x1488
    // mov   di, 010ch                     ;Offset 0x10c int 0x43 - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
    // cli
    // stosw word ptr es:[di]
    // mov   ax, bx
    // stosw word ptr es:[di]
    // sti
    // ret
}

// ;inputs:
// ;al = requested video mode
bool SetVideoMode(uint8_t mode)
{
    using namespace Hag::VGA;

    REGPACK r;
    memset(&r, 0, sizeof(r));
    uint32_t offset = 0;
    Hag::S3::VideoParameters* overrideTable = NULL;
    uint8_t* fontDefinition = NULL;
    uint8_t* graphicsCharacterFontDefinition = NULL;
    Hag::S3::VESAVideoModeData* vesaData = NULL;
    uint8_t* paramBlock = NULL;
    Register_t crtcPort = Hag::System::BDA::VideoBaseIOPort::Get();
    uint8_t modeDataIndex = 0;
    uint8_t flags = 0;
    r.h.al = mode;

//     call UnlockExtendedCRTRegistersSafe ;Offset 0x1374
//     jne  Failure                        ;Offset 0x182b
    if (!UnlockExtendedCRTRegistersSafe())
        goto Failure;

//     mov  ah, al                         ;al = requested video mode
    r.h.ah = r.h.al;

//     and  al, NOT INT10_00_xx_DontClearDisplay;0x7f
    r.h.al &= 0x7f;

//     cmp  al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
//     jbe  ValidVideoMode                 ;Offset 0x182c
    if (r.h.al <= 0x13)
        goto ValidVideoMode;

//     call FindVideoModeData              ;Offset 0x103a
//     je   ValidVideoMode                 ;Offset 0x182c
    if (vesaData = Hag::S3::TrioBase::FindVideoModeData(r.h.al))
        goto ValidVideoMode;

// Failure:                                ;Offset 0x182b
LABEL(SetVideoMode, Failure);

//     ret
    return false;

// ValidVideoMode:                         ;Offset 0x182c
LABEL(SetVideoMode, ValidVideoMode);

//     and  byte ptr ds:[BDA_VideoModeOptions], NOT BDA_VMO_DontClearDisplay;Offset 0x487, 0x7f
    Hag::System::BDA::VideoModeOptions::Get() &= 0x7f;

//     and  ah, INT10_00_xx_DontClearDisplay;0x80
    r.h.ah &= 0x80;

//     or   byte ptr ds:[BDA_VideoModeOptions], ah;Offset 0x487
    Hag::System::BDA::VideoModeOptions::Get() |= r.h.ah;

//     call ModeSetBDA                     ;Offset 0x18e8
    ModeSetBDA(r.h.al);
//     cmp  al, INT10_00_13_G_40x25_8x8_320x200_256C_x_A000;0x13
//     ja   IsVESAMode                     ;Offset 0x1850
    if (r.h.al > 0x13)
        goto IsVESAMode;

//     call VerifyBDAOrDeactivate               ;Offset 0x1a4e - This function seems to destroy al?
//     je   IsVESAMode                     ;Offset 0x1850
    if (VerifyBDAOrDeactivate(r.h.al))
        goto IsVESAMode;

//     and  al, 0dfh                       ;bit 5 = 0
//     mov  byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
//     call EmptyFunction3                 ;Offset 0x1380
//     call SetSystemAdapterVideoMode      ;Offset 0x1acc
//     ret
        return false; //We're not doing the above. We're not calling into the system BIOS.

// IsVESAMode:                             ;Offset 0x1850
LABEL(SetVideoMode, IsVESAMode);

//     push cx
//     mov  byte ptr ds:[BDA_DisplayMode], al;Offset 0x449
    Hag::System::BDA::DisplayMode::Get() = r.h.al;

//     call SetTextModeBiosData            ;Offset 0x1b05
    overrideTable = SetTextModeBiosData(r.h.al);

//     and  byte ptr ds:[BDA_VideoModeOptions], 0f3h;Offset 0x487
    Hag::System::BDA::VideoModeOptions::Get() &= 0xf3;

//     call SaveDynamicParameterData       ;Offset 0x1bac
    SaveDynamicParameterData(overrideTable);

//     call ApplyVideoParameters           ;Offset 0x4829
    ApplyVideoParameters(overrideTable);

//     mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    r.h.al = Hag::System::BDA::DisplayMode::Get();

//     call ApplyVESAOverrideData          ;Offset 0x1079
    ApplyVESAOverrideData(r.h.al, GetCRTControllerIndexRegister(), vesaData);

//     call SetPalette                     ;Offset 0x4909
    SetPalette(mode);

//     mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    r.w.dx = crtcPort;

//     mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    r.h.al = Hag::System::BDA::DisplayMode::Get();

//     cmp  al, BDA_DM_320x200_4_Color_Graphics1;0x04
//     jb   Label0x1885                    ;Offset 0x1885
    if (r.h.al < 0x04)
        goto Label0x1885;

//     cmp  al, BDA_DM_80x25_Monochrome_Text;0x07
//     je   Label0x1885                    ;Offset 0x1885
    if (r.h.al == 0x07)
        goto Label0x1885;

//     cmp  al, BDA_DM_320x200_256_Color_Graphics;0x13
//     jbe  Label0x18b5                    ;Offset 0x18b5
    if (r.h.al <= 0x13)
        goto IsGraphicsMode;

//     call GetVideoModeFlags              ;Offset 0x105d
    Hag::S3::TrioBase::GetVideoModeFlags(r.h.al, flags);

//     test al, 01h                        ;Text
//     je   Label0x18b5                    ;Offset 0x18b5
    if ((flags & 0x01) == 0x00)
        goto IsGraphicsMode;

// Label0x1885:                            ;Offset 0x1885
LABEL(SetVideoMode, Label0x1885);

//     call Func0x1c65                     ;Offset 0x1c65
    SetFont();

//     mov  ax, 000bh                      ;ax = sub index to mode list
    r.w.ax = 0x000b;

//     mov  bx, 0008h                      ;Fetch element 3 - Alphanumeric Character Set Override pointer
    r.w.bx = 0x0008;

//     call FetchCheckedVideoParameterBlockElement;Offset 0x1bc9
//     jne  CharacterSetNotFound           ;Offset 0x1896
    if (!FetchCheckedVideoParameterBlockElement(r.w.bx, r.w.ax, fontDefinition, 0x0B + 0x14))
        goto CharacterSetNotFound;

//     call ConfigureFontAndCursor         ;Offset 0x1bea
    ConfigureFontAndCursor(fontDefinition);

// CharacterSetNotFound:                   ;Offset 0x1896
LABEL(SetVideoMode, CharacterSetNotFound);

//     mov  bx, 0010h                      ;Secondary Save Pointer Table pointer (VGA)
    r.w.bx = 0x0010;

//     call GetVideoParameterBlockElement  ;Offset 0x1d95
//     je   Label0x18c9                    ;Offset 0x18c9
    if (!GetVideoParameterBlockElement(r.w.bx, paramBlock, 0x20))
        goto Label0x18c9;

//     or   ax, bx
//     je   Label0x18c9                    ;Offset 0x18c9 - No font definition found
    if (((FARPointer*)(paramBlock + 0x06))->IsNull())
        goto Label0x18c9;

//     les  bx, es:[bx + 06h]              ;Pointer to Character font definition table
//     mov  ax, es
    fontDefinition = ((FARPointer*)(paramBlock + 0x06))->ToPointer<uint8_t>(0x0B + 0x14);//there's a 0xFF terminated list of modes. max is 14h

//     mov  ax, 0007h
    r.w.ax = 0x0007;

//     call CheckValidInCurrentMode         ;Offset 0x1bd1
//     jne  Label0x18c9                    ;Offset 0x18c9
    if (!CheckValidInCurrentMode(fontDefinition, r.w.ax))
        goto Label0x18c9;

//     call Func0x1c19                     ;Offset 0x1c19
    ConfigureFontRamBank(fontDefinition);

//     jmp  Label0x18c9                    ;Offset 0x18c9
    goto Label0x18c9;

// Label0x18b5:                            ;Offset 0x18b5
LABEL(SetVideoMode, IsGraphicsMode);
//     mov  word ptr ds:[BDA_CursorEndScanLine], 00h;Offset 0x460
    Hag::System::BDA::CursorScanLines::Get().End = 0x00;
    Hag::System::BDA::CursorScanLines::Get().Start = 0x00;

//     mov  ax, 0007h                      ;ax = sub index to mode list
    r.w.ax = 0x0007;

//     mov  bx, 000ch                      ;Fetch element 4 - Graphics Character Set Override pointer
    r.w.bx = 0x000C;

//     call FetchCheckedVideoParameterBlockElement;Offset 0x1bc9
//     jne  Label0x18c9                    ;Offset 0x18c9
    if (!FetchCheckedVideoParameterBlockElement(r.w.bx, r.w.ax, graphicsCharacterFontDefinition, 0x07 + 0x14))
        goto Label0x18c9;

//     call SetGraphicsCharacterFont       ;Offset 0x1c8e
    SetGraphicsCharacterFont(graphicsCharacterFontDefinition); //Sets the pointer in the interrupt table.

// Label0x18c9:                            ;Offset 0x18c9
LABEL(SetVideoMode, Label0x18c9);

//     test byte ptr ds:[BDA_VideoModeOptions], 80h;Offset 0x487
//     jne  Label0x18da                    ;Offset 0x18da
    if ((Hag::System::BDA::VideoModeOptions::Get() & 0x80) != 0x00)
        goto Label0x18da;

//     mov  ax, word ptr ds:[BDA_VideoBufferSize];Offset 0x44c
    r.w.ax = Hag::System::BDA::VideoBufferSize::Get();

//     or   ax, ax
//     je   Label0x18da                    ;Offset 0x18da
    if (r.w.ax == 0x0000)
        goto Label0x18da;

//     call ClearScreen                     ;Offset 0x1d47
    ClearScreen();

// Label0x18da:                            ;Offset 0x18da
LABEL(SetVideoMode, Label0x18da);

//     call Func0x1cb4                     ;Offset 0x1cb4
    SetPaletteProfile();

//     call EnablePaletteBasedVideo        ;Offset 0x479d
    EnablePaletteBasedVideo(crtcPort);

//     call TurnOnScreen                   ;Offset 0x4800
    Hag::VGA::Sequencer::ClockingMode::TurnScreenOn();

//     pop  cx
//     call EmptyFunction3                 ;Offset 0x1380
//     ret  
    return true;
}

}

#pragma pack(pop);
