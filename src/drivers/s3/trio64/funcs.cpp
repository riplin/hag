//Copyright 2023-Present riplin

#include <hag/system/bda.h>
#include <hag/system/pci.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/funcs.h>
#include <hag/drivers/s3/trio64/funcs.h>

namespace Hag { namespace S3 { namespace Trio64
{

VideoMode_t ConvertVesaModeToLegacy(Vesa::VideoMode_t mode)
{
    for (int i = 0; i < Data::VideoModeTranslationCount; ++i)
    {
        if (Data::VideoModeTranslation[i].VesaMode == mode)
            return Data::VideoModeTranslation[i].LegacyMode;
    }
    return VideoMode::Invalid;
}

VideoModeError_t CheckValidVideoMode(VideoMode_t mode)
{
    //Check legacy
    if (mode <= VGA::VideoMode::MaxValid)
        return VideoModeError::Success;

    //Check VESA and OEM
    for (int i = 0; i < Data::VideoModeTranslationCount; ++i)
    {
        if (Data::VideoModeTranslation[i].LegacyMode == mode)
            return VideoModeError::Success;
    }
    return VideoModeError::UnknownVideoMode;
}

Shared::VESAVideoModeData* FindVideoModeData(VideoMode_t mode)
{
    Shared::VESAVideoModeData* videoModeData = NULL;
    for (int i = 0; i < Data::VesaVideoModesCount; ++i)
    {
        if (Data::VesaVideoModes[i].Mode == mode)
        {
            videoModeData = &Data::VesaVideoModes[i];
            break;
        }
    }
    return videoModeData;
}

uint16_t GetDisplayMemoryInKiB()
{
    static uint16_t memorySizeInKB = 0xFFFF;

    //Early out.
    if (memorySizeInKB != 0xFFFF)
        return memorySizeInKB;

    memorySizeInKB = CRTController::Configuration1::GetDisplayMemorySizeInKiB(Hag::System::BDA::VideoBaseIOPort::Get());

    return memorySizeInKB;
}

void* GetLinearFrameBufferInternal(Shared::VESAVideoModeData* vesaData)
{
    void* ret = NULL;
    
    using namespace Hag::System;
    using namespace Hag::System::BDA;
    
    if (vesaData != NULL)
    {
        Register_t crtcPort = VideoBaseIOPort::Get();
        CRTController::RegisterLock1_t rl1 = CRTController::RegisterLock1::Read(crtcPort);
        CRTController::RegisterLock2_t rl2 = CRTController::RegisterLock2::Read(crtcPort);
        CRTController::RegisterLock1::Unlock(crtcPort);
        CRTController::RegisterLock2::Unlock(crtcPort);

        uint16_t deviceId = (uint16_t(CRTController::DeviceIDHigh::Read(crtcPort)) << 8) |
                                      CRTController::DeviceIDLow::Read(crtcPort);

        uint8_t bus = 0xFF;
        uint8_t slot = 0xFF;
        uint8_t function = 0xFF;
        if (PCI::FindDevice(0x5333, deviceId, bus, slot, function))
        {
            ret = (void*)PCI::Read32(bus, slot, function, PCI::Header0::BaseAddress0);

            CRTController::SystemConfiguration::Unlock(crtcPort);

            CRTController::LinearAddressWindowControl_t linearAddressControl = 
                CRTController::LinearAddressWindowControl::Read(crtcPort);
            CRTController::LinearAddressWindowControl::Write(crtcPort, linearAddressControl |
                                                             CRTController::LinearAddressWindowControl::EnableLinearAddressing);
            CRTController::ExtendedMemoryControl1::Write(crtcPort,
                                                         CRTController::ExtendedMemoryControl1::Read(crtcPort) |
                                                         CRTController::ExtendedMemoryControl1::EnableMMIOAccess);
        }
        CRTController::RegisterLock2::Lock(crtcPort, rl2);
        CRTController::RegisterLock1::Lock(crtcPort, rl1);
    }
    return ret;
}

VGA::VideoParameters* GetVideoParameters(VideoMode_t mode)
{
    using namespace Hag::System::BDA;

    VGA::VideoParameters* parameters = NULL;

    if (mode <= VideoMode::MaxValid)
    {
        parameters = Data::LegacyVideoModesV3[mode];
        if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0)
        {
            parameters = Data::LegacyVideoModesV2[mode];
            EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() &
                                                  EGAFeatureBitSwitches::AdapterTypeMask;

            if ((adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2) &&
                (adapterType != EGAFeatureBitSwitches::MDAColor80x25_2))
            {
                parameters = Data::LegacyVideoModesV1[mode];
            }
        }
    }
    else
    {
        Shared::VESAVideoModeData* vesaModeData = NULL;
        if (vesaModeData = FindVideoModeData(mode))
            parameters = vesaModeData->OverrideTable;
    }
    return parameters;
}

uint8_t FetchBusSpecificSystemConfig(VGA::Register_t crtcPort)
{
    return ((CRTController::Configuration1::Read(crtcPort) & 
            CRTController::Configuration1::SystemBusSelect) == 
            CRTController::Configuration1::PCIBus) ?
            Data::PCISystemConfig :
            Data::VLBSystemConfig;
}

void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, VGA::Register_t crtcPort)
{
    for (uint8_t crtcCount = 0; crtcCount < CRTCInitData[0]; ++crtcCount)
    {
        CRTControllerData::Write(crtcPort, CRTCInitData[1 + (crtcCount << 1)], CRTCInitData[2 + (crtcCount << 1)]);
    }

    for (uint8_t sequenceCount = 0; sequenceCount < Data::SequenceInitData[0]; ++sequenceCount)
    {
        Sequencer::Register_t index = Data::SequenceInitData[1 + (sequenceCount << 1)];
        SequencerData_t value = Data::SequenceInitData[2 + (sequenceCount << 1)];

        //Don't write the hsync / vsync data.
        if (index == Sequencer::Register::ExtendedSequencerD)
            value &= ~Sequencer::ExtendedSequencerD::SyncMask;

        SequencerData::Write(index, value);
    }

    CRTController::SystemConfiguration_t sysConf = CRTController::SystemConfiguration::Read(crtcPort);
    sysConf &= CRTController::SystemConfiguration::UpperBitsMask;
    sysConf |= FetchBusSpecificSystemConfig(crtcPort);
    CRTController::SystemConfiguration::Write(crtcPort, sysConf);

    CRTControllerIndex::Write(crtcPort, CRTController::Register::ModeControl);
    CRTController::ModeControl_t modeControl = CRTControllerData::Read(crtcPort + 1) & ~CRTController::ModeControl::Interlaced;
    CRTControllerData::Write(crtcPort + 1, modeControl);
}

void WaitGraphicsEngineReady(VGA::Register_t crtcPort)
{
    CRTController::SystemConfiguration_t systemConfig = CRTController::SystemConfiguration::Read(crtcPort);
    if (((systemConfig & CRTController::SystemConfiguration::EnableEnhancedRegisterAccess) != 0) &&
        GraphicsProcessorStatus::Read() != GraphicsProcessorStatus::FIFOStatusInvalid)
    {
        GraphicsProcessorStatus_t status = 0;
        do
        {
            status = GraphicsProcessorStatus::Read();
        } while ((status & GraphicsProcessorStatus::GraphicsEngineBusy) != 0);
    }
}

void ClearMemory(VGA::Register_t crtcPort)
{
    using namespace Hag::System::BDA;

    if ((VideoModeOptions::Get() & VideoModeOptions::DontClearDisplay) == 0)
    {
        CRTController::RegisterLock1::Unlock(crtcPort);
        CRTController::RegisterLock2::Unlock(crtcPort);

        CRTController::LinearAddressWindowControl_t linearAddressControl = 
            CRTController::LinearAddressWindowControl::Read(crtcPort);
        CRTController::LinearAddressWindowControl::Write(crtcPort, linearAddressControl &
                                                             ~CRTController::LinearAddressWindowControl::EnableLinearAddressing);

        CRTController::ExtendedSystemControl1_t extendedSystemControl = 
            CRTController::ExtendedSystemControl1::Read(crtcPort) & 
            CRTController::ExtendedSystemControl1::LowerMask;
        CRTController::ExtendedSystemControl1::Write(crtcPort, extendedSystemControl |
                                                         CRTController::ExtendedSystemControl1::Length16Bpp);

        CRTController::ExtendedMode_t extendedMode = CRTController::ExtendedMode::Read(crtcPort);
        CRTController::ExtendedMode::Write(crtcPort, extendedMode &
                                               CRTController::ExtendedMode::UnknownMask);

        CRTController::SystemConfiguration_t systemConfiguration = CRTController::SystemConfiguration::Read(crtcPort);
        CRTController::SystemConfiguration::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(AdvancedFunctionControl::EnableEnhancedFunctions |
                                                AdvancedFunctionControl::ReservedAs1 |
                                                AdvancedFunctionControl::EnhancedModePixelLength);

        WriteRegisterData::WriteTopScissors(WriteRegisterData::MinValue);
        WriteRegisterData::WriteLeftScissors(WriteRegisterData::MinValue);
        WriteRegisterData::WriteBottomScissors(WriteRegisterData::MaxValue);
        WriteRegisterData::WriteRightScissors(WriteRegisterData::MaxValue);

        WriteRegisterData::WriteMultifunctionControlMisc(MultifunctionControlMiscellaneous::SelectSlowReadModifyWriteCycle);

        BitplaneWriteMask::Write16x2(BitplaneWriteMask::AllSet);

        ForegroundColor::Write16x2(0);

        ForegroundMix::Write(ForegroundMix::MixNew | ForegroundMix::SelectForegroundColor);

        WriteRegisterData::WritePixelControl(PixelControl::MixForeground);

        CurrentXPosition::Write(0);
        CurrentYPosition::Write(0);

        MajorAxisPixelCount::Write(MajorAxisPixelCount::MaxValue);

        WriteRegisterData::WriteMinorAxisPixelCount(MinorAxisPixelCount::MaxValue);

        DrawingCommand::Write(DrawingCommand::MustBe1 |
                              DrawingCommand::AcrossThePlanePixelMode |
                              DrawingCommand::DrawPixel |
                              DrawingCommand::PosYXmajPosX |
                              DrawingCommand::CommandRectangleFill);

        WaitGraphicsEngineReady(crtcPort);
    }
}

void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data)
{
    Sequencer::UnlockExtendedSequencer::Unlock();
    Sequencer::ClockSynthControl2_t clockSynthControl2;
    if (idx == 0x22)
    {
        SequencerData::Write(Sequencer::Register::Unknown1B, 0x00);
        SequencerData::Write(Sequencer::Register::Unknown1A, 0x46);

        Sequencer::MClockValueLow::Write(0x46);
        Sequencer::MClockValueHigh::Write(0x79);
        Sequencer::DClockValueLow::Write(0x49);
        Sequencer::DClockValueHigh::Write(0x55);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                             Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }
    else
    {
        Sequencer::DClockValueLow::Write(data[1]);
        Sequencer::DClockValueHigh::Write(data[0]);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }

    Sequencer::RAMDACClockSynthControl::Write(0);

    Sequencer::ClockSynthControl2_t originalClockSynthControl2 = Sequencer::ClockSynthControl2::Read();

    originalClockSynthControl2 &= ~(Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::DivideDClockByTwo);
    
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2);
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2 | clockSynthControl2);
}

void SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig)
{
    uint8_t* clockDataPtr = CRTController::Revision::Read(crtcPort) != 0x03 ?
                            Data::ClockData :
                            Data::ClockDataRev3;

    uint32_t offset = (clockConfig & 0x1f) << 1;
    ConfigureDCLKAndMCLK(clockConfig, clockDataPtr + offset);

    CRTController::ModeControl::Write(crtcPort, clockConfig & CRTController::ModeControl::Interlaced);
}

uint8_t GetMemorySizeInMiB(VGA::Register_t crtcPort)
{
    CRTController::RegisterLock1::Unlock(crtcPort);
    CRTController::RegisterLock2::Unlock(crtcPort);

    uint8_t size;
    switch (CRTController::Configuration1::Read(crtcPort) & CRTController::Configuration1::DisplayMemorySize)
    {
    case CRTController::Configuration1::Size05MiB:
        size = 0;      //0.5MiB == 0 for some reason.
        break;
    case CRTController::Configuration1::Size1MiB:
        size = 1;
        break;
    case CRTController::Configuration1::Size2MiB:
        size = 2;
        break;
    case CRTController::Configuration1::Size4MiB:
        size = 4;
        break;
    default:
        size = 1;
        break;
    }
    return size;
}

void ConfigureExtraVESAModeSettings(VideoMode_t mode, VGA::Register_t crtcPort, Shared::VESAVideoModeData* overrideTable, Shared::VESAResolutionVariant* modeData)
{
    uint16_t logicalScreenWidth = overrideTable->ModeInfo->BytesPerScanline;

    if (overrideTable->ModeInfo->MemoryModelType == Vesa::ModeMemoryModel::Text)
        logicalScreenWidth <<= 1;
    
    if (overrideTable->ModeInfo->MemoryModelType == Vesa::ModeMemoryModel::EGA16ColorGraphics)
        logicalScreenWidth <<= 2;

    logicalScreenWidth >>= 3;
    uint8_t logicalScreenWidthHigh = uint8_t(logicalScreenWidth >> 8);
    VGA::CRTController::ScreenOffset::Write(crtcPort, uint8_t(logicalScreenWidth));
    CRTController::ExtendedSystemControl2::Write(crtcPort, logicalScreenWidthHigh <<
                                                     CRTController::ExtendedSystemControl2::Shift::LogicalScreenWidthHigh);

    CRTController::Miscellaneous1_t miscellaneous1 = CRTController::Miscellaneous1::RefreshCount1 |
                                                         CRTController::Miscellaneous1::EnableAlternateRefreshCount;

    CRTController::MemoryConfiguration_t memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                                                                   CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth;    

    if (overrideTable->ModeInfo->BitsPerPixel >= 8)
    {
        miscellaneous1 |= CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode;
        memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    CRTController::Miscellaneous1::Write(crtcPort, miscellaneous1);

    if ((mode == VideoMode::P800x600x4bpp) ||
        (mode == VideoMode::P1024x768x4bpp) ||
        (mode == VideoMode::P1280x1024x4bpp))
    {
        memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              CRTController::MemoryConfiguration::EnableTwoPageScreenImage |
                              CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth |
                              CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    CRTController::MemoryConfiguration::Write(crtcPort, memoryConfiguration);

    CRTController::ExtendedSystemControl1::Write(crtcPort, modeData->ExtendedSystemControl1);

    if ((modeData->ExtendedSystemControl1 & CRTController::ExtendedSystemControl1::Unknown) != 0)
    {
        MiscellaneousOutput_t miscOutput = MiscellaneousOutput::Read() & 
                                           ~(MiscellaneousOutput::SelectNegativeHorizontalSyncPulse |
                                             MiscellaneousOutput::SelectNegativeVerticalSyncPulse);
        MiscellaneousOutput::Write(miscOutput);
    }

    CRTController::BackwardCompatibility3::Write(crtcPort, 
                                                     CRTController::BackwardCompatibility3::EnableStartDisplayFIFOFetch);

    Shared::CRTController::DeviceIDLow_t deviceIdLow = Shared::CRTController::DeviceIDLow::Read(crtcPort);
    Shared::CRTController::DeviceIDHigh_t deviceIdHigh = Shared::CRTController::DeviceIDHigh::Read(crtcPort);

    if ((GetMemorySizeInMiB(crtcPort) == 1) ||              //Trio64 with 1MB
        ((deviceIdHigh == 0x88) && (deviceIdLow == 0x10)))  //Trio32
    {
        CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2_1MiB);
        CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3_1MiB);
    }
    else
    {
        CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2);
        CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3);
    }

    CRTController::ExtendedHorizontalOverflow::Write(crtcPort, modeData->ExtendedHorizontalOverflow);

    uint16_t horizontalTotal = (uint16_t(modeData->ExtendedHorizontalOverflow &
                                  CRTController::ExtendedHorizontalOverflow::HorizontalTotalHigh) << 8) |
                                  VGA::CRTController::HorizontalTotal::Read(crtcPort);

    CRTController::InterlaceRetraceStart::Write(crtcPort, CRTController::InterlaceRetraceStart_t(horizontalTotal >> 1));
    CRTController::StartDisplayFIFO::Write(crtcPort, horizontalTotal - 7);

    CRTController::SystemConfiguration::Unlock(crtcPort);

    if ((mode != VideoMode::T132x25x4bpp) &&
        (mode != VideoMode::T132x43x4bpp) &&
        ((CRTController::Miscellaneous1::Read(crtcPort) & 
        CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode) == 0))
    {
        VGA::CRTController::VerticalRetraceEnd_t verticalRetraceEnd = VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //I somehow get the feeling that these are tweaks that could also have been applied to the data.
        VGA::CRTController::StartHorizontalSyncPosition::Write(crtcPort,
                                                          VGA::CRTController::StartHorizontalSyncPosition::Read(crtcPort) + 2);

        VGA::CRTController::EndHorizontalSyncPosition::Write(crtcPort,
                                                        VGA::CRTController::EndHorizontalSyncPosition::Read(crtcPort) + 2);
        if (mode == VideoMode::P1280x1024x4bpp)
        {
            VGA::CRTController::StartHorizontalBlank::Write(crtcPort,
                                                       VGA::CRTController::StartHorizontalBlank::Read(crtcPort) - 1);
        }

        VGA::CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }

    CRTController::ExtendedVerticalOverflow::Write(crtcPort, modeData->ExtendedVerticalOverflow);

    CRTController::ExtendedMiscellaneousControl2::Write(crtcPort, modeData->ExtendedMiscellaneousControl2);

    if (modeData->ExtendedMiscellaneousControl2 == CRTController::ExtendedMiscellaneousControl2::ColorMode13)
    {
        VGA::CRTController::VerticalRetraceEnd_t verticalRetraceEnd = VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //Another tweak.
        VGA::CRTController::StartHorizontalBlank::Write(crtcPort,
                                                   VGA::CRTController::StartHorizontalBlank::Read(crtcPort) + 1);

        VGA::CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }
}

void Configure256KAddressingAndAddressWindow(VideoMode_t mode, VGA::Register_t crtcPort)
{
    if (mode > VideoMode::MaxValid)
        CRTController::BackwardCompatibility1::Write(crtcPort,
                                                         CRTController::BackwardCompatibility1::Read(crtcPort) &
                                                         ~CRTController::BackwardCompatibility1::StandardVGAMemoryMapping);

    //TODO: This information should come from the PCI configuration space.
    //Depending on the BIOS flags puts a dependency on the BIOS internal implementation.
    CRTController::ExtendedBIOSFlag3_t biosFlags3 = CRTController::ExtendedBIOSFlag3::Read(crtcPort);
    CRTController::LinearAddressWindowPositionH::Write(crtcPort, biosFlags3);
    CRTController::ExtendedBIOSFlag4_t biosFlags4 = CRTController::ExtendedBIOSFlag4::Read(crtcPort);
    CRTController::LinearAddressWindowPositionL::Write(crtcPort, biosFlags4 & 0x80);
}

void SetColorMode(VideoMode_t mode, Shared::VESAColorMode_t colorMode, VGA::Register_t crtcPort)
{
    CRTController::ExtendedMiscellaneousControl2_t cMode = 
        CRTController::ExtendedMiscellaneousControl2::ColorMode0;

    if (mode > VideoMode::MaxValid)
    {
        switch (colorMode)
        {
        case Shared::VESAColorMode::C8bpp2px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode8;
            break;
        case Shared::VESAColorMode::C24bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode13;
            break;
        case Shared::VESAColorMode::C24bppPacked:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode12;
            break;
        case Shared::VESAColorMode::C16bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode10;
            break;
        case Shared::VESAColorMode::C15bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode9;
            break;
        }
    }

    Sequencer::UnlockExtendedSequencer::Unlock();

    if (cMode == CRTController::ExtendedMiscellaneousControl2::ColorMode8)
    {
        Sequencer::RAMDACClockSynthControl::Write(Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl |
                                                      Sequencer::RAMDACClockSynthControl::EnableClockDoubleMode);

        Sequencer::ClockSynthControl2::Write(Sequencer::ClockSynthControl2::Read() |
                                                 Sequencer::ClockSynthControl2::DivideDClockByTwo);
    }
    else
    {
        Sequencer::RAMDACClockSynthControl::Write(Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl);
        
        Sequencer::ClockSynthControl2::Write(Sequencer::ClockSynthControl2::Read() & 
                                                 ~(Sequencer::ClockSynthControl2::DivideDClockByTwo |
                                                   Sequencer::ClockSynthControl2::InvertDClock));
    }

    CRTController::ExtendedMiscellaneousControl2::Write(crtcPort,
                                                            (CRTController::ExtendedMiscellaneousControl2::Read(crtcPort) &
                                                             ~CRTController::ExtendedMiscellaneousControl2::ColorMode) |
                                                             cMode);
}

void ApplyVESAOverrideData(VideoMode_t mode, VGA::Register_t crtcPort, Shared::VESAVideoModeData* overrideTable)
{
    Sequencer::UnlockExtendedSequencer::Unlock();

    Sequencer::ExtendedSequencer9::Write(Sequencer::ExtendedSequencer9::Read() &
                                             ~Sequencer::ExtendedSequencer9::MemoryMappedIOOnly);

    //This is a read-only register according to the spec...
    CRTController::ChipIDRevision::Write(crtcPort, 0x55);
    if (CRTController::ChipIDRevision::Read(crtcPort) != 0x55)
    {
        CRTController::SystemConfiguration::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(AdvancedFunctionControl::ReservedAs1);

        InitializeCRTControllerAndSequencer(Shared::Data::CRTControllerInitData, crtcPort);
    }

    if (overrideTable != NULL)
    {
        Shared::VESAResolutionVariant* modeData = overrideTable->VariantData;

        ClearMemory(crtcPort);

        //TODO: refresh rate limit should be passed in.
        uint16_t refreshRateCap = (uint16_t(CRTController::BIOSFlag::Read(crtcPort)) << 4) |
                                  (CRTController::ExtendedBIOSFlag1::Read(crtcPort) >> 4);

        for (int i = 0; i < 3; ++i)
        {
            if (overrideTable->OverrideTable == Data::VesaResolutions[i + 3])
                break;

            refreshRateCap >>= 3;
        }
        refreshRateCap &= 7;

        VGA::Sequencer::MemoryModeControl::Write(overrideTable->MemoryModeControl);

        CRTController::SystemConfiguration::Unlock(crtcPort);
        VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(overrideTable->AdvancedFunction);

        while (((modeData->FlagsAndFilter & Shared::VESAFlagsAndFilter::Terminate) == 0) && 
               ((modeData->FlagsAndFilter & ~Shared::VESAFlagsAndFilter::Terminate) > refreshRateCap))
        {
            ++modeData;
        }

        SetupClocks(crtcPort, modeData->ClockConfigIndex);

        for (CRTController::Register_t i = 0; i < 25; ++i)
        {
            CRTControllerData::Write(crtcPort, i, modeData->CRTCRegisters[i]);
        }

        ConfigureExtraVESAModeSettings(mode, crtcPort, overrideTable, modeData);
    }

    Configure256KAddressingAndAddressWindow(mode, crtcPort);

    Shared::VESAColorMode_t colorMode = 0;
    if (overrideTable != NULL)
        colorMode = overrideTable->ColorMode;

    SetColorMode(mode, colorMode, crtcPort);

    Sequencer::UnlockExtendedSequencer::Lock();
}

VideoModeError_t SetLegacyVideoModeInternal(VideoMode_t mode, Shared::VESAVideoModeData* vesaData)
{
    using namespace Hag::System;

    VGA::Register_t crtcPort = VGA::GetCRTControllerIndexRegister();
    
    CRTController::RegisterLock1::Unlock(crtcPort);
    CRTController::RegisterLock2::Unlock(crtcPort);

    VideoMode_t dontClearDisplay = mode & VideoMode::DontClearDisplay;

    mode &= ~VideoMode::DontClearDisplay;

    if ((mode <= VideoMode::MaxValid) ||
        (vesaData != NULL))
    {
        BDA::VideoModeOptions::Get() &= ~BDA::VideoModeOptions::DontClearDisplay;
        BDA::VideoModeOptions::Get() |= dontClearDisplay;

        BDA::ModeSetBDA(mode, (Data::FirmwareFlags & Shared::FirmwareFlag::Color) != 0, 
            ((vesaData != NULL) && ((vesaData->Flags & Shared::VESAVideoModeFlags::Color) == 0)));

        if ((mode <= VideoMode::MaxValid) &&
            !BDA::VerifyBDAOrDeactivate(mode, vesaData != NULL, (vesaData != NULL) && 
            ((vesaData->Flags & Shared::VESAVideoModeFlags::Color) != 0)))
        {
            return VideoModeError::AdapterNotActive;
        }

        BDA::DisplayMode::Get() = mode;

        VGA::VideoParameters* parameters = GetVideoParameters(mode);
        VGA::SetTextModeBiosData(mode, parameters);

        BDA::VideoModeOptions::Get() &= ~(BDA::VideoModeOptions::Unknown |
                                          BDA::VideoModeOptions::Inactive);

        VGA::SaveDynamicParameterData(parameters);
        VGA::ApplyVideoParameters(parameters);
        ApplyVESAOverrideData(mode, crtcPort, vesaData);

        bool hasVesaFlags = vesaData != NULL;
        Shared::VESAVideoModeFlags_t flags = hasVesaFlags ? vesaData->Flags : 0;
        bool ifVesaIsColor = hasVesaFlags && ((flags & Shared::VESAVideoModeFlags::Color) != 0x00);
        bool ifVesaIs256Color = hasVesaFlags && ((flags & Shared::VESAVideoModeFlags::Palette256) != 0x00);

        VGA::SetPalette(mode, ifVesaIsColor, ifVesaIs256Color);

        if ((mode < VideoMode::G320x200x2bppC) ||
            (mode == VideoMode::T80x25x1bppM) ||
            (hasVesaFlags &&
            ((flags & Shared::VESAVideoModeFlags::WindowGranularity64KiB) != 0)))
        {
            VGA::SetFont();

            BDA::AlphanumericCharSet* fontDefinition = NULL;
            if (BDA::GetVideoParameterBlockElementAs<BDA::AlphanumericCharSet>(2, fontDefinition, 0x0B + 0x14) &&
                BDA::CheckValidInCurrentMode(fontDefinition->ApplicableModes))
                VGA::ConfigureFontAndCursor(mode, fontDefinition);

            BDA::SecondarySavePointerTable* paramBlock = NULL;
            if (BDA::GetVideoParameterBlockElementAs<BDA::SecondarySavePointerTable>(4, paramBlock, 0x20) &&
                !paramBlock->SecondaryAlphanumericCharacterSetOverride.IsNull())
            {
                BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* graphicsFont =
                    paramBlock->SecondaryAlphanumericCharacterSetOverride.
                    ToPointer<BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable>(0x0B + 0x14);

                if (BDA::CheckValidInCurrentMode(graphicsFont->ApplicableModes))
                    VGA::ConfigureFontRamBank(graphicsFont);
            }
        }
        else
        {
            BDA::CursorScanLines::Get().End = 0;
            BDA::CursorScanLines::Get().Start = 0;

            BDA::GraphicsCharacterSetOverride* graphicsCharacterFontDefinition = NULL;
            if (BDA::GetVideoParameterBlockElementAs<BDA::GraphicsCharacterSetOverride>(3, graphicsCharacterFontDefinition, 0x07 + 0x14) &&
                BDA::CheckValidInCurrentMode(graphicsCharacterFontDefinition->ApplicableVideoModes))
                BDA::SetGraphicsCharacterFont(graphicsCharacterFontDefinition);
        }

        if (((BDA::VideoModeOptions::Get() & BDA::VideoModeOptions::DontClearDisplay) == 0x00) &&
            (BDA::VideoBufferSize::Get() != 0) &&
            (!hasVesaFlags || ((flags & Shared::VESAVideoModeFlags::WindowGranularity64KiB) != 0)))
            VGA::ClearScreen(mode, hasVesaFlags && ifVesaIsColor);

        VGA::SetPaletteProfile(crtcPort);
        VGA::EnableVideoDisplay();
        VGA::Sequencer::ClockingMode::TurnScreenOn();

        return VideoModeError::Success;
    }
    return VideoModeError::UnknownVideoMode;
}

VideoModeError_t SetVesaVideoModeInternal(Vesa::VideoMode_t mode)
{
    VideoMode_t legacyMode = VideoMode::Invalid;
    VideoMode_t intermediateMode = VideoMode::Invalid;
    if ((mode & Vesa::VideoMode::LegacyMask) == 0)
    {
        intermediateMode = VideoMode_t(mode);//Just drop the top byte
    }
    else
    {
        Vesa::VideoMode_t actualMode = mode & Vesa::VideoMode_t(~Vesa::VideoMode::DontClearDisplay);
        intermediateMode = ConvertVesaModeToLegacy(actualMode);
    }

    if (intermediateMode == VideoMode::Invalid)
        return VideoModeError::UnknownVideoMode;
    
    if ((mode & Vesa::VideoMode::DontClearDisplay) == Vesa::VideoMode::DontClearDisplay)
        legacyMode = intermediateMode | VideoMode::DontClearDisplay;

    return SetLegacyVideoModeInternal(legacyMode, FindVideoModeData(intermediateMode));
}

void WaitForVSync()
{
    VGA::Register_t inputStatus1 = Hag::System::BDA::VideoBaseIOPort::Get() + 6;
    InputStatus1_t syncStatus = 0;

    do
    {
        syncStatus = InputStatus1::Read(inputStatus1) & InputStatus1::VerticalSyncActive;
    } while (syncStatus != 0);

    do
    {
        syncStatus = InputStatus1::Read(inputStatus1) & InputStatus1::VerticalSyncActive;
    } while (syncStatus == 0);
}

void SetDisplayStart(uint16_t x, uint16_t y)
{
    VGA::Register_t crtcPort = VGA::GetCRTControllerIndexRegister();

    VGA::CRTController::ScreenOffset_t screenOffsetLow = VGA::CRTController::ScreenOffset::Read(crtcPort);
    CRTController::ExtendedSystemControl2_t screenOffsetHigh = CRTController::ExtendedSystemControl2::Read(crtcPort) &
                                                               CRTController::ExtendedSystemControl2::LogicalScreenWidthHigh;
    uint16_t screenOffset = ((uint16_t(screenOffsetHigh) << 4) | uint16_t(screenOffsetLow)) << 1;

    CRTController::ExtendedMiscellaneousControl2_t colorMode = CRTController::ExtendedMiscellaneousControl2::Read(crtcPort) & 
                                                               CRTController::ExtendedMiscellaneousControl2::ColorMode;

    uint16_t shift = 2;
    if (colorMode <= CRTController::ExtendedMiscellaneousControl2::ColorMode8)
    {
        if (((VGA::Sequencer::MemoryModeControl::Read() & VGA::Sequencer::MemoryModeControl::SelectChain4Mode) == 0))
        {
            shift = 3;
        }
    }
    else if (colorMode <= CRTController::ExtendedMiscellaneousControl2::ColorMode10)
    {
        shift = 1;
    }
    else
    {
        shift = 0;
    }

    uint32_t yStart = screenOffset * y;
    if ((yStart >> 16) > 0x0F)
        return; // out of bounds

    uint32_t newStartOffset = yStart + (x >> shift);
    CRTController::ExtendedSystemControl3::Write(crtcPort, uint8_t(newStartOffset >> 16));
    VGA::CRTController::StartAddressHigh::Write(crtcPort, uint8_t(newStartOffset >> 8));
    VGA::CRTController::StartAddressLow::Write(crtcPort, uint8_t(newStartOffset));
}

void WaitGraphicsEngineReadyFast()
{
    for (GraphicsProcessorStatus_t status = 0;
         (status & GraphicsProcessorStatus::GraphicsEngineBusy) != 0;
          status = MMIO::GraphicsProcessorStatus::Read())
    {}
}

void SetScissors(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
    WaitGraphicsEngineReadyFast();
    MMIO::Packed::ScissorsTopLeft::Write(left, top);
    MMIO::Packed::ScissorsBottomRight::Write(right, bottom);
}

void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
    WaitGraphicsEngineReadyFast();
    MMIO::Packed::ForegroundColor::Write(color);
    MMIO::ForegroundMix::Write(ForegroundMix::MixNew | ForegroundMix::SelectForegroundColor);
    MMIO::Packed::PixelControlAndMultiFunctionMisc2::Write(0, PixelControl::MixForeground);

    uint16_t width = abs(x1 - x0);
    uint16_t height = abs(y1 - y0);
    uint16_t vmax = max(width, height);
    if (vmax != 0)
    {
        uint16_t vmin = min(width, height);
        MMIO::Packed::CurrentXYPosition::Write(x0, y0);
        MMIO::Packed::DestinationXYPositionStepConstant::Write((vmin - vmax) << 1, vmin << 1);
        MMIO::MajorAxisPixelCount::Write(vmax - 1);

        DrawingCommand_t xpos = x0 < x1 ? DrawingCommand::PosX : DrawingCommand::NegX;
        DrawingCommand_t ypos = y0 < y1 ? DrawingCommand::PosY : DrawingCommand::NegY;
        DrawingCommand_t xmaj = width > height ? DrawingCommand::XMajor : DrawingCommand::YMajor;

        LineErrorTerm_t errorTerm = (vmin - vmax) << 1;
        if (xpos != DrawingCommand::PosX)
            ++errorTerm;

        MMIO::LineErrorTerm::Write(errorTerm);

        MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                    DrawingCommand::LastPixelOff |
                                    DrawingCommand::DrawPixel |
                                    xpos | ypos | xmaj |
                                    DrawingCommand::CommandDrawLine);
    }
}

void DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
    if (y1 < y0)
    {
        uint16_t tmpX = x0;
        uint16_t tmpY = y0;
        x0 = x1;
        y0 = y1;
        x1 = tmpX;
        y1 = tmpY;
    }
    
    if (y2 < y0)
    {
        uint16_t tmpX = x0;
        uint16_t tmpY = y0;
        x0 = x2;
        y0 = y2;
        x2 = tmpX;
        y2 = tmpY;
    }

    if (x1 > x2)
    {
        uint16_t tmpX = x1;
        uint16_t tmpY = y1;
        x1 = x2;
        y1 = y2;
        x2 = tmpX;
        y2 = tmpY;
    }

    WaitGraphicsEngineReadyFast();
    MMIO::Packed::ForegroundColor::Write(color);
    MMIO::ForegroundMix::Write(ForegroundMix::MixNew | ForegroundMix::SelectForegroundColor);
    MMIO::Packed::PixelControlAndMultiFunctionMisc2::Write(0, PixelControl::MixForeground);
    if ((y0 != y1) && (y0 != y2))//flat top check
    {
        MMIO::Packed::CurrentXYPosition::Write(x0, y0);
        MMIO::Packed::DestinationXYPositionStepConstant::Write(x1, y1);
        MMIO::Packed::CurrentXYPosition2::Write(x0, y0);
        MMIO::Packed::XYCoordinate2::Write(x2, y2);

        MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                    DrawingCommand::LastPixelOff |
                                    DrawingCommand::DrawPixel |
                                    DrawingCommand::CommandPolygonFillSolid);
        if (y1 != y2)//flat bottom
        {
            WaitGraphicsEngineReadyFast();
            if (y1 < y2)
            {
                MMIO::Packed::DestinationXYPositionStepConstant::Write(x2, y2);
            }
            else if (y2 < y1)
            {
                MMIO::Packed::XYCoordinate2::Write(x1, y1);
            }

            MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                        DrawingCommand::LastPixelOff |
                                        DrawingCommand::DrawPixel |
                                        DrawingCommand::CommandPolygonFillSolid);
        }
    }
    //flat top
    else if (y0 == y1)
    {
        MMIO::Packed::CurrentXYPosition::Write(x0, y0);
        MMIO::Packed::DestinationXYPositionStepConstant::Write(x2, y2);
        MMIO::Packed::CurrentXYPosition2::Write(x1, y1);
        MMIO::Packed::XYCoordinate2::Write(x2, y2);

        MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                    DrawingCommand::LastPixelOff |
                                    DrawingCommand::DrawPixel |
                                    DrawingCommand::CommandPolygonFillSolid);
    } else //y0 == y2
    {
        MMIO::Packed::CurrentXYPosition::Write(x2, y2);
        MMIO::Packed::DestinationXYPositionStepConstant::Write(x1, y1);
        MMIO::Packed::CurrentXYPosition2::Write(x0, y0);
        MMIO::Packed::XYCoordinate2::Write(x1, y1);

        MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                    DrawingCommand::LastPixelOff |
                                    DrawingCommand::DrawPixel |
                                    DrawingCommand::CommandPolygonFillSolid);
    }
}

void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color)
{
    WaitGraphicsEngineReadyFast();
    MMIO::Packed::ForegroundColor::Write(color);
    MMIO::ForegroundMix::Write(ForegroundMix::MixNew | ForegroundMix::SelectForegroundColor);
    MMIO::Packed::PixelControlAndMultiFunctionMisc2::Write(0, PixelControl::MixForeground);

    uint16_t px = min(x0, x1);
    uint16_t py = min(y0, y1);
    uint16_t pw = (max(x0, x1) - px) - 1;
    uint16_t ph = (max(y0, y1) - py) - 1;

    MMIO::Packed::CurrentXYPosition::Write(x0, y0);
    MMIO::Packed::MinorMajorAxisPixelCounts::Write(pw, ph);

    MMIO::DrawingCommand::Write(DrawingCommand::MustBe1 |
                                DrawingCommand::DrawPixel |
                                DrawingCommand::PosYXmajPosX |
                                DrawingCommand::CommandRectangleFill);
}

}}}
