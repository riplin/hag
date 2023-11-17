//Copyright 2023-Present riplin

#include <i86.h>
#include <stdio.h>
#include <string.h>

#include <hag/system/bda.h>
#include <hag/system/sysasm.h>

#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/sqrc/reset.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/memodctl.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/regs.h>

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
#include <hag/drivers/s3/crtc/bkwcomp3.h>
#include <hag/drivers/s3/crtc/conf1.h>
#include <hag/drivers/s3/crtc/devidhi.h>
#include <hag/drivers/s3/crtc/devidlo.h>
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
#include <hag/drivers/s3/sqrc/mclkvhi.h>
#include <hag/drivers/s3/sqrc/mclkvlow.h>
#include <hag/drivers/s3/sqrc/rclksync.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>
#include <hag/drivers/s3/sqrc/extseqd.h>

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
        (S3::TrioBase::GetVideoModeFlags(mode, flags) && ((flags & S3::VESAVideoModeFlags::Unknown1) == 0)))
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
                ((flags & S3::VESAVideoModeFlags::Unknown1) == 0)))
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
                (flags & S3::VESAVideoModeFlags::Unknown1) == 0))
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

void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, uint16_t crtcPort)
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
        S3::CRTController::SystemConfiguration::Write(crtcPort, systemConfiguration |
                                                      S3::CRTController::SystemConfiguration::EnableEnhancedRegisterAccess);

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

    S3::CRTController::SystemConfiguration::Write(crtcPort,
                                                  S3::CRTController::SystemConfiguration::Read(crtcPort) |
                                                  S3::CRTController::SystemConfiguration::EnableEnhancedRegisterAccess);

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

void EnableOver256KAddressingAndSetAddressWindow(uint8_t mode, uint16_t crtcPort)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.h.al = mode;
    r.w.dx = crtcPort;

//     push ax
//     mov  bl, al
    r.h.bl = r.h.al;

//     cmp  bl, 13h
//     jbe  Label0x307                     ;Offset 0x307
    if (r.h.bl <= 0x13)
        goto Label0x307;

//     mov  al, 32h                        ;CR32 - Backward Compatibility 1 register
    r.h.al = 0x32;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 0bfh                       ;set bit 6 to 0 - Memory accesses extending past a 256K boundary do not wrap
    r.h.ah &= 0xbf;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

// Label0x307:                             ;Offset 0x307
LABEL(EnableOver256KAddressingAndSetAddressWindow, Label0x307);

//     mov  al, 6bh                        ;CR6B - Extended BIOS Flag 3 register
    r.h.al = 0x6b;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     mov  al, 59h                        ;CR59 - Linear Address Window Position register MSB
    r.h.al = 0x59;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  al, 6ch                        ;CR6C - Extended BIOS Flag 4 register
    r.h.al = 0x6c;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 80h                        ;Only keep the top bit
    r.h.ah &= 0x80;

//     mov  al, 5ah                        ;CR5A - Linear Address Window Position register LSB
    r.h.al = 0x5a;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     pop  ax
//     ret  
}

// ;inputs:
// ;al = video mode
void SetColorMode(uint8_t mode)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.h.al = mode;
    Hag::S3::VESAVideoModeData* videoModeData = NULL;

//     push dx
//     push bx
//     call FindVideoModeData              ;Offset 0x103a
    videoModeData = Hag::S3::TrioBase::FindVideoModeData(mode);

//     cmp  al, 13h
//     jbe  LegacyMode                     ;Offset 0xe18
    if (r.h.al <= 0x13)
        goto LegacyMode;

//     mov  al, (VideoModeData ptr es:[di + offset VESAVideoModeData]).ColorMode;Offset 0x527 - Translate video mode
    r.h.al = videoModeData->ColorMode;
    
//     cmp  al, 05h
//     je   ColorMode8                     ;Offset 0xe31
    if (r.h.al == 0x05)
        goto ColorMode8;

//     cmp  al, 04h
//     je   ColorMode13                    ;Offset 0xe1d
    if (r.h.al == 0x04)
        goto ColorMode13;

//     cmp  al, 03h
//     je   ColorMode12                    ;Offset 0xe22
    if (r.h.al ==0x03)
        goto ColorMode12;

//     cmp  al, 02h
//     je   ColorMode10                    ;Offset 0xe27
    if (r.h.al == 0x02)
        goto ColorMode10;

//     cmp  al, 01h
//     je   ColorMode9                     ;Offset 0xe2c
    if (r.h.al == 0x01)
        goto ColorMode9;

// LegacyMode:                             ;Offset 0xe18
LABEL(SetColorMode, LegacyMode);

//     xor  cx, cx                         ;Color Mode 0000 - Mode 0: 8-bit color, 1 pixel/VCLK
    r.w.cx = 0x0000;

//     jmp  ConfigureClocks                ;Offset 0xe33
//     nop
    goto ConfigureClocks;

// ColorMode13:                            ;Offset 0xe1d
LABEL(SetColorMode, ColorMode13);

//     mov  cl, 0d0h                       ;Color Mode 1101 - Mode 13: 24-bit color, 1 pixel/VCLK
    r.h.cl = 0xd0;

//     jmp  ConfigureClocks                ;Offset 0xe33
//     nop
    goto ConfigureClocks;

// ColorMode12:                            ;Offset 0xe22
LABEL(SetColorMode,ColorMode12);

//     mov  cl, 70h                        ;Color Mode 0111 - Mode 12: 640x480x24-bit color (packed), 1 pixel/3 DCLKs (Trio 32 only)
    r.h.cl = 0x70;

//     jmp  ConfigureClocks                ;Offset 0xe33
//     nop
    goto ConfigureClocks;

// ColorMode10:                            ;Offset 0xe27
LABEL(SetColorMode,ColorMode10);

//     mov  cl, 50h                        ;Color Mode 0101 - Mode 10: 16-bit color, 1 pixel/VCLK
    r.h.cl = 0x50;

//     jmp  ConfigureClocks                ;Offset 0xe33
//     nop
    goto ConfigureClocks;

// ColorMode9:                             ;Offset 0xe2c
LABEL(SetColorMode, ColorMode9);

//     mov  cl, 30h                        ;Color Mode 0011 - Mode 9: 15-bit color, 1 pixel/VCLK
    r.h.cl = 0x30;

//     jmp  ConfigureClocks                ;Offset 0xe33
//     nop
    goto ConfigureClocks;

// ColorMode8:                             ;Offset 0xe31
LABEL(SetColorMode, ColorMode8);

//     mov  cl, 10h                        ;Color Mode 0001 - Mode 8: 8-bit color, 2 pixels/VCLK
    r.h.cl = 0x10;

// ConfigureClocks:                        ;Offset 0xe33
LABEL(SetColorMode, ConfigureClocks);

//     mov  dx, SequenceIndex              ;port - 0x3c4
    r.w.dx = 0x3c4;

//     mov  ax, 0608h                      ;SR8 - Unlock Extended Sequencer register 0x06 = unlock code.
    r.w.ax = 0x0608;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     cmp  cl, 10h                        ;Color Mode 0001 - Mode 8: 8-bit color, 2 pixels/VCLK
//     jne  NotMode8                       ;Offset 0xe4f
    if (r.h.cl != 0x10)
        goto NotMode8;

//     mov  ax, 0c018h                     ;SR18 - RAMDAC/CLKSYN Control register
//                                         ;bit 6 = 1 - DCLK LUT write cycle
//                                         ;but 7 = 1 - RAMDAC clock doubled mode (0001) enabled
    r.w.ax = 0xc018;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  al, 15h                        ;SR15 - CLKSYN Control 2 register
    r.h.al = 0x15;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     or   ah, 10h                        ;bit 4 = 1 - Divide DCLK by 2
    r.h.ah |= 0x10;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     jmp  ApplyColorMode                 ;Offset 0xe5c
//     nop
    goto ApplyColorMode;

// NotMode8:                               ;Offset 0xe4f
LABEL(SetColorMode, NotMode8);
//     mov  ax, 4018h                      ;SR18 - RAMDAC/CLKSYN Control register
//                                         ;bit 6 - Invert DCLK - Either this bit or bit 4 of this register
//                                         ;                      must be set to 1 for clock doubled RAMDAC
//                                         ;                      Operation (mode 0001).
    r.w.ax = 0x4018;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  al, 15h                        ;SR15 - CLKSYN Control 2 register
    r.h.al = 0x15;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 0afh                       ;bit 6 = 0 - DCLK unchanged (not inverted)
//                                         ;bit 4 = 0 - DCLK unchanged (not divided by 2)
    r.h.ah &= 0xaf;
    
//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

// ApplyColorMode:                         ;Offset 0xe5c
LABEL(SetColorMode, ApplyColorMode);

//     call GetCRTControllerIndexRegister  ;Offset 0xfdd
    r.w.dx = GetCRTControllerIndexRegister();

//     mov  al, 67h                        ;CR67 - Extended Miscellaneous Control 2 register
    r.h.al = 0x67;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 0fh                        ;Clear out top 4 bits - Color Mode
    r.h.ah &= 0x0f;

//     or   ah, cl                         ;Set Color Mode
    r.h.ah |= r.h.cl;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     pop  bx
//     pop  dx
//     ret
}

void ApplyVESAOverrideData(uint8_t mode)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.h.al = mode;
    uint32_t offset = 0;
    uint16_t idx = 0;
    uint16_t crtIdx = 0;
    Hag::VGA::Register_t crtcPort = GetCRTControllerIndexRegister();
    Hag::S3::VESAVideoModeData* overrideTable = NULL;
    Hag::S3::VESAResolutionVariant* modeData = NULL;

//     push ax
//     push bx
//     push cx
//     push es
//     mov  bl, al
    r.h.bl = r.h.al;

//     call UnlockExtendedSequencerRegisters;Offset 0xf92
    Hag::S3::Sequencer::UnlockExtendedSequencer::Unlock();

//     mov  dx, SequenceIndex              ;port - 0x3c4
    r.w.dx = 0x3c4;

//     mov  al, 09h                        ;SR9 - Extended Sequencer register 9
    r.h.al = 0x09;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 7fh                        ;Set bit 7 to 0 - Turn off Memory IO only mode. Both programmed IO and memory IO are allowed.
    r.h.ah &= 0x7f;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     call GetCRTControllerIndexRegister  ;Offset 0xfdd
    r.w.dx = crtcPort;

//     mov  al, 30h                        ;CR30 - Chip ID / Revision register
    r.h.al = 0x30;

//     mov  ah, 55h                        ;Unknown... We are writing to what the documentation says is a read-only register...
    r.h.ah = 0x55;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     cmp  ah, 55h                        ;Check if the write worked
//     je   Label0x10b9                    ;Offset 0x10b9
    if (r.h.ah == 0x55)
        goto Label0x10b9;

//     mov  al, 40h                        ;CR40 - System Configuration register
    r.h.al = 0x40;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     or   ah, 01h                        ;Set bit 0 to 1 - Enhanced register (x2e8h) access enabled
    r.h.ah |= 0x01;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     push dx
//     mov  dx, AdvancedFunctionControl    ;port - 0x4ae8
    r.w.dx = 0x4ae8;

//     mov  al, 02h                        ;Set bit 1 to 1 - Unknown
    r.h.al = 0x02;

//     out  dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

    r.w.dx = crtcPort;
//     push si
//     mov  si, word ptr cs:[CRTControllerInitPtr];Offset 0x1bd points to CRTControllerInitData
//     call InitializeCRTControllerAndSequencer;Offset 0xfee
    InitializeCRTControllerAndSequencer(Hag::S3::TrioBase::m_CRTControllerInitData, crtcPort);

//     pop  si
// Label0x10b9:                            ;Offset 0x10b9
LABEL(ApplyVESAOverrideData, Label0x10b9);

//     mov  al, bl
    r.h.al = r.h.bl;
//     call FindVideoModeData              ;Offset 0x103a
//     je   Found                          ;Offset 0x10c3
    if (overrideTable = Hag::S3::TrioBase::FindVideoModeData(r.h.al))
        goto Found;

//     jmp  NotVESAMode                    ;Offset 0x115c
    goto NotVESAMode;

// Found:                                  ;Offset 0x10c3
LABEL(ApplyVESAOverrideData, Found);
    modeData = overrideTable->VariantData;

//     call ClearMemory                    ;Offset 0x121e
    ClearMemory(crtcPort);

//     call EmptyFunction1                 ;Offset 0x230
//     mov  al, 41h                        ;CR41 - BIOS Flag register
    r.h.al = 0x41;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     mov  bh, ah                         ;
    r.h.bh = r.h.ah;

//     mov  al, 52h                        ;CR52 - Extended BIOS Flag 1 register
    r.h.al = 0x52;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 0f0h                       ;
    r.h.ah &= 0xf0;

//     mov  bl, ah
    r.h.bl = r.h.ah;

//     shr  bx, 04h                        ;
    r.w.bx >>= 4;

//     mov  ax, offset VESAVideoParameters640x480;Offset 0x6f1
    idx = 3; //VESAVideoParameters640x480 sits at index 3 in VESAVideoParameters

//     mov  cx, 03h                        ;Loop 3 times. Every time the Override table doesn't match,
    r.w.cx = 0x0003;

//                                         ;we move up to a higher resolution. 800x600, 1024x768, 1280x1024
//                                         ;While we do that, we drop 3 bits off the low end of bx.
// FindOverrideTable:                      ;Offset 0x10e3
LABEL(ApplyVESAOverrideData, FindOverrideTable);

//     cmp  (VideoModeData ptr es:[di + offset VESAVideoModeData]).OverrideTable, ax;Offset 0x523
//     je   OverrideTableFound             ;Offset 0x10f2
    if (overrideTable->OverrideTable == Hag::S3::TrioBase::m_VesaResolutions[idx])
        goto OverrideTableFound;

//     shr  bx, 03h
    r.w.bx >>= 3;

//     DB 05h,40h,00h                      ;add  ax, 40h - masm encoding is different, but it's the same instruction
    ++idx;

//     loop FindOverrideTable              ;Offset 0x10e3
    --r.w.cx;
    if(r.w.cx != 0x0000)
        goto FindOverrideTable;

// OverrideTableFound:                     ;Offset 0x10f2
LABEL(ApplyVESAOverrideData, OverrideTableFound);
//     and  bx, 07h                        ;Whatever we found (or didn't find), we are using the bottom 3 bits of bx.
    r.w.bx &= 0x0007;

//     mov  dx, SequenceIndex              ;port - 0x3c4
    r.w.dx = 0x3c4;

//     mov  al, 04h                        ;SR4 - MemoryModeControl
    r.h.al = 0x04;

//     mov  ah, (VideoModeData ptr es:[di + offset VESAVideoModeData]).MemoryMode;Offset 0x528
    r.h.ah = overrideTable->MemoryModeControl;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     call GetCRTControllerIndexRegister  ;Offset 0xfdd
    r.w.dx = GetCRTControllerIndexRegister();

//     push dx
//     mov  al, 40h                        ;CR40 - System Configuration register
    r.h.al = 0x40;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     or   ah, 01h                        ;Set bit 0 to 1 - Enable Enhanced (x2e8h) register access
    r.h.ah |= 0x01;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  al, 11h                        ;CR11 - Vertical Retrace End register
    r.h.al = 0x11;

//     call ReadDataWithIndexRegister      ;Offset 0x4640
    r.h.ah = ReadDataWithIndexRegister(r.w.dx, r.h.al);

//     and  ah, 7fh                        ;Set bit 7 to 0 - Enable writes to CRT controller registers
    r.h.ah &= 0x7f;
    
//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov  dx, AdvancedFunctionControl    ;port - 0x4ae8 - Advanced Function Control
    r.w.dx = 0x4ae8;

//     mov  al, (VideoModeData ptr es:[di + offset VESAVideoModeData]).AdvancedFunction;Offset 0x526
    r.h.al = overrideTable->AdvancedFunction;

//     out  dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

//     pop  dx                             ;Restore CRTC register
    r.w.dx = crtcPort;

//     mov  ah, bl
    r.h.ah = r.h.bl;

//     xor  bh, bh
    r.h.bh = 0x00;

//     mov  bl, byte ptr cs:[Data019E]     ;offset 0x19e
//Data019E                DB 0Ch                      ;Offset 0x19e - This is the size of... something
    r.h.bl = 0x0c;

//     mov  di, (VideoModeData ptr es:[di + offset VESAVideoModeData]).Value7;Offset 0x529

// DataSearch:                             ;Offset 0x112e
LABEL(ApplyVESAOverrideData, DataSearch);

//     mov  al, byte ptr es:[di + 02h]     ;Flag and 0..7 index
    r.h.al = modeData->FlagsAndFilter;

//     test al, 80h                        ;If top bit set, exit. we're at the "end" (actually start)
//     jne  DataFound                      ;Offset 0x1141
    if ((r.h.al & 0x80) != 0x00)
        goto DataFound;

//     and  ax, 0f0fh                      ;This and should be 0707h
    r.w.ax &= 0x0f0f;
//     DB 3Ah, 0C4h                        ;cmp  al, ah    masm encoding difference
//     jbe  DataFound                      ;Offset 0x1141
    if (r.h.al <= r.h.ah)
        goto DataFound;

//     sub  di, bx                         ;Move backwards
    ++modeData;

//     jmp  DataSearch                     ;Offset 0x112e
    goto DataSearch;

// DataFound:                              ;Offset 0x1141
LABEL(ApplyVESAOverrideData, DataFound);

//     mov  ah, byte ptr es:[di + 03h]     ;Clock config index
    r.h.ah = modeData->ClockConfigIndex;

//     call SetupClocks                    ;Offset 0x31c
    SetupClocks(crtcPort, r.h.ah);

//     mov  cx, 19h                        ;25 CRTC registers
    r.w.cx = 0x0019;

//     xor  al, al                         ;CR0
    r.h.al = 0x00;

//     mov  si, word ptr es:[di]           ;Load CRTC register data pointer
    //crtData

// WriteCRTCRegisters:                     ;Offset 0x1150
LABEL(ApplyVESAOverrideData, WriteCRTCRegisters);

//     mov  ah, byte ptr es:[si]           ;
    r.h.ah = modeData->CRTCRegisters[crtIdx];

//     out  dx, ax                         ;Write CR0 - CR18
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     inc  al
    ++r.h.al;

//     inc  si
    ++crtIdx;

//     loop WriteCRTCRegisters             ;Offset 0x1150
    --r.w.cx;
    if(r.w.cx != 0x0000)
        goto WriteCRTCRegisters;

//     call ConfigureExtraVESAModeSettings ;Offset 0x3d9
    ConfigureExtraVESAModeSettings(mode, r.w.dx, overrideTable, modeData);

// NotVESAMode:                            ;Offset 0x115c
LABEL(ApplyVESAOverrideData, NotVESAMode);

//     mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    r.h.al = Hag::System::BDA::DisplayMode::Get();

//     call EnableOver256KAddressingAndSetAddressWindow;Offset 0x2f6
    EnableOver256KAddressingAndSetAddressWindow(r.h.al, r.w.dx);

//     call SetColorMode                   ;Offset 0xdf6
    SetColorMode(r.h.al);

//     call LockExtendedSequencerRegisters ;Offset 0xf9c
    Hag::S3::Sequencer::UnlockExtendedSequencer::Lock();

//     pop  es
//     pop  cx
//     pop  bx
//     pop  ax
//     ret
}

// ;inputs:
// ;si = pointer to Palette data
// ;outputs:
// ;ah = red
// ;ch = green
// ;cl = blue
// ;si = points to next palette entry
// ;Palette color data is 6 bits. the top two bits are used
// ;as a compression scheme. 
// ;Bits in red:
// ;00 = Load Green
// ;01 = Green = Red
// ;10 = Blue = Green = Red
// ;11 = Load Green, Blue = Red
// ;Bits in Green:
// ;00 = Load Blue
// ;01 = Blue = Green
void DecompressPaletteColor(uint8_t*& paletteData, uint8_t& red, uint8_t& green, uint8_t& blue)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
//     lodsb cs:[si]
    r.h.al = *paletteData;
    ++paletteData;

//     mov   ah, al                        ;Set red
    r.h.ah = r.h.al;

//     and   ah, 03fh                      ;Isolate red color data
    r.h.ah &= 0x3f;

//     and   al, 0c0h                      ;Isolate compression data
    r.h.al &= 0xc0;

//     je    LoadGreen                     ;Offset 0x49f3
    if (r.h.al == 0x00)
        goto LoadGreen;

//     cmp   al, 40h                       ;
//     je    GreenIsRed                    ;Offset 0x4a01
    if (r.h.al == 0x40)
        goto GreenIsRed;

//     cmp   al, 80h
//     je    GreenAndBlueIsRed             ;Offset 0x49ee
    if (r.h.al == 0x80)
        goto GreenAndBlueIsRed;

//     mov   cl, ah                        ;Else C0h - Blue is Red
    r.h.cl = r.h.ah;

//     lodsb cs:[si]
    r.h.al = *paletteData;
    ++paletteData;

//     mov   ch, al                        ;Load Green
    r.h.ch = r.h.al;

//     ret
    red = r.h.ah;
    green = r.h.ch;
    blue = r.h.cl;
    return;

// GreenAndBlueIsRed:                      ;Offset 0x49ee
LABEL(DecompressPaletteColor, GreenAndBlueIsRed);

//     mov   ch, ah
    r.h.ch = r.h.ah;

//     mov   cl, ah
    r.h.cl = r.h.ah;

//     ret
    red = r.h.ah;
    green = r.h.ch;
    blue = r.h.cl;
    return;

// LoadGreen:                              ;Offset 0x49f3
LABEL(DecompressPaletteColor, LoadGreen);

//     lodsb cs:[si]
    r.h.al = *paletteData;
    ++paletteData;

//     mov   ch, al
    r.h.ch = r.h.al;

//     test  al, 40h
//     je    LoadBlue                      ;Offset 0x4a03
    if ((r.h.al & 0x40) == 0x00)
        goto LoadBlue;

//     and   ch, 0bfh                      ;1011 1111
    r.h.ch &= 0xbf;

//     mov   cl, ch                        ;Blue is Green
    r.h.cl = r.h.ch;

//     ret
    red = r.h.ah;
    green = r.h.ch;
    blue = r.h.cl;
    return;

// GreenIsRed:                             ;Offset 0x4a01
LABEL(DecompressPaletteColor, GreenIsRed);

//     mov   ch, ah
    r.h.ch = r.h.ah;

// LoadBlue:                               ;Offset 0x4a03
LABEL(DecompressPaletteColor, LoadBlue);

//     lodsb cs:[si]                       ;Load blue
    r.h.al = *paletteData;
    ++paletteData;

//     mov   cl, al
    r.h.cl = r.h.al;

//     ret
    red = r.h.ah;
    green = r.h.ch;
    blue = r.h.cl;
}

void SetPaletteColorInternal(uint16_t& colorIndex, uint8_t red, uint8_t green, uint8_t blue)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.w.bx = colorIndex;
    r.h.ah = red;
    r.h.ch = green;
    r.h.cl = blue;

//     mov   dx, DACWriteIndex             ;port - 03c8h
    r.w.dx = 0x3c8;

//     mov   al, bl
    r.h.al = r.h.bl;

//     out   dx, al                        ;Write color index
    SYS_WritePortByte(r.w.dx, r.h.al);

//     inc   dx                            ;port - 03c9h - RAMDACData
    ++r.w.dx;

//     mov   al, ah
    r.h.al = r.h.ah;

//     out   dx, al                        ;Write red
    SYS_WritePortByte(r.w.dx, r.h.al);

//     mov   al, ch
    r.h.al = r.h.ch;

//     out   dx, al                        ;Write green
    SYS_WritePortByte(r.w.dx, r.h.al);

//     mov   al, cl
    r.h.al = r.h.cl;

//     out   dx, al                        ;Write blue
    SYS_WritePortByte(r.w.dx, r.h.al);

//     inc   bx
    ++colorIndex;

//     popf
    SYS_RestoreInterrupts();

//     ret
}

// ;inputs:
// ;bl = color index
// ;ah = red
// ;ch = green
// ;cl = blue
// ;outputs:
// ;bx = bx + 1
// ;destroys dx
void SetPaletteColor(uint16_t& colorIndex, uint8_t red, uint8_t green, uint8_t blue)
{
//     pushf
//     cli
    SYS_ClearInterrupts();
    SetPaletteColorInternal(colorIndex, red, green, blue);
}

// ;continue!
// ;inputs:
// ;bx = color index
// ;di = count
// ;si = pointer to Palette data
void ApplyPalette(uint16_t& colorIndex, uint16_t count, uint8_t* paletteData)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

LABEL(ApplyPalette, NextColor);

//     call  DecompressPaletteColor        ;Offset 0x49d4
    DecompressPaletteColor(paletteData, red, green, blue);

//     call  SetPaletteColor               ;Offset 0x47d8
    SetPaletteColor(colorIndex, red, green, blue);

//     dec   di
//     jne   ApplyPalette                  ;Offset 0x498e
    --count;
    if (count != 0x0000)
        goto NextColor;

//     ret
}

uint16_t Data2ee7 = 0x2666;
uint16_t Data2ee9 = 0x4B85;
uint16_t Data2eeb = 0x0E14;

// ;inputs:
// ;ah = red
// ;ch = green
// ;cl = blue
void MakeColorGreyscale(uint8_t& red, uint8_t& green, uint8_t& blue)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.h.ah = red;
    r.h.ch = green;
    r.h.cl = blue;

//     push bx
//     push dx
//     and  ax, 3f00h
    r.w.ax &= 0x3f00;
//     xchg al, ah
    r.h.al = r.h.ah;
    r.h.ah = 0x00;

//     mul  word ptr cs:[Data2ee7]         ;Offset 0x2ee7
//     push dx
//     push ax
    uint32_t v1 = r.w.ax * Data2ee7;
    
//     mov  al, ch
    r.h.al = r.h.ch;

//     and  al, 3fh
    r.h.al &= 0x3f;

//     xor  ah, ah
    r.h.ah = 0x00;
//     mul  word ptr cs:[Data2ee9]         ;Offset 0x2ee9
//     push dx
//     push ax
    uint32_t v2 = r.w.ax * Data2ee9;

//     mov  al, cl
    r.h.al = r.h.cl;

//     and  al, 3fh
    r.h.al &= 0x3f;

//     xor  ah, ah
    r.h.ah = 0x00;

//     mul  word ptr cs:[Data2eeb]         ;Offset 0x2eeb
    uint32_t v3 = r.w.ax * Data2eeb;

//     pop  bx
//     add  ax, bx
//     pop  bx
//     adc  dx, bx
    v3 += v2;

//     pop  bx
//     add  ax, bx
//     pop  bx
//     adc  dx, bx
    v3 += v1;

//     add  ax, ax
//     adc  dx, dx
    v3 <<= 1;

//     add  ax, 8000h
//     adc  dx, 0
    v3 += 0x8000;
    v3 >>= 16;
    v3 &= 0xff;    

//     mov  ah, dl
    red = uint8_t(v3);

//     mov  cl, dl
    blue = uint8_t(v3);

//     mov  ch, dl
    green = uint8_t(v3);

//     pop  dx
//     pop  bx
//     ret;done
}

void MakeColorGreyscaleIfNeeded(uint8_t& red, uint8_t& green, uint8_t& blue)
{
//     test byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0x489, 0x6
//     je   EmptyFunction8                 ;Offset 0x2f38
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x06) != 0x00)
    {
        MakeColorGreyscale(red, green, blue);
    }
}

uint8_t Data43b1[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x95,
    0x55, 0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55,
    0xFF, 0x15, 0x7F, 0x15, 0xBF
};

uint8_t Data43ce[] =
{ 
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

uint8_t Data43de[] =
{
    0x80, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x98,
    0x9C, 0xA0, 0xA4, 0xA8, 0xAD, 0xB2, 0xB8, 0xBF
};

uint8_t Data43ee[] =
{
    0x40, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F,
    0x2F, 0x00, 0x3F, 0xFF, 0x00, 0x3F, 0x00, 0x2F,
    0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x40,
    0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F,
    0x00, 0x7F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F,
    0x00, 0x10, 0x3F, 0x00, 0xC0, 0x3F, 0x00, 0x3F,
    0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00,
    0x7F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00,
    0x10, 0x3F, 0x5F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F,
    0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0xFF, 0x1F, 0x3F,
    0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27,
    0x3F, 0x5F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F,
    0x3F, 0x37, 0x1F, 0x7F, 0x1F, 0x37, 0x3F, 0x1F,
    0x2F, 0x3F, 0x1F, 0x27, 0x3F, 0x1F, 0xDF, 0x3F,
    0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F,
    0x37, 0x1F, 0x7F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F,
    0x3F, 0x1F, 0x27, 0x3F, 0x6D, 0x3F, 0x31, 0x2D,
    0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0xFF,
    0x2D, 0x3F, 0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F,
    0x2D, 0x31, 0x3F, 0x6D, 0x3F, 0x31, 0x2D, 0x3F,
    0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x7F, 0x2D, 0x3A,
    0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D,
    0xED, 0x3F, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36,
    0x2D, 0x3F, 0x3A, 0x2D, 0x7F, 0x2D, 0x3A, 0x3F,
    0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x40, 0x1C,
    0x07, 0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00,
    0x1C, 0xDC, 0x00, 0x1C, 0x00, 0x15, 0x1C, 0x00,
    0x0E, 0x1C, 0x00, 0x07, 0x1C, 0x40, 0x1C, 0x07,
    0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x5C,
    0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07,
    0x1C, 0x00, 0xC0, 0x1C, 0x00, 0x1C, 0x07, 0x00,
    0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x5C, 0x00,
    0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C,
    0x4E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C,
    0x18, 0x0E, 0x1C, 0xDC, 0x0E, 0x1C, 0x0E, 0x18,
    0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x4E,
    0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18,
    0x0E, 0x5C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C,
    0x0E, 0x11, 0x1C, 0x0E, 0xCE, 0x1C, 0x0E, 0x1C,
    0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E,
    0x5C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E,
    0x11, 0x1C, 0x54, 0x1C, 0x16, 0x14, 0x1C, 0x18,
    0x14, 0x1C, 0x1A, 0x14, 0x1C, 0xDC, 0x14, 0x1C,
    0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16,
    0x1C, 0x54, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14,
    0x1C, 0x1A, 0x14, 0x5C, 0x14, 0x1A, 0x1C, 0x14,
    0x18, 0x1C, 0x14, 0x16, 0x1C, 0x14, 0xD4, 0x1C,
    0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 0x1C,
    0x1A, 0x14, 0x5C, 0x14, 0x1A, 0x1C, 0x14, 0x18,
    0x1C, 0x14, 0x16, 0x1C, 0x40, 0x10, 0x04, 0x00,
    0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0xD0,
    0x00, 0x10, 0x00, 0x0C, 0x10, 0x00, 0x08, 0x10,
    0x00, 0x04, 0x10, 0x40, 0x10, 0x04, 0x00, 0x10,
    0x08, 0x00, 0x10, 0x0C, 0x00, 0x50, 0x00, 0x0C,
    0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00,
    0xC0, 0x10, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08,
    0x00, 0x10, 0x0C, 0x00, 0x50, 0x00, 0x0C, 0x10,
    0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x48, 0x10,
    0x0A, 0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08,
    0x10, 0xD0, 0x08, 0x10, 0x08, 0x0E, 0x10, 0x08,
    0x0C, 0x10, 0x08, 0x0A, 0x10, 0x48, 0x10, 0x0A,
    0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x50,
    0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A,
    0x10, 0x08, 0xC8, 0x10, 0x08, 0x10, 0x0A, 0x08,
    0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x50, 0x08,
    0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10,
    0x4B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10,
    0x0F, 0x0B, 0x10, 0xD0, 0x0B, 0x10, 0x0B, 0x0F,
    0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x4B,
    0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F,
    0x0B, 0x50, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10,
    0x0B, 0x0C, 0x10, 0x0B, 0xCB, 0x10, 0x0B, 0x10,
    0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B,
    0x50, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B,
    0x0C, 0x10
};

uint8_t Data4371[] =
{
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF
};

uint8_t Data41e9[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x80,
    0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A, 0x40,
    0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF, 0x95, 0x55, 0x3F, 0xD5,
    0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF, 0x15, 0x7F,
    0x15, 0xBF, 0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00,
    0x6A, 0x2A, 0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00,
    0xAA, 0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A,
    0x2A, 0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA,
    0x95, 0x55, 0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F,
    0x55, 0xFF, 0x15, 0x7F, 0x15, 0xBF, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF
};

uint8_t Data425d[] =
{
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

uint8_t Data429d[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x6A, 0x00, 0xAA, 0x40, 0x15,
    0x40, 0x3F, 0x00, 0x2A, 0x15, 0x00, 0x2A, 0x3F,
    0x2A, 0x00, 0x15, 0x2A, 0x00, 0x3F, 0x6A, 0x15,
    0x6A, 0x3F, 0xC0, 0x15, 0x00, 0x15, 0x2A, 0xC0,
    0x3F, 0x00, 0x3F, 0x2A, 0x2A, 0x15, 0x00, 0xEA,
    0x15, 0x2A, 0x3F, 0x00, 0xEA, 0x3F, 0x00, 0x55,
    0x00, 0x15, 0x3F, 0x00, 0x3F, 0x15, 0x00, 0x7F,
    0x2A, 0x55, 0x2A, 0x15, 0x3F, 0x2A, 0x3F, 0x15,
    0x2A, 0x7F, 0x15, 0x40, 0x15, 0x00, 0x2A, 0x15,
    0x2A, 0x00, 0x15, 0x6A, 0x3F, 0x40, 0x3F, 0x00,
    0x2A, 0x3F, 0x2A, 0x00, 0x3F, 0x6A, 0xD5, 0x00,
    0x15, 0x00, 0x3F, 0xD5, 0x2A, 0x15, 0x2A, 0x3F,
    0x3F, 0x00, 0x15, 0xFF, 0x00, 0x3F, 0x2A, 0x15,
    0xFF, 0x2A, 0x55, 0x00, 0x55, 0x2A, 0x15, 0x3F,
    0x00, 0x15, 0x3F, 0x2A, 0x3F, 0x15, 0x00, 0x3F,
    0x15, 0x2A, 0x7F, 0x00, 0x7F, 0x2A, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF
};

uint8_t Data4331[] =
{
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0xA5, 0xA8,
    0x82, 0x87, 0x9B, 0xA0, 0x8F, 0x94, 0xA8, 0xAC,
    0x8C, 0x91, 0xA5, 0xAA, 0x94, 0x9E, 0xB2, 0xB6,
    0x8F, 0x93, 0xA7, 0xAC, 0x9B, 0xA0, 0xB4, 0xB9,
    0x86, 0x8B, 0x9F, 0xA4, 0x93, 0x98, 0xAC, 0xB0,
    0x89, 0x8D, 0xA1, 0xA6, 0x95, 0x9A, 0xAE, 0xB3,
    0x93, 0x97, 0xAB, 0xB0, 0x9F, 0xA4, 0xB8, 0xBD,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

void Set248ColorPalette(uint16_t& colorIndex)
{
   REGPACK r;
    memset(&r, 0, sizeof(r));
    uint8_t* palettePtr = NULL;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
 
 //     mov   di, 0010h                     ;Count 16
    r.w.di = 0x0010;

//     mov   si, offset Data43b1           ;Offset 0x43b1
    palettePtr = Data43b1;

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0489h, 0x06
//     je    Label0x49a8                   ;Offset 0x49a8
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x06) == 0x00)
        goto Label0x49a8;

//     mov   si, offset Data43ce           ;Offset 0x43ce
    palettePtr = Data43ce;

// Label0x49a8:                            ;Offset 0x49a8
LABEL(Set248ColorPalette, Label0x49a8);

//     call  ApplyPalette                  ;Offset 0x498e
    ApplyPalette(colorIndex, r.w.di, palettePtr);

//     mov   di, 0010h                     ;count 16
    r.w.di = 0x10;

//     mov   bx, 0010h                     ;color index 16
    colorIndex = 0x10;

//     mov   si, offset Data43de           ;Offset 0x43de
    palettePtr = Data43de;

//     call  ApplyPalette                  ;Offset 0x498e
    ApplyPalette(colorIndex, r.w.di, palettePtr);

//     mov   di, 00d8h                     ;count 216
    r.w.di = 0x00d8;

//     mov   bx, 0020h                     ;color index 32
    colorIndex = 0x20;

//     mov   si, offset Data43ee           ;Offset 0x43ee
    palettePtr = Data43ee;

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0489h, 0x06
//     je    ApplyPalette                  ;Offset 0x498e
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x06) == 0x00)
    {
        ApplyPalette(colorIndex, r.w.di, palettePtr);
        return;
    }

// ApplyGreyscalePalette:                  ;Offset 0x49c7
LABEL(Set248ColorPalette, ApplyGreyscalePalette);

//     call  DecompressPaletteColor        ;Offset 0x49d4
    DecompressPaletteColor(palettePtr, red, green, blue);

//     call  MakeColorGreyscaleIfNeeded    ;Offset 0x2eed
    MakeColorGreyscaleIfNeeded(red, green, blue);

//     call  SetPaletteColor               ;Offset 0x47d8
    SetPaletteColor(colorIndex, red, green, blue);

//     dec   di
    --r.w.di;

//     jne   ApplyGreyscalePalette         ;Offset 0x49c7
    if (r.w.di != 0x00)
        goto ApplyGreyscalePalette;

//     ret
}

void SetPalette()
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    uint8_t* palettePtr = NULL;
    uint8_t flags = 0;

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_PaletteLoadingEnabled;Offset 0489h, 0x08
//     je    PaletteLoadingEnabled         ;Offset 0x4911
//     ret
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x08) != 0x00)
        return;
        
// PaletteLoadingEnabled:                  ;Offset 0x4911
//     mov   dx, DACMask                   ;port - 0x3c6
    r.w.dx = 0x3c6;

//     in    al, dx
    r.h.al = SYS_ReadPortByte(r.w.dx);

//     inc   al                            ;If DAC mask was 0xff, jump
    ++r.h.al;

//     je    IsFF                          ;Offset 0x491c
    if (r.h.al == 0x00)
        goto IsFF;

//     mov   al, 0ffh
    r.h.al = 0xFF;

//     out   dx, al                        ;set to 0xff
    SYS_WritePortByte(r.w.dx, r.h.al);

// IsFF:                                   ;Offset 0x491c
LABEL(SetPalette, IsFF);

//     mov   di, 0040h                     ;count 64
    r.w.di = 0x0040;

//     xor   bx, bx
    r.w.bx = 0x0000;

//     mov   al, byte ptr ds:[BDA_DisplayMode];Offset 0449h
    r.h.al = Hag::System::BDA::DisplayMode::Get();

//     mov   si, offset Data4371           ;Offset 0x4371
    palettePtr = Data4371;

//     cmp   al, BDA_DM_80x25_Monochrome_Text;0x07
//     je    ApplyPalette                  ;Offset 0x498e
    if (r.h.al == 0x07)
    {
        ApplyPalette(r.w.bx, r.w.di, palettePtr);
        return;
    }

//     cmp   al, BDA_DM_640x350_Monochrome_Graphics;0x0f
//     je    ApplyPalette                  ;Offset 0x498e
    if (r.h.al == 0x0f)
    {
        ApplyPalette(r.w.bx, r.w.di, palettePtr);
        return;
    }

//     cmp   al, BDA_DM_320x200_256_Color_Graphics;0x13
//     je    Set248ColorPalette            ;Offset 0x4998
    if (r.h.al == 0x13)
    {
        Set248ColorPalette(r.w.bx);
        return;
    }

//     jb    LegacyMode                    ;Offset 0x4948
    if (r.h.al < 0x13)
        goto LegacyMode;

//     mov   ah, al
    r.h.ah = r.h.al;

//     call  GetVideoModeFlags             ;Offset 0x105d
    Hag::S3::TrioBase::GetVideoModeFlags(r.h.al, flags);

//     xchg  ah, al
    r.h.al = r.h.ah;
    r.h.ah = flags;

//     test  ah, 02h
//     je    ApplyPalette                  ;Offset 0x498e
    if ((r.h.ah & 0x02) == 0x00)
    {
        ApplyPalette(r.w.bx, r.w.di, palettePtr);
        return;
    }

//     test  ah, 04h
//     je    Label0x4981                   ;Offset 0x4981
    if ((r.h.ah & 0x04) == 0x00)
        goto Label0x4981;

//     jmp   Set248ColorPalette            ;Offset 0x4998
    Set248ColorPalette(r.w.bx);
    return;

// LegacyMode:                             ;Offset 0x4948
LABEL(SetPalette, LegacyMode);

//     cmp   al, BDA_DM_320x200_4_Color_Graphics1;0x04
//     jb    Label0x495a                   ;Offset 0x495a
    if (r.h.al < 0x04)
        goto Label0x495a;

//     cmp   al, BDA_DM_640x200_BW_Graphics;0x06
//     jbe   Label0x4972                   ;Offset 0x4972
    if (r.h.al <= 0x06)
        goto Label0x4972;

//     cmp   al, BDA_DM_Unknown1           ;0x08
//     je    Label0x4981                   ;Offset 0x4981
    if (r.h.al == 0x08)
        goto Label0x4981;
        
//     cmp   al, BDA_DM_640x200_16_Color_Graphics;0xe
//     jbe   Label0x4972                   ;Offset 0x4972
    if (r.h.al <= 0x0e)
        goto Label0x4972;

//     jmp   Label0x4981                   ;Offset 0x4981
    goto Label0x4981;

// Label0x495a:                            ;Offset 0x495a
LABEL(SetPalette, Label0x495a);

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_LineMode400;Offset 0489h, 0x10
//     jne   Label0x4981                   ;Offset 0x4981
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x10) != 0x00)
        goto Label0x4981;

//     mov   ah, byte ptr ds:[BDA_EGAFeatureBitSwitches];Offset 0448h
    r.h.ah = Hag::System::BDA::EGAFeatureBitSwitches::Get();

//     and   ah, 0fh
    r.h.ah &= 0x0f;

//     cmp   ah, 03h
//     je    Label0x4981                   ;Offset 0x4981
    if (r.h.ah == 0x03)
        goto Label0x4981;

//     cmp   ah, 09h
//     je    Label0x4981                   ;Offset 0x4981
    if (r.h.ah == 0x09)
        goto Label0x4981;

// Label0x4972:                            ;Offset 0x4972
LABEL(SetPalette, Label0x4972);

//     mov   si, offset Data41e9           ;Offset 0x41e9
    palettePtr = Data41e9;

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0489h, 0x06
//     je    ApplyPalette                  ;Offset 0x498e
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x06) == 0x00)
    {
        ApplyPalette(r.w.bx, r.w.di, palettePtr);
        return;
    }

//     mov   si, offset Data425d           ;Offset 0x425d
    palettePtr = Data425d;

//     jmp   ApplyPalette                  ;Offset 0x498e
    ApplyPalette(r.w.bx, r.w.di, palettePtr);
    return;

// Label0x4981:                            ;Offset 0x4981
LABEL(SetPalette, Label0x4981);

//     mov   si, offset Data429d           ;Offset 0x429d
    palettePtr = Data429d;

//     test  byte ptr ds:[BDA_VideoDisplayDataArea], BDA_VDDA_GrayScale OR BDA_VDDA_MonochromeMonitor;Offset 0489h, 0x06
//     je    ApplyPalette                  ;Offset 0x498e
    if ((Hag::System::BDA::VideoDisplayDataArea::Get() & 0x06) == 0x00)
    {
        ApplyPalette(r.w.bx, r.w.di, palettePtr);
        return;
    }

//     mov   si, offset Data4331           ;Offset 0x4331
    palettePtr = Data4331;

//     jmp  ApplyPalette
    ApplyPalette(r.w.bx, r.w.di, palettePtr);
}

// ;inputs:
// ;al = ?
// ;bl = character generator ram bank
void Func0x227(uint8_t function, uint8_t& ramBank)
{
//     push ax
//     cmp  al, 55h
//     jne  Label0x22f                     ;Offset 0x22f
//     and  bl, 3fh                        ;if al == 55h, and off the top 2 bits from bl
    if (function == 0x55)
        ramBank &= 0x3f;
// Label0x22f:                             ;Offset 0x22f
//     pop  ax
}

// ;inputs:
// ;cx = number of lines
// ;bh = bytes per line
// ;dx bytes to skip in destination
// ;ds:si = source
// ;es:di = destination
// ;preserves ax
bool CopyRect(uint8_t* src, uint8_t* dst, uint16_t lines, uint8_t bytesPerLine, uint16_t skipCount)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
//     push      ax                        ;store ax
// LineLoop:
LABEL(CopyRect, LineLoop);

//     push      cx                        ;Preserve line count
//     mov       cl, bh                    ;set byte count
//     xor       ch, ch                    ;zero out top of count
    r.w.cx = bytesPerLine;

//     rep movsb                           ;move cl bytes
    while (r.w.cx != 0x0000)
    {
        *dst = *src;
        ++src;
        ++dst;
        --r.w.cx;
    }

//     pop       cx                        ;restore line count
//     add       di, dx                    ;add bytes to skip in destination
    dst += skipCount;

//     loop      LineLoop                  ;Offset 0xeaa - loop cx times
    --lines;
    if (lines != 0x0000)
        goto LineLoop;

//     or        al, 0ffh                  ;Set flags?
//     pop       ax                        ;restore ax
//     ret
    return true;
}

uint8_t Patch8x14[] =
{
    0x1D, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x00, 0x63, 0x63, 0x63, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2B, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
    0x2D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4D, 0x00, 0x00, 0xC3, 0xE7, 0xFF, 0xDB, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x00,
    0x54, 0x00, 0x00, 0xFF, 0xDB, 0x99, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
    0x57, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x66, 0x00, 0x00, 0x00,
    0x58, 0x00, 0x00, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0xC3, 0xC3, 0x00, 0x00, 0x00,
    0x5A, 0x00, 0x00, 0xFF, 0xC3, 0x86, 0x0C, 0x18, 0x30, 0x61, 0xC3, 0xFF, 0x00, 0x00, 0x00,
    0x5B, 0x00, 0x00, 0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00, 0x00, 0x00,
    0x5D, 0x00, 0x00, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00, 0x00, 0x00,
    0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0xFF, 0xDB, 0xDB, 0xDB, 0xDB, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x00, 0x00, 0x00,
    0x91, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x1B, 0x7E, 0xD8, 0xDC, 0x77, 0x00, 0x00, 0x00,
    0x9D, 0x00, 0x00, 0xC3, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x9E, 0x00, 0xFC, 0x66, 0x66, 0x7C, 0x62, 0x66, 0x6F, 0x66, 0x66, 0xF3, 0x00, 0x00, 0x00,
    0xF1, 0x00, 0x00, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18, 0x00, 0xFF, 0x00, 0x00, 0x00,
    0xF6, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
    0x00, 0x00
};

uint8_t Patch8x16[] =
{
    0x1D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x22, 0x00, 0x63, 0x63, 0x63, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4D, 0x00, 0x00, 0xC3, 0xE7, 0xFF, 0xDB, 0xDB, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x00, 0x00,
    0x54, 0x00, 0x00, 0xFF, 0xDB, 0x99, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00, 0x00,
    0x57, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00,
    0x5A, 0x00, 0x00, 0xFF, 0xC3, 0x83, 0x06, 0x0C, 0x18, 0x30, 0x61, 0xC3, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0x5B, 0x00, 0x00, 0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00, 0x00, 0x00, 0x00,
    0x5D, 0x00, 0x00, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00, 0x00, 0x00, 0x00,
    0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0xFF, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0x00, 0x00, 0x00, 0x00,
    0x77, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x00, 0x00, 0x00, 0x00,
    0x91, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x3B, 0x1B, 0x7E, 0xD8, 0xDC, 0x77, 0x00, 0x00, 0x00, 0x00,
    0x9D, 0x00, 0x00, 0xC3, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
    0x9E, 0x00, 0xFC, 0x66, 0x66, 0x7C, 0x62, 0x66, 0x6F, 0x66, 0x66, 0x66, 0xF3, 0x00, 0x00, 0x00, 0x00,
    0xF1, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00,
    0xF6, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0xFF, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00
};

// ;inputs:
// ;bl = flags 0x80 = 8 lines, 0x40 = 7 lines
// ;
void PatchCharacterSet(uint8_t flags)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    FARPointer dst(0xA000, 0x0000);
    uint8_t* src = NULL;

//     push      ax
//     push      cx
//     push      di
//     push      si
//     push      es
//     mov       cx, 0a000h
//     mov       es, cx                    ;es points to video segment
//     test      bl, 0c0h                  ;bl & 0c0h == 0 -> exit
//     je        Exit                      ;Offset 0x4797
    if ((flags & 0xC0) == 0x00)
        return;
        
//     mov       cx, 0007h                 ;8x14
    r.w.cx = 0x0007;

//     mov       si, offset Patch8x14      ;Offset 0x6d20
    src = Patch8x14;

//     test      bl, 80h                   ;bit 7 set -> Start copy of 8x14 patch
//     jne       NextCharacter             ;Offset 0x477f
    if ((r.h.bl & 0x80) != 0x00)
        goto NextCharacter;

//     mov       si, offset Patch8x16      ;Offset 0x7e30
    src = Patch8x16;

//     mov       cl, 08h                   ;8x16
    r.h.cl = 0x08;

// NextCharacter:                          ;Offset 0x477f
LABEL(PatchCharacterSet, NextCharacter);

//     mov       ah, byte ptr cs:[si]      ;read byte
    r.h.ah = *src;

//     inc       si                        ;point to next one
    ++src;

//     or        ah, ah                    ;is ah zero? -> Exit
//     je        Exit                      ;Offset 0x4797
    if (r.h.ah == 0x00)
        return;

//     xor       al, al                    ;empty al
    r.h.al = 0x00;

//     shr       ax, 03h                   ;ax = ax / 8
    r.w.ax >>= 3;

//     mov       di, ax                    ;new destination
    r.w.di = r.w.ax;

//     push      cx                        ;store count
//     cli                                 ;clear interrupts
    SYS_ClearInterrupts();

//     rep movsw es:[di], cs:[si]          ;move cx words
    dst.Offset = r.w.di;
    uint8_t* dstPtr = dst.ToPointer<uint8_t>(r.w.cx << 1);
    for (int i = 0; i < r.w.cx; ++i)
    {
        *dstPtr = *src;
        ++dstPtr;
        ++src;
        *dstPtr = *src;
        ++dstPtr;
        ++src;
    }
//     sti                                 ;restore interrupts
    SYS_RestoreInterrupts();
//     pop       cx                        ;restore count
//     jmp       NextCharacter             ;Offset 0x477f
    goto NextCharacter;
// Exit:                                   ;Offset 0x4797
//     pop       es
//     pop       si
//     pop       di
//     pop       cx
//     pop       ax
//     ret       
}

void EnablePaletteBasedVideo()
{
    REGPACK r;
    memset(&r, 0, sizeof(r));

//     push      dx                        ;preserve dx
//     mov       dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0463h = port - 0x3?4
    r.w.dx = Hag::System::BDA::VideoBaseIOPort::Get();

//     add       dl, 06h                   ;port - 0x3?A - Input Status 1
    r.h.dl += 0x06;

//     in        al, dx                    ;
    r.h.al = SYS_ReadPortByte(r.w.dx);

//     mov       dx, 03c0h                 ;port 0x3c0 - Attribute Controller Index
    r.w.dx = 0x3c0;

//     mov       al, 20h                   ;Enable Video Display = 1, Display video using the palette registers enabled (normal display operation).
    r.h.al = 0x20;

//     out       dx, al
    SYS_WritePortByte(r.w.dx, r.h.al);

//     pop       dx                        ;restore dx
//     ret       
}

uint16_t RamBankOffset[] =
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

// ;inputs:
// ;es:si pointer to character font
// ;dx = offset into ?
// ;cx = Number of characters
// ;bh = character height
// ;bl = character generator ram bank
// ;outputs:
// ;nothing. all registers preserved
void SetTextFontAndAddressing(uint8_t* font, uint16_t someOffset, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    FARPointer dest(0xA000, 0x0000);
    r.w.dx = someOffset;
    r.w.cx = numCharacters;
    r.h.bh = charHeight;
    r.h.bl = ramBank;

//     push      ax
//     push      bx
//     push      cx
//     push      dx
//     push      di
//     push      si
//     push      ds
//     push      es
//     call      PrepareAttributeController;Offset 0x47ae
    PrepareAttributeController();
    
//     push      dx                        ;Store dx //Some offset
//     mov       dx, SequenceIndex         ;port - 0x3c4 - Sequence Index register
    r.w.dx = 0x3c4;

//     mov       ax, 0402h                 ;SR2 - Enable Write Plane Register
    r.w.ax = 0x0402;

//     out       dx, ax                    ;bit 2 = 1 - Enable plane 3 (counting from 1)
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       ax, 0704h                 ;SR4 - Memory Mode Control register
    r.w.ax = 0x0704;

//     out       dx, ax                    ;bit 0 = 1 - Unknown, bit 1 = 1 - 256kb memory access, bit 2 = 1 - Sequential Addressing Mode
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       dx, GraphicsControllerIndex;port - 0x3ce - Graphics Controller Index register
    r.w.dx = 0x3ce;

//     mov       ax, 0204h                 ;GR4 - Read Plane Select Register
    r.w.ax = 0x0204;

//     out       dx, ax                    ;bit 1 = 1 - Enable plane 2 (counting from 1) 
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       ax, 0005h                 ;GR5 - Graphics Controller Mode register
    r.w.ax = 0x0005;

//     out       dx, ax                    ;bit 1-0 = 00 - Write Mode 0. Each of four video memory planes is written with the CPU data
//                                         ;               rotated by the number of counts in the rotate register. If the Set/Reset
//                                         ;               register is enabled for any of four planes, the corresponding plane is written
//                                         ;               with the data stored in the set/reset register. Raster operations and bit mask 
//                                         ;               registers are effective
//                                         ;bit 3 = 0 - The CPU reads data from the video memory planes. The plane is selected by the
//                                         ;            Read Plane Select register. This is called read mode 0
//                                         ;bit 4 = 0 - Standard addressing.
//                                         ;bit 5 = 0 - Normal shift mode
//                                         ;bit 6 = 0 - Bit 5 in this register controls operation ofthe video shift registers
//                                         ;
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       ax, 0406h                 ;GR6 - Memory Map Mode Control register
    r.w.ax = 0x0406;

//     out       dx, ax                    ;bit 0 = 0 - Text mode display addressing selected
//                                         ;bit 1 = 0 - AO address bit unchanged
//                                         ;bit 3-2 = 01 - AOOOOH to AFFFFH (64 KBytes)
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     pop       dx                        ;Restore dx
    r.w.dx = someOffset;

//     mov       ax, es
//     mov       ds, ax
//     mov       ax, 0a000h                ;Video memory
//     mov       es, ax
//     push      bx                        ;store bh = character height, bl = character generator ram bank
    uint16_t bxsave = r.w.bx;

//     and       bl, 07h                   ;0x00           ;0x01           ;0x02           ;0x03           ;0x04           ;0x05           ;0x06           ;0x07
    r.h.bl &= 0x07;

//     ror       bl, 01h                   ;0x00 - C = 0   ;0x80 - C = 1   ;0x01 - C = 0   ;0x81 - C = 1   ;0x02 - C = 0   ;0x82 - C = 1   ;0x03 - C = 0   ;0x83 - C = 1
//     ror       bl, 01h                   ;0x00 - C = 0   ;0x40 - C = 0   ;0x80 - C = 1   ;0xC0 - C = 1   ;0x01 - C = 0   ;0x41 - C = 0   ;0x81 - C = 1   ;0xC1 - C = 1
//     rcr       bl, 01h                   ;0x00 - C = 0   ;0x20 - C = 0   ;0x40 - C = 0   ;0x60 - C = 0   ;0x00 - C = 1   ;0x20 - C = 1   ;0xD0 - C = 1   ;0xE0 - C = 1
//     jae       NoCarry                   ;Offset 0x4701 - same as jnc = jump no carry
//     add       bl, 10h                   ;0x10           ;0x30           ;0x50           ;0x70           ;               ;               ;               ;
// NoCarry:                                ;Offset 0x4701
//     shl       bl, 01h                   ;0x20           ;0x60           ;0xA0           ;0xE0           ;0x00           ;0x40           ;0x80           ;0xC0
//     mov       ah, bl
//     mov       al, 00h
//     mov       di, ax                    ;di = 0x0000(4), 0x2000(0), 0x4000(5), 0x6000(1), 0x8000(6), 0xA000(2), 0xC000(7), 0xE000(3)
    r.w.di = RamBankOffset[r.h.bl]; //Yeah, I'm not doing that funky math up there. Also, my math was apparently wrong. darn ror's.

//     or        dx, dx
//     je        NoOffset                  ;Offset 0x4714
    if (r.w.dx != 0x0000)
    {
//     mov       ax, 20h
//     mul       dx
//     add       di, ax
    r.w.di += r.w.dx << 5;
    }
    
// NoOffset:                               ;Offset 0x4714
    dest.Offset = r.w.di;

//     mov       dx, 20h
    r.w.dx = 0x0020;

//     sub       dl, bh                    ;32 - character height
    r.h.dl -= r.h.bh;

//     jcxz      CopyDone                  ;Offset 0x472c - if no characters, bail
    if (r.w.cx != 0x0000)
    {
//     call      CopyRect                  ;Offset 0xea9
//     jne       CopyDone                  ;Offset 0x472c
        if (!CopyRect(font, dest.ToPointer<uint8_t>(0x2000), r.w.cx, r.h.bh, r.w.dx))//Always true.
        {
// Label0x4720:                            ;Offset 0x4720
//     push      cx
//     mov       cl, bh
//     mov       ch, 0
//     rep movsb
//     add       di, dx
//     pop       cx
//     loop      Label0x4720               ;Offset 0x4720
        }
// CopyDone:                               ;Offset 0x472c
    }

//     pop       bx                        ;restore bh = character height, bl = character generator ram bank
    r.w.bx = bxsave;

//     pop       es                        ;
//     pop       ds                        ;
//     call      PatchCharacterSet         ;Offset 0x4760
    PatchCharacterSet(r.h.bl);

//     mov       dx, SequenceIndex         ;port - 0x3c4
    r.w.dx = 0x3c4;

//     mov       ax, 0302h                 ;SR2 - Enable Write Plane register
//                                         ;bits 3-0 = 0011 - Enable writing to plane 0 and 1
    r.w.ax = 0x0302;
    
//     out       dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);
//     mov       ax, 0304h                 ;SR4 - Memory Mode Control register
//                                         ;bit 0 = 1 - Unknown
//                                         ;bit 1 = 1 - Allows complete memory access to 256KiB
//                                         ;bit 2 = 0 - Sequential Addressing Mode.
//                                         ;            Enables the odd/even addressing mode.
//                                         ;            Even addresses access planes 0 and 2.
//                                         ;            Odd addresses access planes 1 and 3.
    r.w.ax = 0x0304;

//     out       dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       dx, GraphicsControllerIndex;port - 0x3ce
    r.w.dx = 0x3ce;

//     mov       ax, 0004h                 ;GR4 - Read Plane Select Register
//                                         ;bits 1-0 = 00 - Plane 0
    r.w.ax = 0x0004;

//     out       dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       ax, 1005h                 ;GR5 - Graphics Controller Mode register
//                                         ;bit 4 = 1 - Odd/Even addressing mode. Even CPU addresses access plane 0 and 2,
//                                         ;            while odd CPU addresses access plane 1 and 3. This option is useful
//                                         ;            for emulating the CGA compatible mode. The value of this bit should be
//                                         ;            the inverted value programmed in bit 2 of the Sequencer Memory Mode register
//                                         ;            SR4. This bit affects reading of display memory by the CPU.
    r.w.ax = 0x1005;

//     out       dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     mov       ax, 0a06h                 ;GR6 - Memory Map Mode Control register
//                                         ;bit 0 = 0 - Text mode display addressing selected
//                                         ;bit 1 = 1 - Chain odd / even planes
//                                         ;bits 3-2 = 10 - B0000 - B7FFF (32KiB)
    r.w.ax = 0x0a06;

//     cmp       word ptr ds:[BDA_VideoBaseIOPort], 03b4h;Offset 0463h
//     je        Label0x4755               ;Offset 0x4755
    if (Hag::System::BDA::VideoBaseIOPort::Get() != 0x03b4)
    {

//     mov       ah, 0eh                   ;GR6 - Memory Map Mode Control register
//                                         ;bit 0 = 0 - Text mode display addressing selected
//                                         ;bit 1 = 1 - Chain odd / even planes
//                                         ;bits 3-2 = 11 - B8000 - BFFFF (32KiB)
        r.h.ah = 0x0e;
// Label0x4755:                            ;Offset 0x4755
    }

//     out       dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);

//     call      EnablePaletteBasedVideo   ;Offset 0x479d
    EnablePaletteBasedVideo();
    
//     pop       si
//     pop       di
//     pop       dx
//     pop       cx
//     pop       bx
//     pop       ax
//     ret
}

// ;inputs:
// ;bh = point height of character
// ;
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

// ;inputs:
// ;bl = character generator ram bank
void Set8x14TextFontAndAddressing(uint8_t ramBank)
{
//     mov  si, offset Characters8x14      ;Offset 0x5f20
//     mov  ax, cs
//     mov  es, ax
//     xor  dx, dx
//     mov  cx, 0100h
//     mov  bh, 0eh
//     jmp  SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x14, 0x0000, 0x0100, 0x0e, ramBank);
}

// ;inputs:
// ;bl = character generator ram bank
void Set8x8TextFontAndAddressing(uint8_t ramBank)
{
//     mov  si, offset LowerCharacters8x8  ;Offset 0x5720
//     mov  ax, cs
//     mov  es, ax
//     xor  dx, dx
//     mov  cx, 100h
//     mov  bh, 08h
//     jmp  SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x8, 0x0000, 0x0100, 0x08, ramBank);
}

// ;inputs:
// ;bl = character generator ram bank
void SelectCharacterFont(uint8_t ramBank)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));

//     mov  al, 03h                        ;SR3 - Character Font Select register
    r.h.al = 0x03;

//     mov  ah, bl
    r.h.ah = ramBank;

//     mov  dx, SequenceIndex              ;port - 0x3c4
    r.w.dx = 0x3c4;

//     out  dx, ax
    SYS_WritePortShort(r.w.dx, r.w.ax);
//     ret  
}


// ;inputs:
// ;bl = character generator ram bank
void Set8x16TextFontAndAddressing(uint8_t ramBank)
{
    // mov  si, offset Characters8x16      ;Offset 0x6e30
    // mov  ax, cs
    // mov  es, ax
    // xor  dx, dx
    // mov  cx, 100h
    // mov  bh, 10h
    // jmp  SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAndAddressing(Hag::S3::TrioBase::m_Characters8x16, 0x0000, 0x0100, 0x10, ramBank);
}

void SetTextFontAddressingAndCursor(uint8_t* font, uint16_t someOffset, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank)
{
    // call SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAndAddressing(font, someOffset, numCharacters, charHeight, ramBank);

    // jmp  ConfigureCursorPropertiesAndVerticalDisplayEnd;Offset 0x3010
    ConfigureCursorPropertiesAndVerticalDisplayEnd(charHeight);

    // nop
}

void Set8x14TextFontAddressingAndCursor(uint8_t ramBank)
 {
    // mov  si, offset Characters8x14      ;Offset 0x5f20
    // mov  ax, cs
    // mov  es, ax
    // xor  dx, dx
    // mov  cx, 100h
    // mov  bh, 0eh
    // call SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAddressingAndCursor(Hag::S3::TrioBase::m_Characters8x14, 0x0000, 0x0100, 0x0E, ramBank);
    
    // jmp  ConfigureCursorPropertiesAndVerticalDisplayEnd;Offset 0x3010
    ConfigureCursorPropertiesAndVerticalDisplayEnd(0x0E);

    // nop
}

void Set8x8TextFontAddressingAndCursor(uint8_t ramBank)
 {
    // mov  si, offset LowerCharacters8x8  ;Offset 0x5720
    // mov  ax, cs
    // mov  es, ax
    // xor  dx, dx
    // mov  cx, 100h
    // mov  bh, 08h
    // call SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAddressingAndCursor(Hag::S3::TrioBase::m_Characters8x8, 0x0000, 0x0100, 0x08, ramBank);

    // jmp  ConfigureCursorPropertiesAndVerticalDisplayEnd;Offset 0x3010
    ConfigureCursorPropertiesAndVerticalDisplayEnd(0x08);

    // nop
}

void Set8x16TextFontAddressingAndCursor(uint8_t ramBank)
{
    // mov  si, offset Characters8x16      ;Offset 0x6e30
    // mov  ax, cs
    // mov  es, ax
    // xor  dx, dx
    // mov  cx, 100h
    // mov  bh, 10h
    // call SetTextFontAndAddressing       ;Offset 0x46c2
    SetTextFontAddressingAndCursor(Hag::S3::TrioBase::m_Characters8x16, 0x0000, 0x0100, 0x10, ramBank);

    // jmp  ConfigureCursorPropertiesAndVerticalDisplayEnd;Offset 0x3010
    ConfigureCursorPropertiesAndVerticalDisplayEnd(0x10);

    // nop  
}

uint8_t NumberOfRows[] = { 0x0D, 0x18, 0x2A };


// ;inputs:
// ;bl = row specifier (00 = user set)
// ;dl = row count index = 01 = 14, 02 = 25, 03 = 43 or if bl == 00, any number (that's valid)
// ;cx = character height
void SetUserFont(uint8_t* font, uint8_t rowSpecifier, uint8_t rowCountIndex, uint16_t charHeight)
{
    printf("SetUserFont - Not supported!\n"); //Let's hope the mode set doesn't use this.

//     cli
//     mov  word ptr es:[010ch], si        ;Offset 0x10c - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
//     mov  word ptr es:[010eh], di        ;Offset 0x10e
//     sti
//     cmp  bl, 04h
//     jb   Label0x313f                    ;Offset 0x313f
//     mov  bl, 02h
// Label0x313f:                            ;Offset 0x313f
//     dec  dl
//     or   bl, bl
//     je   Label0x314e                    ;Offset 0x314e
//     dec  bl
//     mov  bh, 00h
//     mov  dl, byte ptr cs:[bx + offset NumberOfRows]
// Label0x314e:                            ;Offset 0x314e
//     mov  word ptr ds:[BDA_PointHeightOfCharacterMatrix], cx;Offset 0x485
//     mov  byte ptr ds:[BDA_RowsOnScreen], dl;Offset 0x484
//     mov  byte ptr [bp + 10h], dl
//     ret
}

void Func0x3125(uint8_t* font, uint8_t rowSpecifier, uint8_t rowCountIndex, uint16_t charHeight)
{
//     mov  di, es
//     mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
    SetUserFont(font, rowSpecifier, rowCountIndex, charHeight);
}

void SetUserUpper8x8Font()
{
    printf("SetUserUpper8x8Font - Not supported!\n"); //Let's hope the mode set doesn't use this.
//     mov  di, es
//     mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
//     cli
//     mov  word ptr es:[7ch], si          ;Offset 0x7c - SYSTEM DATA - 8x8 GRAPHICS FONT
//     mov  word ptr es:[7eh], di          ;Offset 0x7e
//     sti
//     ret
}

void Set8x14Font(uint8_t rowSpecifier, uint8_t rowCountIndex)
{
//     mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
//     mov  si, offset Characters8x14      ;Offset 0x5f20
//     mov  di, cs
//     mov  cx, 0eh
//     jmp  SetUserFont                    ;Offset 0x312c
    SetUserFont(Hag::S3::TrioBase::m_Characters8x14, rowSpecifier, rowCountIndex, 0x0E);
}

void Set8x8Font(uint8_t rowSpecifier, uint8_t rowCountIndex)
{
//     mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
//     mov  si, offset LowerCharacters8x8  ;Offset 0x5720
//     mov  di, cs
//     mov  cx, 08h
//     jmp  SetUserFont                    ;Offset 0x312c
    SetUserFont(Hag::S3::TrioBase::m_Characters8x8, rowSpecifier, rowCountIndex, 0x08);
}

void Set8x16Font(uint8_t rowSpecifier, uint8_t rowCountIndex)
{
//     mov  es, word ptr cs:[Data1488]     ;Offset 0x1488
//     mov  si, offset Characters8x16      ;Offset 0x6e30
//     mov  di, cs
//     mov  cx, 10h
//     jmp  SetUserFont                    ;Offset 0x312c
    SetUserFont(Hag::S3::TrioBase::m_Characters8x16, rowSpecifier, rowCountIndex, 0x10);
}

// ;inputs:
// ;bh = font index
// ;     00 = Upper 8x8 graphics font
// ;     01 = Current character font
// ;     02 = 8x14 font
// ;     03 = 8x8 character font
// ;     04 = 8x8 graphics font
// ;     05 = 8x14 font patch data
// ;     06 = 8x16 font
// ;     07 = 8x16 font patch data

void GetFontInfo()
{
    printf("GetFontInfo - Not supported!\n"); //Let's hope the mode set doesn't use this.
//     mov es, word ptr cs:[Data1488]      ;Offset 0x1488
//     or  bh, bh
//     jne NotUpperFont                    ;Offset 0x3176
//     les bx, es:[7ch]                    ;Offset 0x7c - SYSTEM DATA - 8x8 GRAPHICS FONT
//     jmp ReturnFontInfo                  ;Offset 0x3197
// NotUpperFont:                           ;Offset 0x3176
//     dec bh
//     jne NotCurrentFont                  ;Offset 0x3181
//     les bx, es:[10ch]                   ;Offset 0x10c - VIDEO DATA - CHARACTER TABLE (EGA,MCGA,VGA)
//     jmp ReturnFontInfo                  ;Offset 0x3197
// NotCurrentFont:                         ;Offset 0x3181
//     dec bh
//     cmp bh, 05h
//     ja  Exit                            ;Offset 0x31ae
//     mov ax, cs
//     mov es, ax
//     mov bl, bh
//     mov bh, 00h
//     add bx, bx
//     mov bx, word ptr cs:[bx + offset Data315a];Offset 0x315a
// ReturnFontInfo:                         ;Offset 0x3197
//     mov word ptr ss:[bp + 04h], bx
//     mov word ptr ss:[bp + 02h], es
//     mov ax, word ptr ds:[BDA_PointHeightOfCharacterMatrix];Offset 0x485
//     mov word ptr ss:[bp + 0ch], ax
//     mov al, byte ptr ds:[BDA_RowsOnScreen];Offset 0x484
//     mov byte ptr ss:[bp + 0ah], al
//     mov word ptr ss:[bp + 10h], 0000h
// Exit:                                   ;Offset 0x31ae
//     ret
}

// ;inputs:
// ;al = function + flag bit 4 = configure cursor
// ;bl = character generator ram bank
void TextModeCharFunctionsInternal(uint8_t function, 
    uint8_t* font, uint16_t someOffset, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank, uint8_t rowSpecifier, uint8_t rowCountIndex)
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
    r.h.al = function;
    r.h.bl = ramBank;
//     call Func0x227                      ;Offset 0x227 ands the top 2 bits off bl if al == 55
    Func0x227(r.h.al, r.h.bl);

//     mov  ah, al                         ;ah is now function
    r.h.ah = r.h.al;

//     and  al, 0fh                        ;keep bits 3-0
    r.h.al &= 0xf;

//     and  ah, 30h                        ;keep bits 5-4
    r.h.ah &= 0x30;

//     shr  ah, 01h                        ;now 4-3 
    r.h.ah >>= 1;
//     or   al, ah                         ;mush together. Effectively, bit 4 and 3 overlap. If either is set, or neither is set.
    r.h.al |= r.h.ah;

//     cmp  al, 19h                        ;if 25 or over, leave
//     jae  EmptyFunction9                 ;Offset 0x2f8e
    if (r.h.al >= 0x19)
        return;

//     mov  ah, 00h
    r.h.ah = 0x00;
//     mov  di, ax
//     shl  di, 01h
//     jmp  word ptr cs:[di + TextModeCharCallTable];Offset 0x2f39
    switch(r.h.al)
    {
    case 0:
        SetTextFontAndAddressing(font, someOffset, numCharacters, charHeight, ramBank);
        break;
    case 1:
        Set8x14TextFontAndAddressing(ramBank);
        break;
    case 2:
        Set8x8TextFontAndAddressing(ramBank);
        break;
    case 3:
        SelectCharacterFont(ramBank);
        break;
    case 4:
        Set8x16TextFontAndAddressing(ramBank);
        break;
    case 8:
        SetTextFontAddressingAndCursor(font, someOffset, numCharacters, charHeight, ramBank);
        break;
    case 9:
        Set8x14TextFontAddressingAndCursor(ramBank);
        break;
    case 10:
        Set8x8TextFontAddressingAndCursor(ramBank);
        break;
    case 12:
        Set8x16TextFontAddressingAndCursor(ramBank);
        break;
    case 16:
        SetUserUpper8x8Font();
        break;
    case 17:
        Func0x3125(font, rowSpecifier, rowCountIndex, charHeight);
        break;
    case 18:
        Set8x14Font(rowSpecifier, rowCountIndex);
        break;
    case 19:
        Set8x8Font(rowSpecifier, rowCountIndex);
        break;
    case 20:
        Set8x16Font(rowSpecifier, rowCountIndex);
        break;
    case 24:
        GetFontInfo();
        break;
    default:
        break;
    }
}

void SetFont()
{
    REGPACK r;
    memset(&r, 0, sizeof(r));
//     xor   bl, bl
    r.h.bl = 0x00;

//     mov   al, 01h
    r.h.al = 0x01;

//     cmp   word ptr ds:[BDA_PointHeightOfCharacterMatrix], 0eh;Offset 0x485 8x14
//     je    Is8x14                        ;Offset 0x1c80
    if (Hag::System::BDA::PointHeightOfCharacterMatrix::Get() == 0x0e)
        goto Is8x14;

//     mov   al, 02h
    r.h.al = 0x02;

//     cmp   word ptr ds:[BDA_PointHeightOfCharacterMatrix], 08h;Offset 0x485 8x8
//     je    Label0x1c8a                   ;Offset 0x1c8a
    if (Hag::System::BDA::PointHeightOfCharacterMatrix::Get() == 0x08)
        goto Label0x1c8a;

//     mov   al, 04h
    r.h.al = 0x04;

//     or    bl, 40h
    r.h.bl |= 0x40;

//     jmp   Label0x1c8a                   ;Offset 0x1c8a
    goto Label0x1c8a;

// Is8x14:                                 ;Offset 0x1c80
LABEL(SetFont, Is8x14);

//     cmp   byte ptr ds:[BDA_DisplayMode], 07h;Offset 0x449
//     jne   Label0x1c8a                   ;Offset 0x1c8a
    if (Hag::System::BDA::DisplayMode::Get() != 0x07)
        goto Label0x1c8a;

//     or    bl, 80h
    r.h.bl |= 0x80;

// Label0x1c8a:                            ;Offset 0x1c8a
LABEL(SetFont, Label0x1c8a);

//     call  TextModeCharFunctionsInternal ;Offset 0x2f71
    TextModeCharFunctionsInternal(r.h.al, NULL, 0x0000, 0x0000, 0x00, r.h.bl, 0x00, 0x00);//the 0 arguments aren't used.

//     ret   
}

// ;inputs:
// ;ax = offset in to es:bx table
bool CheckCurrentModeExists(uint8_t* ptr, uint16_t offset)
{
//     push bx
//     add  bx, ax
    ptr += offset;

//     mov  al, byte ptr ds:[BDA_DisplayMode];Offset 0x449
    uint8_t displayMode = Hag::System::BDA::DisplayMode::Get();
    uint8_t mode = 0;
// FindMode:                               ;Offset 0x1bd7
    do
    {
//     mov  ah, byte ptr es:[bx]
        mode = *ptr;

//     inc  bx
        ++ptr;
//     cmp  ah, 0ffh
//     je   NotFound                       ;Offset 0x1be6
        if(mode == 0xFF)
            return false;

//     DB 03Ah, 0C4h                       ;cmp  ah, al - masm encoding difference
//     jne  FindMode                       ;Offset 0x1bd7
        if (mode == displayMode)
            return true;

    } while (true);
//     pop  bx
//     ret
// NotFound:                               ;Offset 0x1be6
//     or   al, 0ffh
//     pop  bx
//     ret
}

// ;inputs:
// ;bx is index into video parameter table
// ;ax sub index into pulled table
// ;outputs:
// ;es:bx is new pointer - can be null
// ;al = 0xff if fail. unmodified otherwise.
// ;
// ;Fetches a pointer from the parameter block and then
// ;adds ax to it. after that, it runs through until 0xff or until current video mode found
// ;if found, it returns unmodified bx and al = video mode.
// ;if not found, it returns unmodified bx and al = 0xff
bool FetchCheckedVideoParameterBlockElement(uint16_t paramTableIdx, uint16_t subIdx, uint8_t*& parameterBlockElement, uint16_t size = sizeof(FARPointer))
{
//     call GetVideoParameterBlockElement  ;Offset 0x1d95
    if (GetVideoParameterBlockElement(paramTableIdx, parameterBlockElement, size))
        return CheckCurrentModeExists(parameterBlockElement, subIdx);
//     jne  CheckCurrentModeExists         ;Offset 0x1bd1
//     or   al, 0ffh                       ;
//     ret
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

//     mov  al, 10h                        ;SetTextFontAddressingAndCursor
    r.h.al = 0x10;

//     call TextModeCharFunctionsInternal  ;Offset 0x2f71
    TextModeCharFunctionsInternal(r.h.al, font, r.w.dx, r.w.cx, r.h.bh, r.h.bl, 0x00, 0x00);

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

//     mov   al, 00h
    r.h.al = 0x00;

//     push  bx
//     call  TextModeCharFunctionsInternal ;Offset 0x2f71
    TextModeCharFunctionsInternal(r.h.al, font, r.w.dx, r.w.cx, r.h.bh, r.h.bl, 0x00, 0x00);

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
    //     call  CheckCurrentModeExists        ;Offset 0x1bd1
    if (!CheckCurrentModeExists(paletteProfile, 0x14))
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
    REGPACK r;
    memset(&r, 0, sizeof(r));
    uint32_t offset = 0;
    Hag::S3::VideoParameters* overrideTable = NULL;
    uint8_t* fontDefinition = NULL;
    uint8_t* graphicsCharacterFontDefinition = NULL;
    uint8_t* paramBlock = NULL;
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
    if (Hag::S3::TrioBase::FindVideoModeData(r.h.al) != NULL)
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
    ApplyVESAOverrideData(r.h.al);

//     call SetPalette                     ;Offset 0x4909
    SetPalette();

//     mov  dx, word ptr ds:[BDA_VideoBaseIOPort];Offset 0x463
    r.w.dx = Hag::System::BDA::VideoBaseIOPort::Get();

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

//     call CheckCurrentModeExists         ;Offset 0x1bd1
//     jne  Label0x18c9                    ;Offset 0x18c9
    if (!CheckCurrentModeExists(fontDefinition, r.w.ax))
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
    EnablePaletteBasedVideo();

//     call TurnOnScreen                   ;Offset 0x4800
    Hag::VGA::Sequencer::ClockingMode::TurnScreenOn();

//     pop  cx
//     call EmptyFunction3                 ;Offset 0x1380
//     ret  
    return true;
}

}

#pragma pack(pop);
