//Copyright 2023-Present riplin

#pragma once

#include <string.h>
#include <hag/system/bda.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/funcs.h>
#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/vga/data.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/attribc/index.h>
#include <hag/drivers/vga/attribc/palette.h>
#include <hag/drivers/vga/crtc/curstrsl.h>
#include <hag/drivers/vga/crtc/curendsl.h>
#include <hag/drivers/vga/crtc/index.h>
#include <hag/drivers/vga/crtc/maxscanl.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/undloc.h>
#include <hag/drivers/vga/crtc/verdisen.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/gfxc/gfxcmode.h>
#include <hag/drivers/vga/gfxc/mmmctrl.h>
#include <hag/drivers/vga/gfxc/rdplnsel.h>
#include <hag/drivers/vga/sqrc/reset.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/memodctl.h>


namespace Hag { namespace VGA
{

Register_t GetCRTControllerIndexRegister()
{
    return (MiscellaneousOutput::Read() & 
            MiscellaneousOutput::IOAddressSelect) == 
            MiscellaneousOutput::IOAddressSelect ?
            Register::CRTControllerIndexD :
            Register::CRTControllerIndexB;
}

void ClearScreen(VideoMode_t mode, bool ifVesaIsColor)
{
    uint32_t size =  0x2000;
    uint16_t segment = 0;
    uint32_t value = 0;

    if (mode > VideoMode::MaxValid)
    {
        segment = 0xB800;
        value = 0x07200720;

        if (!ifVesaIsColor)
        {
            segment = 0xB000;
        }
    }
    else
    {
        if (mode == VideoMode::T80x25x1bppM)
        {
            segment = 0xB000;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::T80x25x4bppC)
        {
            segment = 0xB800;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::G640x200x1bppM)
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
    //0x40 = 8x14, 0x80 = 8x16, 0x00 = no patch.
    if ((flags & 0xC0) != 0x00)
    {
        bool is8x16 = (flags & 0x80) == 0x00;
        uint16_t count = is8x16 ? 16 : 14;
        uint8_t* src = is8x16 ? Data::CharacterPatch8x16 : Data::CharacterPatch8x14;

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

void SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank)
{
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
    
    Sequencer::EnableWritePlane::Write(Sequencer::EnableWritePlane::Plane3);
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

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane1);

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
    EnableVideoDisplay();
}

void ConfigureCursorPropertiesAndVerticalDisplayEnd(VideoMode_t mode, uint8_t characterPointHeight)
{
    using namespace Hag::System;

    Register_t crtcPort = BDA::VideoBaseIOPort::Get();

    BDA::PointHeightOfCharacterMatrix::Get() = characterPointHeight;

    uint16_t screenHeight = 0;

    if ((mode == VideoMode::G640x480x1bppM) ||
        (mode == VideoMode::G640x480x4bppC))
    {
        screenHeight = 400;
    }
    else if ((mode == VideoMode::G320x200x8bppC) ||
        (mode == VideoMode::G640x200x1bppM) ||
        (mode == VideoMode::G320x200x2bppG) ||
        ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x200x4bppC)))
    {
        screenHeight = 200;
    }
    else if ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x350x2bppC))
    {
        screenHeight = 350;
    }
    else
    {
        BDA::EGAFeatureBitSwitches_t adapterType = BDA::EGAFeatureBitSwitches::Get() & BDA::EGAFeatureBitSwitches::AdapterTypeMask;

        if ((BDA::VideoDisplayDataArea::Get() & BDA::VideoDisplayDataArea::LineMode400) != 0)
        {
            screenHeight = 400;
        }
        else if (((BDA::VideoModeOptions::Get() & 0x02) != 0) ||
                (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced) ||
                (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2) ||
                (mode == VideoMode::T80x25x1bppM))
        {
            screenHeight = 350;
        }
        else
        {
            screenHeight = 200;
        }
    }

    uint8_t rowsOnScreen = uint8_t(screenHeight / characterPointHeight);
    BDA::RowsOnScreen::Get() = rowsOnScreen - 1;

    BDA::NumberOfScreenColumns_t bytesPerLine = BDA::NumberOfScreenColumns::Get() << 1;

    BDA::VideoBufferSize::Get() = bytesPerLine * rowsOnScreen + 256;

    uint8_t charPointHeightMinusOne = characterPointHeight - 1;

    if (mode == VideoMode::T80x25x1bppM)
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

    BDA::CursorScanLines::Get().End = cursorBottom;
    BDA::CursorScanLines::Get().Start = cursorTop;

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
    using namespace Hag::System;

    if (BDA::PointHeightOfCharacterMatrix::Get() == 14)
    {
        SetTextFontAndAddressing(Data::Characters8x14, 0, 256, 14, BDA::DisplayMode::Get() == VideoMode::T80x25x1bppM ? 0x80 : 0);
    }
    else if (BDA::PointHeightOfCharacterMatrix::Get() == 8)
    {
        SetTextFontAndAddressing(Data::Characters8x8, 0, 256, 8, 0);
    }
    else
    {
        SetTextFontAndAddressing(Data::Characters8x16, 0, 256, 16, 0x40);
    }
}

void ConfigureFontAndCursor(VideoMode_t mode, System::BDA::AlphanumericCharSet* fontDefinition)
{
    using namespace Hag::System;
    
    uint8_t* font = fontDefinition->FontData.ToPointer<uint8_t>(fontDefinition->CharacterHeight * fontDefinition->NumCharacters);

    SetTextFontAndAddressing(font,
                             fontDefinition->FirstCharacter,
                             fontDefinition->NumCharacters,
                             fontDefinition->CharacterHeight,
                             fontDefinition->RamBank & 0x3F);
    ConfigureCursorPropertiesAndVerticalDisplayEnd(mode, fontDefinition->CharacterHeight);

    if (fontDefinition->Rows != 0xFF)
    {
        BDA::RowsOnScreen::Get() = fontDefinition->Rows - 1;
    }
}

void ConfigureFontRamBank(System::BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* fontDefinition)
{
    using namespace Hag::System;

    if (fontDefinition->CharacterHeight != BDA::PointHeightOfCharacterMatrix::Get())
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

void SetTextModeBiosData(VideoMode_t mode, VideoParameters* parameters)
{
    using namespace Hag::System::BDA;

    static uint8_t Mode0_7ControlRegValue[] = { 0x2C, 0x28, 0x2D, 0x29, 0x2A, 0x2E, 0x1E, 0x29 };

    for (int i = 0; i < 8; ++i)
    {
        CursorPositions::Get()[i].Column = 0;
        CursorPositions::Get()[i].Row = 0;
    }

    ActiveDisplayNumber::Get() = 0;
    VideoBufferOffset::Get() = 0;

    if (DisplayMode::Get() <= VideoMode::T80x25x1bppM)
    {
        CGAColorPaletteMaskSetting::Get() = DisplayMode::Get() == VideoMode::G640x200x1bppM ?
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

    NumberOfScreenColumns::Get() = parameters->DisplayedCharacterColumns;
    RowsOnScreen::Get() = parameters->DisplayedRowsMinus1;
    PointHeightOfCharacterMatrix::Get() = parameters->CharacterMatrixHeightPoints;
    VideoBufferSize::Get() = parameters->VideoBufferSize;
    CursorScanLines::Get().End = parameters->CRTCRegisters[11];
    CursorScanLines::Get().Start = parameters->CRTCRegisters[10];
}

void ApplyVideoParameters(VideoParameters* parameters)
{
    using namespace Hag::System::BDA;

    PrepareAttributeController();

    SYS_ClearInterrupts();
    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset);
    Sequencer::ClockingMode::Write(Sequencer::ClockingMode::ScreenOff);
    Sequencer::ClockingMode::Write(Sequencer::ClockingMode::ScreenOff | parameters->SequencerRegisters[0]);
    MiscellaneousOutput::Write(parameters->MiscOutputRegisterValues);
    Sequencer::Reset::Write(Sequencer::Reset::AsynchronousReset | Sequencer::Reset::SynchronousReset);
    SYS_RestoreInterrupts();

    Sequencer::EnableWritePlane::Write(parameters->SequencerRegisters[1]);
    Sequencer::CharacterFontSelect::Write(parameters->SequencerRegisters[2]);
    Sequencer::MemoryModeControl::Write(parameters->SequencerRegisters[3]);

    Register_t crtRegister = VideoBaseIOPort::Get();

    if ((DisplayMode::Get() < VideoMode::Unknown1) ||
        (DisplayMode::Get() > VideoMode::Reserved2))
    {
        crtRegister = (parameters->MiscOutputRegisterValues & MiscellaneousOutput::IOAddressSelect) == 0 ? 
                        Register::CRTControllerIndexB :
                        Register::CRTControllerIndexD;
    }
    VideoBaseIOPort::Get() = crtRegister;

    CRTController::VerticalRetraceEnd::Write(crtRegister, 0);

    for (uint8_t crtIndex = 0; crtIndex < 25; ++crtIndex)
    {
        CRTControllerData::Write(crtRegister, crtIndex, parameters->CRTCRegisters[crtIndex]);
    }
    
    //Reset Attribute Controller port 0x3c0 to point to index register
    Register_t inputStatus1 = crtRegister + (Register::InputStatus1D - Register::CRTControllerIndexD);
    InputStatus1::Read(inputStatus1);

    if ((DisplayMode::Get() == VideoMode::Unknown2) ||
        (DisplayMode::Get() == VideoMode::Reserved1))
    {
        InputStatus1::Read(Register::InputStatus1B);
    }

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        for (uint8_t attribIndex = 0; attribIndex < 16; ++attribIndex)
        {
            AttributeControllerData::Write(attribIndex, parameters->AttributeControllerRegs[attribIndex]);
        }
    }

    uint8_t attribIndex = 16;
    for (uint8_t attribCount = 0; attribCount < 5; ++attribCount)
    {
        if ((attribIndex != 17) || ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0))
        {
            AttributeControllerData_t value = attribIndex == 20 ? 0 :parameters->AttributeControllerRegs[16 + attribCount];
            AttributeControllerData::Write(attribIndex, value);
        }
        ++attribIndex;
    }

    for (uint8_t gfxIndex = 0; gfxIndex < 9; ++gfxIndex)
    {
        GraphicsControllerData::Write(gfxIndex, parameters->GraphicsControllerRegs[gfxIndex]);
    }
}

void PrepareAttributeController()
{
    using namespace Hag::System::BDA;

    InputStatus1::Read(VideoBaseIOPort::Get() + (Register::InputStatus1D - Register::CRTControllerIndexD));
    AttributeControllerIndex::Write(AttributeController::Register::Palette0);
}

void SaveDynamicParameterData(VideoParameters* parameters)
{
    using namespace Hag::System::BDA;

    uint8_t* savePointer;
    if (!GetVideoParameterBlockElement(1, savePointer, 0x100))
        return;

    memcpy(savePointer, parameters->AttributeControllerRegs, 16);
    savePointer += 16;
    *savePointer = parameters->AttributeControllerRegs[17];
}

void SetPaletteProfile(Register_t crtcPort)
{
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

void EnableVideoDisplay()
{
    using namespace Hag::System::BDA;

    Register_t inputStatus = VideoBaseIOPort::Get() + (Register::InputStatus1D - Register::CRTControllerIndexD);
    AttributeControllerIndex::ResetIndex(inputStatus);
    AttributeControllerIndex::Write(AttributeControllerIndex::EnableVideoDisplay);
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
    using namespace Hag::System::BDA;

    uint16_t colorIndex = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
 
    colorIndex = ApplyPalette(colorIndex, 16, ((VideoDisplayDataArea::Get() &
                                               (VideoDisplayDataArea::GrayScale |
                                                VideoDisplayDataArea::MonochromeMonitor)) == 0) ?
                                                Data::ColorPalette : Data::MonochromePalette);

    colorIndex = ApplyPalette(colorIndex, 16, Data::SecondPalette);

    if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
    {
        ApplyPalette(colorIndex, 216, Data::Color216Palette);
    }
    else
    {
        uint8_t* palettePtr = Data::Color216Palette;
        for (int i = 0; i < 216; ++i)
        {
            palettePtr = DecompressPaletteColor(palettePtr, red, green, blue);
            red = green = blue = ((blue * 36043 + green * 19333 + red * 9830 + 16384) >> 15) & 0xFF;
            colorIndex = SetPaletteColor(colorIndex, red, green, blue);
        }
    }
}

void SetPalette(VideoMode_t mode, bool ifVesaIsColor, bool ifVesaIs256Color)
{
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        if (DACMask::Read() != 0xFF)
            DACMask::Write(0xFF);

        bool paletteNotSet = false;
        if ((mode == VideoMode::T80x25x1bppM) ||
            (mode == VideoMode::G640x350x1bppM))
        {
            ApplyPalette(0, 64, Data::Monochrome64Palette);
        }
        else if (mode == VideoMode::G320x200x8bppC)
        {
            Set248ColorPalette();
        }
        else if (mode > VideoMode::MaxValid)
        {
            if (!ifVesaIsColor)
            {
                ApplyPalette(0, 64, Data::Monochrome64Palette);
            }
            else if (ifVesaIs256Color)
            {
                Set248ColorPalette();
            }
            else paletteNotSet = true;
        }
        else paletteNotSet = true;

        if (paletteNotSet)
        {
            bool paletteSet3 = false;
            if (mode < VideoMode::G320x200x2bppC)
            {
                EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

                paletteSet3 = (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2));
            }
            else
            {
                paletteSet3 = ((mode != VideoMode::Unknown1) && (mode <= VideoMode::G640x200x4bppC));
            }

            if (paletteSet3)
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, Data::ColorPalette3);
                }
                else
                {
                    ApplyPalette(0, 64, Data::MonochromePalette3);
                }
            }
            else
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, Data::ColorPalette2);
                }
                else
                {
                    ApplyPalette(0, 64, Data::MonochromePalette2);
                }
            }
        }
    }
}

}}
