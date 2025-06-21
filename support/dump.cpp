//Copyright 2025-Present riplin

#include <support/dump.h>
#include <hag/drivers/vga/vga.h>
#include <hag/system/bda.h>
#include <hag/system/pci.h>

namespace Support
{

struct ScanContext
{
    FILE* fp;
    FILE* fpbin;
    Device* devices;
    const char* vendorName;
    uint16_t deviceCount;
    uint16_t vendor;
};

bool FindDevices(uint8_t bus, uint8_t slot, uint8_t function, void* context)
{
    using namespace Hag::System;

    ScanContext* ctx = (ScanContext*)context;

    if (PCI::GetVendorId(bus, slot, function) == ctx->vendor)
    {
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "\n");

        uint16_t deviceID = PCI::GetDeviceId(bus, slot, function);
        bool found = false;
        for (uint16_t deviceIdx = 0; deviceIdx < ctx->deviceCount; ++deviceIdx)
        {
            if (ctx->devices[deviceIdx].DeviceID == deviceID)
            {
                found = true;
                if (ctx->fp != nullptr)
                    fprintf(ctx->fp, "%s:\n", ctx->devices[deviceIdx].Name);
                break;
            }
        }
        if (!found)
        {
            if (ctx->fp != nullptr)
                fprintf(ctx->fp, "Unknown %s device 0x%04X:\n", ctx->vendorName, deviceID);
        }
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "Bus: 0x%02X, Slot: 0x%02X, Function: 0x%02X\n", bus, slot, function);
        for (uint8_t pciRegIdx = 0; pciRegIdx < 64; ++pciRegIdx)
        {
            uint32_t pciReg = PCI::Read32(bus, slot, function, pciRegIdx << 2);
            if (ctx->fp != nullptr)
                fprintf(ctx->fp, "0x%02X: 0x%02X, 0x%02X, 0x%02X, 0x%02X,    0x%08lX\n",
                        pciRegIdx,
                        uint8_t(pciReg),
                        uint8_t(pciReg >> 8),
                        uint8_t(pciReg >> 16),
                        uint8_t(pciReg >> 24),
                        pciReg);
            if (ctx->fpbin != nullptr)
            {
                fwrite(&pciReg, sizeof(pciReg), 1, ctx->fpbin);
            }
        }
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "\n");
    }
    return true;
}

void PCIDump(FILE* fptext, FILE* fpbin, uint16_t vendor, const char* vendorName, Device* devices, uint16_t deviceCount)
{
    ScanContext context;
    context.fp = fptext;
    context.fpbin = fpbin;
    context.devices = devices;
    context.deviceCount = deviceCount;
    context.vendor = vendor;
    context.vendorName = vendorName;

    Hag::System::PCI::ScanBus(0, FindDevices, &context);
}

void BDADump(FILE* fptext, FILE* fpbin)
{
    using namespace Hag::System::BDA;

    if (fpbin != nullptr) fwrite(&SystemBDA(), 256, 1, fpbin);
    if (fptext != nullptr)
    {
        fprintf(fptext, "BIOS Data Area:\n\n");
        fprintf(fptext, "Display mode                  : 0x%02X\n", DisplayMode::Get());
        fprintf(fptext, "Detected hardware             : 0x%02X\n", DetectedHardware::Get());
        fprintf(fptext, "Number of screen columns      : 0x%04X\n", NumberOfScreenColumns::Get());
        fprintf(fptext, "Video buffer size             : 0x%04X\n", VideoBufferSize::Get());
        fprintf(fptext, "Video buffer offset           : 0x%04X\n", VideoBufferOffset::Get());
    
        for (uint8_t cursorPos = 0; cursorPos < 8; ++cursorPos)
        {
            fprintf(fptext, "Cursor position %i             : 0x%02X, 0x%02X\n", cursorPos,
                CursorPositions::Get()[cursorPos].Column,
                CursorPositions::Get()[cursorPos].Row);
        }
        fprintf(fptext, "Cursor start scanline         : 0x%02X\n", CursorScanLines::Get().Start);
        fprintf(fptext, "Cursor end scanline           : 0x%02X\n", CursorScanLines::Get().End);
        fprintf(fptext, "Active display number         : 0x%02X\n", ActiveDisplayNumber::Get());
        fprintf(fptext, "Video Base IO port            : 0x%04X\n", VideoBaseIOPort::Get());
        fprintf(fptext, "CRT mode control reg          : 0x%02X\n", CRTModeControlRegValue::Get());
        fprintf(fptext, "CGA Color Palette mask        : 0x%02X\n", CGAColorPaletteMaskSetting::Get());
        fprintf(fptext, "Rows on screen                : 0x%02X\n", RowsOnScreen::Get());
        fprintf(fptext, "Point height of Char Matrix   : 0x%02X\n", PointHeightOfCharacterMatrix::Get());
        fprintf(fptext, "Video mode options            : 0x%02X\n", VideoModeOptions::Get());
        fprintf(fptext, "EGA feature bit switches      : 0x%02X\n", EGAFeatureBitSwitches::Get());
        fprintf(fptext, "Video display data area       : 0x%02X\n", VideoDisplayDataArea::Get());
        fprintf(fptext, "Display combination code index: 0x%02X\n", DisplayCombinationCodeTableIndex::Get());
        fprintf(fptext, "Video parameter ctrl block ptr: %04X:%04X\n", VideoParameterControlBlockPointer::Get().Segment, VideoParameterControlBlockPointer::Get().Offset);
        if (!VideoParameterControlBlockPointer::Get().IsNull())
        {
            VideoParameterControlBlock* videoParameterControlBlock = VideoParameterControlBlockPointer::Get().ToPointer<VideoParameterControlBlock>();
            fprintf(fptext, "\nVideo parameter control block:\n");
            fprintf(fptext, "Video parameters              : %04X:%04X\n", videoParameterControlBlock->VideoParameters.Segment, videoParameterControlBlock->VideoParameters.Offset);        
            fprintf(fptext, "Dynamic param save area       : %04X:%04X\n", videoParameterControlBlock->DynamicParamSaveArea.Segment, videoParameterControlBlock->DynamicParamSaveArea.Offset);
            fprintf(fptext, "Alphanumeric Charset override : %04X:%04X\n", videoParameterControlBlock->AlphanumericCharsetOverride.Segment, videoParameterControlBlock->AlphanumericCharsetOverride.Offset);
            fprintf(fptext, "Graphics charset override     : %04X:%04X\n", videoParameterControlBlock->GraphicsCharacterSetOverride.Segment, videoParameterControlBlock->GraphicsCharacterSetOverride.Offset);
            fprintf(fptext, "Secondary save pointer        : %04X:%04X\n", videoParameterControlBlock->SecondarySavePointer.Segment, videoParameterControlBlock->SecondarySavePointer.Offset);

            if (!videoParameterControlBlock->DynamicParamSaveArea.IsNull())
            {
                DynamicParameterSaveArea* dynamicParameterSaveArea = videoParameterControlBlock->DynamicParamSaveArea.ToPointer<DynamicParameterSaveArea>();
                fprintf(fptext, "\nDynamic parameter save area:\n");
                for (uint8_t i = 0; i < 16; ++i)
                {
                    fprintf(fptext, "Palette register %02i           : 0x%02X\n", i, dynamicParameterSaveArea->PaletteRegisters[i]);
                }
                fprintf(fptext, "Border color                  : 0x%02X\n", dynamicParameterSaveArea->GraphicsControllerOverscanRegister);
            }

            if (!videoParameterControlBlock->AlphanumericCharsetOverride.IsNull())
            {
                AlphaNumericCharacterSetOverride* alphaNumericCharacterSetOverride = videoParameterControlBlock->AlphanumericCharsetOverride.ToPointer<AlphaNumericCharacterSetOverride>();
                fprintf(fptext, "\nAlphanumeric character set override:\n");
                fprintf(fptext, "Character length              : 0x%02X\n", alphaNumericCharacterSetOverride->CharacterLength);
                fprintf(fptext, "Character generator RAM bank  : 0x%02X\n", alphaNumericCharacterSetOverride->CharacterGeneratorRAMBank);
                fprintf(fptext, "Number of characters          : 0x%04X\n", alphaNumericCharacterSetOverride->NumberOfCharacters);
                fprintf(fptext, "First character code in table : 0x%04X\n", alphaNumericCharacterSetOverride->FirstCharacterCodeInTable);
                fprintf(fptext, "Character font definition tbl : %04X:%04X\n", alphaNumericCharacterSetOverride->CharacterFontDefinitionTable.Segment, alphaNumericCharacterSetOverride->CharacterFontDefinitionTable.Offset);
                fprintf(fptext, "Character rows                : 0x%02X\n", alphaNumericCharacterSetOverride->NumberOfCharacterRowsDisplayed);
                int idx = 0;
                while (alphaNumericCharacterSetOverride->ApplicableVideoModes[idx] != 0xFF)
                {
                    fprintf(fptext, "Applicable mode %02i            : 0x%02X\n", idx, alphaNumericCharacterSetOverride->ApplicableVideoModes[idx]);
                    ++idx;
                }
            }
            
            if (!videoParameterControlBlock->GraphicsCharacterSetOverride.IsNull())
            {
                GraphicsCharacterSet* graphicsCharacterSet = videoParameterControlBlock->GraphicsCharacterSetOverride.ToPointer<GraphicsCharacterSet>();
                fprintf(fptext, "\nGraphics character set override:\n");
                fprintf(fptext, "Number of character rows      : 0x%02X\n", graphicsCharacterSet->NumberOfCharacterRowsDisplayed);
                fprintf(fptext, "Character length              : 0x%04X\n", graphicsCharacterSet->CharacterLength);
                fprintf(fptext, "Character font definition tbl : %04X:%04X\n", graphicsCharacterSet->CharacterFontDefinitionTable.Segment, graphicsCharacterSet->CharacterFontDefinitionTable.Offset);
                int idx = 0;
                while (graphicsCharacterSet->ApplicableVideoModes[idx] != 0xFF)
                {
                    fprintf(fptext, "Applicable mode %02i            : 0x%02X\n", idx, graphicsCharacterSet->ApplicableVideoModes[idx]);
                    ++idx;
                }
            }

            if (!videoParameterControlBlock->SecondarySavePointer.IsNull())
            {
                SecondarySavePointerTable* secondarySavePointerTable = videoParameterControlBlock->SecondarySavePointer.ToPointer<SecondarySavePointerTable>();
                fprintf(fptext, "\nSecondary save pointer table:\n");
                fprintf(fptext, "Length of table               : 0x%02X\n", secondarySavePointerTable->LengthOfTableInBytes);
                fprintf(fptext, "Display comination code talbe : %04X:%04X\n", secondarySavePointerTable->DisplayCombinationCodeTable.Segment, secondarySavePointerTable->DisplayCombinationCodeTable.Offset);
                fprintf(fptext, "Secondary Alpha charset ovr   : %04X:%04X\n", secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.Segment, secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.Offset);
                fprintf(fptext, "User palette profile table    : %04X:%04X\n", secondarySavePointerTable->UserPaletteProfileTable.Segment, secondarySavePointerTable->UserPaletteProfileTable.Offset);

                if (!secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.IsNull())
                {
                    SecondaryAlphaModeAuxillaryCharacterGeneratorTable* secondaryAlpha = secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.ToPointer<SecondaryAlphaModeAuxillaryCharacterGeneratorTable>();
                    fprintf(fptext, "\nSecondary alpha charset override:\n");
                    fprintf(fptext, "Character height              : 0x%02X\n", secondaryAlpha->CharacterHeight);
                    fprintf(fptext, "RAM bank                      : 0x%02X\n", secondaryAlpha->RamBank);
                    fprintf(fptext, "Font data                     : %04X:%04X\n", secondaryAlpha->FontData.Segment, secondaryAlpha->FontData.Offset);
                    int idx = 0;
                    while (secondaryAlpha->ApplicableModes[idx] != 0xFF)
                    {
                        fprintf(fptext, "Applicable mode %02i            : 0x%02X\n", idx, secondaryAlpha->ApplicableModes[idx]);
                        ++idx;
                    }
                }

                if (!secondarySavePointerTable->UserPaletteProfileTable.IsNull())
                {
                    PaletteProfile* paletteProfile = secondarySavePointerTable->UserPaletteProfileTable.ToPointer<PaletteProfile>();
                    fprintf(fptext, "\nUser palette profile table:\n");
                    fprintf(fptext, "Underlining                   : 0x%02X\n", paletteProfile->Underlining);
                    fprintf(fptext, "Attribute register count      : 0x%04X\n", paletteProfile->AttributeRegisterCount);
                    fprintf(fptext, "Attribute start index         : 0x%04X\n", paletteProfile->AttributeRegisterStartIndex);
                    fprintf(fptext, "Attribute register table      : %04X:%04X\n", paletteProfile->AttributeRegisterTable.Segment, paletteProfile->AttributeRegisterTable.Offset);
                    fprintf(fptext, "DAC register count            : 0x%04X\n", paletteProfile->DACRegisterCount);
                    fprintf(fptext, "DAC start index               : 0x%02X\n", paletteProfile->DACRegisterStartIndex);
                    fprintf(fptext, "DAC register table            : %04X:%04X\n", paletteProfile->DACRegisterTable.Segment, paletteProfile->DACRegisterTable.Offset);
                    int idx = 0;
                    while (paletteProfile->ApplicableModes[idx] != 0xFF)
                    {
                        fprintf(fptext, "Applicable mode %02i            : 0x%02X\n", idx, paletteProfile->ApplicableModes[idx]);
                        ++idx;
                    }
                }
            }
        }
        fprintf(fptext, "\n");
    }
}

void VGADump(FILE* fptext, FILE* fpbin, Hag::VGA::Register_t baseIOPort)
{
    using namespace Hag::VGA;
    if (fptext != nullptr) fprintf(fptext, "VGA Registers:\n\n");

    FeatureControl_t featureControl = FeatureControl::Read();
    if (fptext != nullptr) fprintf(fptext, "Feature control               : 0x%04X, 0x%02X\n", Register::FeatureControlR, featureControl);
    if (fpbin != nullptr) fwrite(&featureControl, sizeof(featureControl), 1, fpbin);

    //InputStatus1_t inputStatus1 = InputStatus1::Read(baseIOPort + 0x06);
    //fprintf(fp, "Input status 1                : 0x%02X\n", inputStatus1);
    //if (fpbin != nullptr) fwrite(&inputStatus1, sizeof(inputStatus1), 1, fpbin);

    MiscellaneousOutput_t miscellaneousOutput = MiscellaneousOutput::Read();
    if (fptext != nullptr) fprintf(fptext, "Miscellaneous output          : 0x%04X, 0x%02X\n", Register::MiscellaneousR, miscellaneousOutput);
    if (fpbin != nullptr) fwrite(&miscellaneousOutput, sizeof(miscellaneousOutput), 1, fpbin);

    //InputStatus0_t inputStatus0 = InputStatus0::Read();
    //if (fptext != nullptr) fprintf(fptext, "Input status 0                : 0x%02X\n", inputStatus0);
    //if (fpbin != nullptr) fwrite(&inputStatus0, sizeof(inputStatus0), 1, fpbin);

    //VideoSubsystemEnable_t videoSubsystemEnable = VideoSubsystemEnable::Read();
    //if (fptext != nullptr) fprintf(fptext, "Video subsystem enable        : 0x%02X\n", videoSubsystemEnable);
    //if (fpbin != nullptr) fwrite(&videoSubsystemEnable, sizeof(videoSubsystemEnable), 1, fpbin);

    DACMask_t dacMask = DACMask::Read();
    if (fptext != nullptr) fprintf(fptext, "DAC mask                      : 0x%04X, 0x%02X\n", Register::DACMask, dacMask);
    if (fpbin != nullptr) fwrite(&dacMask, sizeof(dacMask), 1, fpbin);

    DACStatus_t dacStatus = DACStatus::Read();
    if (fptext != nullptr) fprintf(fptext, "DAC status                    : 0x%04X, 0x%02X\n", Register::DACStatus, dacStatus);
    if (fpbin != nullptr) fwrite(&dacStatus, sizeof(dacStatus), 1, fpbin);

    if (fptext != nullptr) fprintf(fptext, "\nAttributes:\n");
    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::Register_t origAttribIdx = AttributeControllerIndex::Read();
    if (fptext != nullptr) fprintf(fptext, "Attribute Index:              : 0x%02X\n", origAttribIdx);
    if (fpbin != nullptr) fwrite(&origAttribIdx, sizeof(origAttribIdx), 1, fpbin);

    for (uint8_t attribIdx = 0; attribIdx < 0x10; ++attribIdx)
    {
        InputStatus1::Read(baseIOPort + 0x06);
        AttributeController::Palette_t palette = AttributeController::Palette::Read(attribIdx);
        if (fptext != nullptr) fprintf(fptext, "Palette entry %02X              : 0x%02X\n", attribIdx, palette);
        if (fpbin != nullptr) fwrite(&palette, sizeof(palette), 1, fpbin);
    }

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::AttributeMode_t attributeMode = AttributeController::AttributeMode::Read();
    if (fptext != nullptr) fprintf(fptext, "Attribute mode control        : 0x%02X\n", attributeMode);
    if (fpbin != nullptr) fwrite(&attributeMode, sizeof(attributeMode), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::BorderColor_t borderColor = AttributeController::BorderColor::Read();
    if (fptext != nullptr) fprintf(fptext, "Overscan color                : 0x%02X\n", borderColor);
    if (fpbin != nullptr) fwrite(&borderColor, sizeof(borderColor), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::ColorPlane_t colorPlane = AttributeController::ColorPlane::Read();
    if (fptext != nullptr) fprintf(fptext, "Color palette enable          : 0x%02X\n", colorPlane);
    if (fpbin != nullptr) fwrite(&colorPlane, sizeof(colorPlane), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::HorizontalPixelPanning_t horizontalPixelPanning = AttributeController::HorizontalPixelPanning::Read();
    if (fptext != nullptr) fprintf(fptext, "Horizontal PEL panning        : 0x%02X\n", horizontalPixelPanning);
    if (fpbin != nullptr) fwrite(&horizontalPixelPanning, sizeof(horizontalPixelPanning), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::PixelPadding_t pixelPadding = AttributeController::PixelPadding::Read();
    if (fptext != nullptr) fprintf(fptext, "Color select                  : 0x%02X\n", pixelPadding);
    if (fpbin != nullptr) fwrite(&pixelPadding, sizeof(pixelPadding), 1, fpbin);


    InputStatus1::Read(baseIOPort + 0x06);
    AttributeControllerIndex::Write(origAttribIdx);

    if (fptext != nullptr) fprintf(fptext, "\nCRTC:\n");
    CRTController::HorizontalTotal_t horizontalTotal = CRTController::HorizontalTotal::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Horizontal total              : 0x%02X\n", horizontalTotal);
    if (fpbin != nullptr) fwrite(&horizontalTotal, sizeof(horizontalTotal), 1, fpbin);

    CRTController::HorizontalDisplayEnd_t horizontalDisplayEnd = CRTController::HorizontalDisplayEnd::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Horizontal display enable end : 0x%02X\n", horizontalDisplayEnd);
    if (fpbin != nullptr) fwrite(&horizontalDisplayEnd, sizeof(horizontalDisplayEnd), 1, fpbin);

    CRTController::StartHorizontalBlank_t startHorizontalBlank = CRTController::StartHorizontalBlank::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Star horizontal blanking      : 0x%02X\n", startHorizontalBlank);
    if (fpbin != nullptr) fwrite(&startHorizontalBlank, sizeof(startHorizontalBlank), 1, fpbin);

    CRTController::EndHorizontalBlank_t endHorizontalBlank = CRTController::EndHorizontalBlank::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "End horizontal blanking       : 0x%02X\n", endHorizontalBlank);
    if (fpbin != nullptr) fwrite(&endHorizontalBlank, sizeof(endHorizontalBlank), 1, fpbin);

    CRTController::StartHorizontalSyncPosition_t startHorizontalSyncPosition = CRTController::StartHorizontalSyncPosition::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Start horizontal retrace pulse: 0x%02X\n", startHorizontalSyncPosition);
    if (fpbin != nullptr) fwrite(&startHorizontalSyncPosition, sizeof(startHorizontalSyncPosition), 1, fpbin);

    CRTController::EndHorizontalSyncPosition_t endHorizontalSyncPosition = CRTController::EndHorizontalSyncPosition::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "End horizontal retrace pulse  : 0x%02X\n", endHorizontalSyncPosition);
    if (fpbin != nullptr) fwrite(&endHorizontalSyncPosition, sizeof(endHorizontalSyncPosition), 1, fpbin);

    CRTController::VerticalTotal_t verticalTotal = CRTController::VerticalTotal::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Vertical total                : 0x%02X\n", verticalTotal);
    if (fpbin != nullptr) fwrite(&verticalTotal, sizeof(verticalTotal), 1, fpbin);

    CRTController::CRTCOverflow_t crtcOverflow = CRTController::CRTCOverflow::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Overflow                      : 0x%02X\n", crtcOverflow);
    if (fpbin != nullptr) fwrite(&crtcOverflow, sizeof(crtcOverflow), 1, fpbin);

    CRTController::PresetRowScan_t presetRowScan = CRTController::PresetRowScan::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Preset row scan               : 0x%02X\n", presetRowScan);
    if (fpbin != nullptr) fwrite(&presetRowScan, sizeof(presetRowScan), 1, fpbin);

    CRTController::MaximumScanLine_t maximumScanLine = CRTController::MaximumScanLine::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Maximum scan line             : 0x%02X\n", maximumScanLine);
    if (fpbin != nullptr) fwrite(&maximumScanLine, sizeof(maximumScanLine), 1, fpbin);

    CRTController::CursorStartScanLine_t cursorStartScanLine = CRTController::CursorStartScanLine::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Cursor start                  : 0x%02X\n", cursorStartScanLine);
    if (fpbin != nullptr) fwrite(&cursorStartScanLine, sizeof(cursorStartScanLine), 1, fpbin);

    CRTController::CursorEndScanLine_t cursorEndScanLine = CRTController::CursorEndScanLine::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Cursor end                    : 0x%02X\n", cursorEndScanLine);
    if (fpbin != nullptr) fwrite(&cursorEndScanLine, sizeof(cursorEndScanLine), 1, fpbin);

    CRTController::StartAddressHigh_t startAddressHigh = CRTController::StartAddressHigh::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Start address high            : 0x%02X\n", startAddressHigh);
    if (fpbin != nullptr) fwrite(&startAddressHigh, sizeof(startAddressHigh), 1, fpbin);

    CRTController::StartAddressLow_t startAddressLow = CRTController::StartAddressLow::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Start address low             : 0x%02X\n", startAddressLow);
    if (fpbin != nullptr) fwrite(&startAddressLow, sizeof(startAddressLow), 1, fpbin);

    CRTController::CursorLocationAddressHigh_t cursorLocationAddressHigh = CRTController::CursorLocationAddressHigh::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Cursor location high          : 0x%02X\n", cursorLocationAddressHigh);
    if (fpbin != nullptr) fwrite(&cursorLocationAddressHigh, sizeof(cursorLocationAddressHigh), 1, fpbin);

    CRTController::CursorLocationAddressLow_t cursorLocationAddressLow = CRTController::CursorLocationAddressLow::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Cursor location low           : 0x%02X\n", cursorLocationAddressLow);
    if (fpbin != nullptr) fwrite(&cursorLocationAddressLow, sizeof(cursorLocationAddressLow), 1, fpbin);

    CRTController::VerticalRetraceStart_t verticalRetraceStart = CRTController::VerticalRetraceStart::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Vertical retrace start        : 0x%02X\n", verticalRetraceStart);
    if (fpbin != nullptr) fwrite(&verticalRetraceStart, sizeof(verticalRetraceStart), 1, fpbin);

    CRTController::VerticalRetraceEnd_t verticalRetraceEnd = CRTController::VerticalRetraceEnd::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Vertical retrace end          : 0x%02X\n", verticalRetraceEnd);
    if (fpbin != nullptr) fwrite(&verticalRetraceEnd, sizeof(verticalRetraceEnd), 1, fpbin);

    CRTController::VerticalDisplayEnd_t verticalDisplayEnd = CRTController::VerticalDisplayEnd::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Vertical display enable end   : 0x%02X\n", verticalDisplayEnd);
    if (fpbin != nullptr) fwrite(&verticalDisplayEnd, sizeof(verticalDisplayEnd), 1, fpbin);

    CRTController::ScreenOffset_t screenOffset = CRTController::ScreenOffset::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Offset                        : 0x%02X\n", screenOffset);
    if (fpbin != nullptr) fwrite(&screenOffset, sizeof(screenOffset), 1, fpbin);

    CRTController::UnderlineLocation_t underlineLocation = CRTController::UnderlineLocation::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Underline location            : 0x%02X\n", underlineLocation);
    if (fpbin != nullptr) fwrite(&underlineLocation, sizeof(underlineLocation), 1, fpbin);

    CRTController::StartVerticalBlank_t startVerticalBlank = CRTController::StartVerticalBlank::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Start vertical blank          : 0x%02X\n", startVerticalBlank);
    if (fpbin != nullptr) fwrite(&startVerticalBlank, sizeof(startVerticalBlank), 1, fpbin);

    CRTController::EndVerticalBlank_t endVerticalBlank = CRTController::EndVerticalBlank::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "End vertical blank            : 0x%02X\n", endVerticalBlank);
    if (fpbin != nullptr) fwrite(&endVerticalBlank, sizeof(endVerticalBlank), 1, fpbin);

    CRTController::CRTCModeControl_t crtcModeControl = CRTController::CRTCModeControl::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "CRTC mode control             : 0x%02X\n", crtcModeControl);
    if (fpbin != nullptr) fwrite(&crtcModeControl, sizeof(crtcModeControl), 1, fpbin);

    CRTController::LineCompare_t lineCompare = CRTController::LineCompare::Read(baseIOPort);
    if (fptext != nullptr) fprintf(fptext, "Line compare                  : 0x%02X\n", lineCompare);
    if (fpbin != nullptr) fwrite(&lineCompare, sizeof(lineCompare), 1, fpbin);


    if (fptext != nullptr) fprintf(fptext, "\nSequencer:\n");
    Sequencer::Reset_t reset = Sequencer::Reset::Read();
    if (fptext != nullptr) fprintf(fptext, "Reset                         : 0x%02X\n", reset);
    if (fpbin != nullptr) fwrite(&reset, sizeof(reset), 1, fpbin);

    Sequencer::ClockingMode_t clockingMode = Sequencer::ClockingMode::Read();
    if (fptext != nullptr) fprintf(fptext, "Clocking mode                 : 0x%02X\n", clockingMode);
    if (fpbin != nullptr) fwrite(&clockingMode, sizeof(clockingMode), 1, fpbin);

    Sequencer::EnableWritePlane_t enableWritePlane = Sequencer::EnableWritePlane::Read();
    if (fptext != nullptr) fprintf(fptext, "Map mask                      : 0x%02X\n", enableWritePlane);
    if (fpbin != nullptr) fwrite(&enableWritePlane, sizeof(enableWritePlane), 1, fpbin);

    Sequencer::CharacterFontSelect_t characterFontSelect = Sequencer::CharacterFontSelect::Read();
    if (fptext != nullptr) fprintf(fptext, "Character map select          : 0x%02X\n", characterFontSelect);
    if (fpbin != nullptr) fwrite(&characterFontSelect, sizeof(characterFontSelect), 1, fpbin);

    Sequencer::MemoryModeControl_t memoryModeControl = Sequencer::MemoryModeControl::Read();
    if (fptext != nullptr) fprintf(fptext, "Memory mode                   : 0x%02X\n", memoryModeControl);
    if (fpbin != nullptr) fwrite(&memoryModeControl, sizeof(memoryModeControl), 1, fpbin);


    if (fptext != nullptr) fprintf(fptext, "\nGraphics controller:\n");
    GraphicsController::SetResetData_t setResetData = GraphicsController::SetResetData::Read();
    if (fptext != nullptr) fprintf(fptext, "Set/Reset                     : 0x%02X\n", setResetData);
    if (fpbin != nullptr) fwrite(&setResetData, sizeof(setResetData), 1, fpbin);

    GraphicsController::EnableSetResetData_t enableSetResetData = GraphicsController::EnableSetResetData::Read();
    if (fptext != nullptr) fprintf(fptext, "Enable Set/Reset              : 0x%02X\n", enableSetResetData);
    if (fpbin != nullptr) fwrite(&enableSetResetData, sizeof(enableSetResetData), 1, fpbin);

    GraphicsController::ColorCompare_t colorCompare = GraphicsController::ColorCompare::Read();
    if (fptext != nullptr) fprintf(fptext, "Color compare                 : 0x%02X\n", colorCompare);
    if (fpbin != nullptr) fwrite(&colorCompare, sizeof(colorCompare), 1, fpbin);

    GraphicsController::RasterOperationRotateCount_t rasterOperationRotateCount = GraphicsController::RasterOperationRotateCount::Read();
    if (fptext != nullptr) fprintf(fptext, "Data rotate                   : 0x%02X\n", rasterOperationRotateCount);
    if (fpbin != nullptr) fwrite(&rasterOperationRotateCount, sizeof(rasterOperationRotateCount), 1, fpbin);

    GraphicsController::ReadPlaneSelect_t readPlaneSelect = GraphicsController::ReadPlaneSelect::Read();
    if (fptext != nullptr) fprintf(fptext, "Read map select               : 0x%02X\n", readPlaneSelect);
    if (fpbin != nullptr) fwrite(&readPlaneSelect, sizeof(readPlaneSelect), 1, fpbin);

    GraphicsController::GraphicsControllerMode_t graphicsControllerMode = GraphicsController::GraphicsControllerMode::Read();
    if (fptext != nullptr) fprintf(fptext, "Graphics mode                 : 0x%02X\n", graphicsControllerMode);
    if (fpbin != nullptr) fwrite(&graphicsControllerMode, sizeof(graphicsControllerMode), 1, fpbin);

    GraphicsController::MemoryMapModeControl_t memoryMapModeControl = GraphicsController::MemoryMapModeControl::Read();
    if (fptext != nullptr) fprintf(fptext, "Miscellaneous                 : 0x%02X\n", memoryMapModeControl);
    if (fpbin != nullptr) fwrite(&memoryMapModeControl, sizeof(memoryMapModeControl), 1, fpbin);

    GraphicsController::ColorDontCare_t colorDontCare = GraphicsController::ColorDontCare::Read();
    if (fptext != nullptr) fprintf(fptext, "Color don't care              : 0x%02X\n", colorDontCare);
    if (fpbin != nullptr) fwrite(&colorDontCare, sizeof(colorDontCare), 1, fpbin);

    GraphicsController::BitMask_t bitMask = GraphicsController::BitMask::Read();
    if (fptext != nullptr) fprintf(fptext, "Bit mask                      : 0x%02X\n", bitMask);
    if (fpbin != nullptr) fwrite(&bitMask, sizeof(bitMask), 1, fpbin);


    if (fptext != nullptr) fprintf(fptext, "\nRAMDAC registers:\n");
    DACReadIndex::Write(0);
    for (uint16_t color = 0; color < 256; ++color)
    {
        uint8_t red = RAMDACData::Read();
        uint8_t green = RAMDACData::Read();
        uint8_t blue = RAMDACData::Read();
        if (fptext != nullptr) fprintf(fptext, "0x%02X: 0x%02X, 0x%02X, 0x%02X\n", color, red, green, blue);
        if (fpbin != nullptr)
        {
            fwrite(&red, 1, 1, fpbin);
            fwrite(&green, 1, 1, fpbin);
            fwrite(&blue, 1, 1, fpbin);
        }
    }

    if (fptext != nullptr) fprintf(fptext, "\n");
}

}