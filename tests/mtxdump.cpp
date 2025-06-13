//Copyright 2025-Present riplin

#include <i86.h>
#include <stdio.h>
#include <string.h>
#include <hag/system/bda.h>
#include <hag/drivers/vga/vga.h>
#include <hag/system/pci.h>

#include <hag/drivers/matrox/shared/crtc/cpudata.h>         //CR22
#include <hag/drivers/matrox/shared/crtc/atadrdat.h>        //CR24
#include <hag/drivers/matrox/shared/crtc/attraddr.h>        //CR26

#include <hag/drivers/matrox/shared/crtcext/index.h>        //CRTCE Index 0x3DE
#include <hag/drivers/matrox/shared/crtcext/data.h>         //CRTCE Data  0x3DF
#include <hag/drivers/matrox/shared/crtcext/adgenext.h>     //CER0
#include <hag/drivers/matrox/shared/crtcext/hrcntext.h>     //CER1
#include <hag/drivers/matrox/shared/crtcext/vrcntext.h>     //CER2
#include <hag/drivers/matrox/shared/crtcext/misc.h>         //CER3
#include <hag/drivers/matrox/shared/crtcext/mempg.h>        //CER4
#include <hag/drivers/matrox/shared/crtcext/hrhlfcnt.h>     //CER5

#include <hag/drivers/matrox/shared/pci/ctrlap.h>

#include <hag/drivers/matrox/shared/pci/idx/curbadl.h>      //IDX04
#include <hag/drivers/matrox/shared/pci/idx/curbadh.h>      //IDX05
#include <hag/drivers/matrox/shared/pci/idx/curctrl.h>      //IDX06
#include <hag/drivers/matrox/shared/pci/idx/curcols.h>      //IDX08, IDX09, IDX0A, IDX0C, IDX0D, IDX0E, IDX10, IDX11, IDX12
#include <hag/drivers/matrox/shared/pci/idx/vrefctrl.h>     //IDX18
#include <hag/drivers/matrox/shared/pci/idx/muxctrl.h>      //IDX19
#include <hag/drivers/matrox/shared/pci/idx/pclkctrl.h>     //IDX1A
#include <hag/drivers/matrox/shared/pci/idx/genctrl.h>      //IDX1D
#include <hag/drivers/matrox/shared/pci/idx/miscctrl.h>     //IDX1E
#include <hag/drivers/matrox/shared/pci/idx/gpioctrl.h>     //IDX2A
#include <hag/drivers/matrox/shared/pci/idx/gpiodata.h>     //IDX2B
#include <hag/drivers/matrox/shared/pci/idx/syspllm.h>      //IDX2C
#include <hag/drivers/matrox/shared/pci/idx/sysplln.h>      //IDX2D
#include <hag/drivers/matrox/shared/pci/idx/syspllp.h>      //IDX2E
#include <hag/drivers/matrox/shared/pci/idx/syspllst.h>     //IDX2F
#include <hag/drivers/matrox/shared/pci/idx/zoomctrl.h>     //IDX38
#include <hag/drivers/matrox/shared/pci/idx/sensetst.h>     //IDX3A
#include <hag/drivers/matrox/shared/pci/idx/crcreml.h>      //IDX3C
#include <hag/drivers/matrox/shared/pci/idx/crcremh.h>      //IDX3D
#include <hag/drivers/matrox/shared/pci/idx/crcbitsl.h>     //IDX3E
#include <hag/drivers/matrox/shared/pci/idx/colkeyml.h>     //IDX40
#include <hag/drivers/matrox/shared/pci/idx/colkeymh.h>     //IDX41
#include <hag/drivers/matrox/shared/pci/idx/colkeyl.h>      //IDX42
#include <hag/drivers/matrox/shared/pci/idx/colkeyh.h>      //IDX43
#include <hag/drivers/matrox/shared/pci/idx/pixpllm.h>      //IDX44, IDX48, IDX4C
#include <hag/drivers/matrox/shared/pci/idx/pixplln.h>      //IDX45, IDX49, IDX4D
#include <hag/drivers/matrox/shared/pci/idx/pixpllp.h>      //IDX46, IDX4A, IDX4E
#include <hag/drivers/matrox/shared/pci/idx/pixpllst.h>     //IDX4F

void BDADump(FILE* fp, FILE* fpbin)
{
    using namespace Hag::System::BDA;
    if (fpbin != nullptr) fwrite(&SystemBDA(), 256, 1, fpbin);
    fprintf(fp, "BIOS Data Area:\n\n");
    fprintf(fp, "Display mode                  : 0x%02X\n", DisplayMode::Get());
    fprintf(fp, "Detected hardware             : 0x%02X\n", DetectedHardware::Get());
    fprintf(fp, "Number of screen columns      : 0x%04X\n", NumberOfScreenColumns::Get());
    fprintf(fp, "Video buffer size             : 0x%04X\n", VideoBufferSize::Get());
    fprintf(fp, "Video buffer offset           : 0x%04X\n", VideoBufferOffset::Get());
    
    for (uint8_t cursorPos = 0; cursorPos < 8; ++cursorPos)
    {
        fprintf(fp, "Cursor position %i             : 0x%02X, 0x%02X\n", cursorPos,
            CursorPositions::Get()[cursorPos].Column,
            CursorPositions::Get()[cursorPos].Row);
    }
    fprintf(fp, "Cursor start scanline         : 0x%02X\n", CursorScanLines::Get().Start);
    fprintf(fp, "Cursor end scanline           : 0x%02X\n", CursorScanLines::Get().End);
    fprintf(fp, "Active display number         : 0x%02X\n", ActiveDisplayNumber::Get());
    fprintf(fp, "Video Base IO port            : 0x%04X\n", VideoBaseIOPort::Get());
    fprintf(fp, "CRT mode control reg          : 0x%02X\n", CRTModeControlRegValue::Get());
    fprintf(fp, "CGA Color Palette mask        : 0x%02X\n", CGAColorPaletteMaskSetting::Get());
    fprintf(fp, "Rows on screen                : 0x%02X\n", RowsOnScreen::Get());
    fprintf(fp, "Point height of Char Matrix   : 0x%02X\n", PointHeightOfCharacterMatrix::Get());
    fprintf(fp, "Video mode options            : 0x%02X\n", VideoModeOptions::Get());
    fprintf(fp, "EGA feature bit switches      : 0x%02X\n", EGAFeatureBitSwitches::Get());
    fprintf(fp, "Video display data area       : 0x%02X\n", VideoDisplayDataArea::Get());
    fprintf(fp, "Display combination code index: 0x%02X\n", DisplayCombinationCodeTableIndex::Get());
    fprintf(fp, "Video parameter ctrl block ptr: %04X:%04X\n", VideoParameterControlBlockPointer::Get().Segment, VideoParameterControlBlockPointer::Get().Offset);
    if (!VideoParameterControlBlockPointer::Get().IsNull())
    {
        VideoParameterControlBlock* videoParameterControlBlock = VideoParameterControlBlockPointer::Get().ToPointer<VideoParameterControlBlock>();
        fprintf(fp, "\nVideo parameter control block:\n");
        fprintf(fp, "Video parameters              : %04X:%04X\n", videoParameterControlBlock->VideoParameters.Segment, videoParameterControlBlock->VideoParameters.Offset);        
        fprintf(fp, "Dynamic param save area       : %04X:%04X\n", videoParameterControlBlock->DynamicParamSaveArea.Segment, videoParameterControlBlock->DynamicParamSaveArea.Offset);
        fprintf(fp, "Alphanumeric Charset override : %04X:%04X\n", videoParameterControlBlock->AlphanumericCharsetOverride.Segment, videoParameterControlBlock->AlphanumericCharsetOverride.Offset);
        fprintf(fp, "Graphics charset override     : %04X:%04X\n", videoParameterControlBlock->GraphicsCharacterSetOverride.Segment, videoParameterControlBlock->GraphicsCharacterSetOverride.Offset);
        fprintf(fp, "Secondary save pointer        : %04X:%04X\n", videoParameterControlBlock->SecondarySavePointer.Segment, videoParameterControlBlock->SecondarySavePointer.Offset);

        if (!videoParameterControlBlock->DynamicParamSaveArea.IsNull())
        {
            DynamicParameterSaveArea* dynamicParameterSaveArea = videoParameterControlBlock->DynamicParamSaveArea.ToPointer<DynamicParameterSaveArea>();
            fprintf(fp, "\nDynamic parameter save area:\n");
            for (uint8_t i = 0; i < 16; ++i)
            {
                fprintf(fp, "Palette register %02i           : 0x%02X\n", i, dynamicParameterSaveArea->PaletteRegisters[i]);
            }
            fprintf(fp, "Border color                  : 0x%02X\n", dynamicParameterSaveArea->GraphicsControllerOverscanRegister);
        }

        if (!videoParameterControlBlock->AlphanumericCharsetOverride.IsNull())
        {
            AlphaNumericCharacterSetOverride* alphaNumericCharacterSetOverride = videoParameterControlBlock->AlphanumericCharsetOverride.ToPointer<AlphaNumericCharacterSetOverride>();
            fprintf(fp, "\nAlphanumeric character set override:\n");
            fprintf(fp, "Character length              : 0x%02X\n", alphaNumericCharacterSetOverride->CharacterLength);
            fprintf(fp, "Character generator RAM bank  : 0x%02X\n", alphaNumericCharacterSetOverride->CharacterGeneratorRAMBank);
            fprintf(fp, "Number of characters          : 0x%04X\n", alphaNumericCharacterSetOverride->NumberOfCharacters);
            fprintf(fp, "First character code in table : 0x%04X\n", alphaNumericCharacterSetOverride->FirstCharacterCodeInTable);
            fprintf(fp, "Character font definition tbl : %04X:%04X\n", alphaNumericCharacterSetOverride->CharacterFontDefinitionTable.Segment, alphaNumericCharacterSetOverride->CharacterFontDefinitionTable.Offset);
            fprintf(fp, "Character rows                : 0x%02X\n", alphaNumericCharacterSetOverride->NumberOfCharacterRowsDisplayed);
            int idx = 0;
            while (alphaNumericCharacterSetOverride->ApplicableVideoModes[idx] != 0xFF)
            {
                fprintf(fp, "Applicable mode %02i            : 0x%02X\n", alphaNumericCharacterSetOverride->ApplicableVideoModes[idx]);
                ++idx;
            }
        }
        
        if (!videoParameterControlBlock->GraphicsCharacterSetOverride.IsNull())
        {
            GraphicsCharacterSet* graphicsCharacterSet = videoParameterControlBlock->GraphicsCharacterSetOverride.ToPointer<GraphicsCharacterSet>();
            fprintf(fp, "\nGraphics character set override:\n");
            fprintf(fp, "Number of character rows      : 0x%02X\n", graphicsCharacterSet->NumberOfCharacterRowsDisplayed);
            fprintf(fp, "Character length              : 0x%04X\n", graphicsCharacterSet->CharacterLength);
            fprintf(fp, "Character font definition tbl : %04X:%04X\n", graphicsCharacterSet->CharacterFontDefinitionTable.Segment, graphicsCharacterSet->CharacterFontDefinitionTable.Offset);
            int idx = 0;
            while (graphicsCharacterSet->ApplicableVideoModes[idx] != 0xFF)
            {
                fprintf(fp, "Applicable mode %02i            : 0x%02X\n", graphicsCharacterSet->ApplicableVideoModes[idx]);
                ++idx;
            }
        }

        if (!videoParameterControlBlock->SecondarySavePointer.IsNull())
        {
            SecondarySavePointerTable* secondarySavePointerTable = videoParameterControlBlock->SecondarySavePointer.ToPointer<SecondarySavePointerTable>();
            fprintf(fp, "\nSecondary save pointer table:\n");
            fprintf(fp, "Length of table               : 0x%02X\n", secondarySavePointerTable->LengthOfTableInBytes);
            fprintf(fp, "Display comination code talbe : %04X:%04X\n", secondarySavePointerTable->DisplayCombinationCodeTable.Segment, secondarySavePointerTable->DisplayCombinationCodeTable.Offset);
            fprintf(fp, "Secondary Alpha charset ovr   : %04X:%04X\n", secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.Segment, secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.Offset);
            fprintf(fp, "User palette profile table    : %04X:%04X\n", secondarySavePointerTable->UserPaletteProfileTable.Segment, secondarySavePointerTable->UserPaletteProfileTable.Offset);

            if (!secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.IsNull())
            {
                SecondaryAlphaModeAuxillaryCharacterGeneratorTable* secondaryAlpha = secondarySavePointerTable->SecondaryAlphanumericCharacterSetOverride.ToPointer<SecondaryAlphaModeAuxillaryCharacterGeneratorTable>();
                fprintf(fp, "\nSecondary alpha charset override:\n");
                fprintf(fp, "Character height              : 0x%02X\n", secondaryAlpha->CharacterHeight);
                fprintf(fp, "RAM bank                      : 0x%02X\n", secondaryAlpha->RamBank);
                fprintf(fp, "Font data                     : %04X:%04X\n", secondaryAlpha->FontData.Segment, secondaryAlpha->FontData.Offset);
                int idx = 0;
                while (secondaryAlpha->ApplicableModes[idx] != 0xFF)
                {
                    fprintf(fp, "Applicable mode %02i            : 0x%02X\n", secondaryAlpha->ApplicableModes[idx]);
                    ++idx;
                }
            }

            if (!secondarySavePointerTable->UserPaletteProfileTable.IsNull())
            {
                PaletteProfile* paletteProfile = secondarySavePointerTable->UserPaletteProfileTable.ToPointer<PaletteProfile>();
                fprintf(fp, "\nUser palette profile table:\n");
                fprintf(fp, "Underlining                   : 0x%02X\n", paletteProfile->Underlining);
                fprintf(fp, "Attribute register count      : 0x%04X\n", paletteProfile->AttributeRegisterCount);
                fprintf(fp, "Attribute start index         : 0x%04X\n", paletteProfile->AttributeRegisterStartIndex);
                fprintf(fp, "Attribute register table      : %04X:%04X\n", paletteProfile->AttributeRegisterTable.Segment, paletteProfile->AttributeRegisterTable.Offset);
                fprintf(fp, "DAC register count            : 0x%04X\n", paletteProfile->DACRegisterCount);
                fprintf(fp, "DAC start index               : 0x%02X\n", paletteProfile->DACRegisterStartIndex);
                fprintf(fp, "DAC register table            : %04X:%04X\n", paletteProfile->DACRegisterTable.Segment, paletteProfile->DACRegisterTable.Offset);
                int idx = 0;
                while (paletteProfile->ApplicableModes[idx] != 0xFF)
                {
                    fprintf(fp, "Applicable mode %02i            : 0x%02X\n", paletteProfile->ApplicableModes[idx]);
                    ++idx;
                }
            }
        }
    }
    fprintf(fp, "\n");
}

void VGADump(FILE* fp, FILE* fpbin, Hag::VGA::Register_t baseIOPort)
{
    using namespace Hag::VGA;
    fprintf(fp, "VGA Registers:\n\n");

    FeatureControl_t featureControl = FeatureControl::Read();
    fprintf(fp, "Feature control               : 0x%02X\n", featureControl);
    if (fpbin != nullptr) fwrite(&featureControl, sizeof(featureControl), 1, fpbin);

    //InputStatus1_t inputStatus1 = InputStatus1::Read(baseIOPort + 0x06);
    //fprintf(fp, "Input status 1                : 0x%02X\n", inputStatus1);
    //if (fpbin != nullptr) fwrite(&inputStatus1, sizeof(inputStatus1), 1, fpbin);

    MiscellaneousOutput_t miscellaneousOutput = MiscellaneousOutput::Read();
    fprintf(fp, "Miscellaneous output          : 0x%02X\n", miscellaneousOutput);
    fwrite(&miscellaneousOutput, sizeof(miscellaneousOutput), 1, fpbin);

    //InputStatus0_t inputStatus0 = InputStatus0::Read();
    //fprintf(fp, "Input status 0                : 0x%02X\n", inputStatus0);
    //if (fpbin != nullptr) fwrite(&inputStatus0, sizeof(inputStatus0), 1, fpbin);

    //VideoSubsystemEnable_t videoSubsystemEnable = VideoSubsystemEnable::Read();
    //fprintf(fp, "Video subsystem enable        : 0x%02X\n", videoSubsystemEnable);
    //if (fpbin != nullptr) fwrite(&videoSubsystemEnable, sizeof(videoSubsystemEnable), 1, fpbin);

    DACMask_t dacMask = DACMask::Read();
    fprintf(fp, "DAC mask                      : 0x%02X\n", dacMask);
    if (fpbin != nullptr) fwrite(&dacMask, sizeof(dacMask), 1, fpbin);

    DACStatus_t dacStatus = DACStatus::Read();
    fprintf(fp, "DAC status                    : 0x%02X\n", dacStatus);
    if (fpbin != nullptr) fwrite(&dacStatus, sizeof(dacStatus), 1, fpbin);

    fprintf(fp, "\nAttributes:\n");
    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::Register_t origAttribIdx = AttributeControllerIndex::Read();
    fprintf(fp, "Attribute Index:              : 0x%02X\n", origAttribIdx);
    if (fpbin != nullptr) fwrite(&origAttribIdx, sizeof(origAttribIdx), 1, fpbin);

    for (uint8_t attribIdx = 0; attribIdx < 0x10; ++attribIdx)
    {
        InputStatus1::Read(baseIOPort + 0x06);
        AttributeController::Palette_t palette = AttributeController::Palette::Read(attribIdx);
        fprintf(fp, "Palette entry %02X              : 0x%02X\n", attribIdx, palette);
        if (fpbin != nullptr) fwrite(&palette, sizeof(palette), 1, fpbin);
    }

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::AttributeMode_t attributeMode = AttributeController::AttributeMode::Read();
    fprintf(fp, "Attribute mode control        : 0x%02X\n", attributeMode);
    if (fpbin != nullptr) fwrite(&attributeMode, sizeof(attributeMode), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::BorderColor_t borderColor = AttributeController::BorderColor::Read();
    fprintf(fp, "Overscan color                : 0x%02X\n", borderColor);
    if (fpbin != nullptr) fwrite(&borderColor, sizeof(borderColor), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::ColorPlane_t colorPlane = AttributeController::ColorPlane::Read();
    fprintf(fp, "Color palette enable          : 0x%02X\n", colorPlane);
    if (fpbin != nullptr) fwrite(&colorPlane, sizeof(colorPlane), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::HorizontalPixelPanning_t horizontalPixelPanning = AttributeController::HorizontalPixelPanning::Read();
    fprintf(fp, "Horizontal PEL panning        : 0x%02X\n", horizontalPixelPanning);
    if (fpbin != nullptr) fwrite(&horizontalPixelPanning, sizeof(horizontalPixelPanning), 1, fpbin);

    InputStatus1::Read(baseIOPort + 0x06);
    AttributeController::PixelPadding_t pixelPadding = AttributeController::PixelPadding::Read();
    fprintf(fp, "Color select                  : 0x%02X\n", pixelPadding);
    if (fpbin != nullptr) fwrite(&pixelPadding, sizeof(pixelPadding), 1, fpbin);


    InputStatus1::Read(baseIOPort + 0x06);
    AttributeControllerIndex::Write(origAttribIdx);

    fprintf(fp, "\nCRTC:\n");
    CRTController::HorizontalTotal_t horizontalTotal = CRTController::HorizontalTotal::Read(baseIOPort);
    fprintf(fp, "Horizontal total              : 0x%02X\n", horizontalTotal);
    if (fpbin != nullptr) fwrite(&horizontalTotal, sizeof(horizontalTotal), 1, fpbin);

    CRTController::HorizontalDisplayEnd_t horizontalDisplayEnd = CRTController::HorizontalDisplayEnd::Read(baseIOPort);
    fprintf(fp, "Horizontal display enable end : 0x%02X\n", horizontalDisplayEnd);
    if (fpbin != nullptr) fwrite(&horizontalDisplayEnd, sizeof(horizontalDisplayEnd), 1, fpbin);

    CRTController::StartHorizontalBlank_t startHorizontalBlank = CRTController::StartHorizontalBlank::Read(baseIOPort);
    fprintf(fp, "Star horizontal blanking      : 0x%02X\n", startHorizontalBlank);
    if (fpbin != nullptr) fwrite(&startHorizontalBlank, sizeof(startHorizontalBlank), 1, fpbin);

    CRTController::EndHorizontalBlank_t endHorizontalBlank = CRTController::EndHorizontalBlank::Read(baseIOPort);
    fprintf(fp, "End horizontal blanking       : 0x%02X\n", endHorizontalBlank);
    if (fpbin != nullptr) fwrite(&endHorizontalBlank, sizeof(endHorizontalBlank), 1, fpbin);

    CRTController::StartHorizontalSyncPosition_t startHorizontalSyncPosition = CRTController::StartHorizontalSyncPosition::Read(baseIOPort);
    fprintf(fp, "Start horizontal retrace pulse: 0x%02X\n", startHorizontalSyncPosition);
    if (fpbin != nullptr) fwrite(&startHorizontalSyncPosition, sizeof(startHorizontalSyncPosition), 1, fpbin);

    CRTController::EndHorizontalSyncPosition_t endHorizontalSyncPosition = CRTController::EndHorizontalSyncPosition::Read(baseIOPort);
    fprintf(fp, "End horizontal retrace pulse  : 0x%02X\n", endHorizontalSyncPosition);
    if (fpbin != nullptr) fwrite(&endHorizontalSyncPosition, sizeof(endHorizontalSyncPosition), 1, fpbin);

    CRTController::VerticalTotal_t verticalTotal = CRTController::VerticalTotal::Read(baseIOPort);
    fprintf(fp, "Vertical total                : 0x%02X\n", verticalTotal);
    if (fpbin != nullptr) fwrite(&verticalTotal, sizeof(verticalTotal), 1, fpbin);

    CRTController::CRTCOverflow_t crtcOverflow = CRTController::CRTCOverflow::Read(baseIOPort);
    fprintf(fp, "Overflow                      : 0x%02X\n", crtcOverflow);
    if (fpbin != nullptr) fwrite(&crtcOverflow, sizeof(crtcOverflow), 1, fpbin);

    CRTController::PresetRowScan_t presetRowScan = CRTController::PresetRowScan::Read(baseIOPort);
    fprintf(fp, "Preset row scan               : 0x%02X\n", presetRowScan);
    if (fpbin != nullptr) fwrite(&presetRowScan, sizeof(presetRowScan), 1, fpbin);

    CRTController::MaximumScanLine_t maximumScanLine = CRTController::MaximumScanLine::Read(baseIOPort);
    fprintf(fp, "Maximum scan line             : 0x%02X\n", maximumScanLine);
    if (fpbin != nullptr) fwrite(&maximumScanLine, sizeof(maximumScanLine), 1, fpbin);

    CRTController::CursorStartScanLine_t cursorStartScanLine = CRTController::CursorStartScanLine::Read(baseIOPort);
    fprintf(fp, "Cursor start                  : 0x%02X\n", cursorStartScanLine);
    if (fpbin != nullptr) fwrite(&cursorStartScanLine, sizeof(cursorStartScanLine), 1, fpbin);

    CRTController::CursorEndScanLine_t cursorEndScanLine = CRTController::CursorEndScanLine::Read(baseIOPort);
    fprintf(fp, "Cursor end                    : 0x%02X\n", cursorEndScanLine);
    if (fpbin != nullptr) fwrite(&cursorEndScanLine, sizeof(cursorEndScanLine), 1, fpbin);

    CRTController::StartAddressHigh_t startAddressHigh = CRTController::StartAddressHigh::Read(baseIOPort);
    fprintf(fp, "Start address high            : 0x%02X\n", startAddressHigh);
    if (fpbin != nullptr) fwrite(&startAddressHigh, sizeof(startAddressHigh), 1, fpbin);

    CRTController::StartAddressLow_t startAddressLow = CRTController::StartAddressLow::Read(baseIOPort);
    fprintf(fp, "Start address low             : 0x%02X\n", startAddressLow);
    if (fpbin != nullptr) fwrite(&startAddressLow, sizeof(startAddressLow), 1, fpbin);

    CRTController::CursorLocationAddressHigh_t cursorLocationAddressHigh = CRTController::CursorLocationAddressHigh::Read(baseIOPort);
    fprintf(fp, "Cursor location high          : 0x%02X\n", cursorLocationAddressHigh);
    if (fpbin != nullptr) fwrite(&cursorLocationAddressHigh, sizeof(cursorLocationAddressHigh), 1, fpbin);

    CRTController::CursorLocationAddressLow_t cursorLocationAddressLow = CRTController::CursorLocationAddressLow::Read(baseIOPort);
    fprintf(fp, "Cursor location low           : 0x%02X\n", cursorLocationAddressLow);
    if (fpbin != nullptr) fwrite(&cursorLocationAddressLow, sizeof(cursorLocationAddressLow), 1, fpbin);

    CRTController::VerticalRetraceStart_t verticalRetraceStart = CRTController::VerticalRetraceStart::Read(baseIOPort);
    fprintf(fp, "Vertical retrace start        : 0x%02X\n", verticalRetraceStart);
    if (fpbin != nullptr) fwrite(&verticalRetraceStart, sizeof(verticalRetraceStart), 1, fpbin);

    CRTController::VerticalRetraceEnd_t verticalRetraceEnd = CRTController::VerticalRetraceEnd::Read(baseIOPort);
    fprintf(fp, "Vertical retrace end          : 0x%02X\n", verticalRetraceEnd);
    if (fpbin != nullptr) fwrite(&verticalRetraceEnd, sizeof(verticalRetraceEnd), 1, fpbin);

    CRTController::VerticalDisplayEnd_t verticalDisplayEnd = CRTController::VerticalDisplayEnd::Read(baseIOPort);
    fprintf(fp, "Vertical display enable end   : 0x%02X\n", verticalDisplayEnd);
    if (fpbin != nullptr) fwrite(&verticalDisplayEnd, sizeof(verticalDisplayEnd), 1, fpbin);

    CRTController::ScreenOffset_t screenOffset = CRTController::ScreenOffset::Read(baseIOPort);
    fprintf(fp, "Offset                        : 0x%02X\n", screenOffset);
    if (fpbin != nullptr) fwrite(&screenOffset, sizeof(screenOffset), 1, fpbin);

    CRTController::UnderlineLocation_t underlineLocation = CRTController::UnderlineLocation::Read(baseIOPort);
    fprintf(fp, "Underline location            : 0x%02X\n", underlineLocation);
    if (fpbin != nullptr) fwrite(&underlineLocation, sizeof(underlineLocation), 1, fpbin);

    CRTController::StartVerticalBlank_t startVerticalBlank = CRTController::StartVerticalBlank::Read(baseIOPort);
    fprintf(fp, "Start vertical blank          : 0x%02X\n", startVerticalBlank);
    if (fpbin != nullptr) fwrite(&startVerticalBlank, sizeof(startVerticalBlank), 1, fpbin);

    CRTController::EndVerticalBlank_t endVerticalBlank = CRTController::EndVerticalBlank::Read(baseIOPort);
    fprintf(fp, "End vertical blank            : 0x%02X\n", endVerticalBlank);
    if (fpbin != nullptr) fwrite(&endVerticalBlank, sizeof(endVerticalBlank), 1, fpbin);

    CRTController::CRTCModeControl_t crtcModeControl = CRTController::CRTCModeControl::Read(baseIOPort);
    fprintf(fp, "CRTC mode control             : 0x%02X\n", crtcModeControl);
    if (fpbin != nullptr) fwrite(&crtcModeControl, sizeof(crtcModeControl), 1, fpbin);

    CRTController::LineCompare_t lineCompare = CRTController::LineCompare::Read(baseIOPort);
    fprintf(fp, "Line compare                  : 0x%02X\n", lineCompare);
    if (fpbin != nullptr) fwrite(&lineCompare, sizeof(lineCompare), 1, fpbin);


    fprintf(fp, "\nSequencer:\n");
    Sequencer::Reset_t reset = Sequencer::Reset::Read();
    fprintf(fp, "Reset                         : 0x%02X\n", reset);
    if (fpbin != nullptr) fwrite(&reset, sizeof(reset), 1, fpbin);

    Sequencer::ClockingMode_t clockingMode = Sequencer::ClockingMode::Read();
    fprintf(fp, "Clocking mode                 : 0x%02X\n", clockingMode);
    if (fpbin != nullptr) fwrite(&clockingMode, sizeof(clockingMode), 1, fpbin);

    Sequencer::EnableWritePlane_t enableWritePlane = Sequencer::EnableWritePlane::Read();
    fprintf(fp, "Map mask                      : 0x%02X\n", enableWritePlane);
    if (fpbin != nullptr) fwrite(&enableWritePlane, sizeof(enableWritePlane), 1, fpbin);

    Sequencer::CharacterFontSelect_t characterFontSelect = Sequencer::CharacterFontSelect::Read();
    fprintf(fp, "Character map select          : 0x%02X\n", characterFontSelect);
    if (fpbin != nullptr) fwrite(&characterFontSelect, sizeof(characterFontSelect), 1, fpbin);

    Sequencer::MemoryModeControl_t memoryModeControl = Sequencer::MemoryModeControl::Read();
    fprintf(fp, "Memory mode                   : 0x%02X\n", memoryModeControl);
    if (fpbin != nullptr) fwrite(&memoryModeControl, sizeof(memoryModeControl), 1, fpbin);


    fprintf(fp, "\nGraphics controller:\n");
    GraphicsController::SetResetData_t setResetData = GraphicsController::SetResetData::Read();
    fprintf(fp, "Set/Reset                     : 0x%02X\n", setResetData);
    if (fpbin != nullptr) fwrite(&setResetData, sizeof(setResetData), 1, fpbin);

    GraphicsController::EnableSetResetData_t enableSetResetData = GraphicsController::EnableSetResetData::Read();
    fprintf(fp, "Enable Set/Reset              : 0x%02X\n", enableSetResetData);
    if (fpbin != nullptr) fwrite(&enableSetResetData, sizeof(enableSetResetData), 1, fpbin);

    GraphicsController::ColorCompare_t colorCompare = GraphicsController::ColorCompare::Read();
    fprintf(fp, "Color compare                 : 0x%02X\n", colorCompare);
    if (fpbin != nullptr) fwrite(&colorCompare, sizeof(colorCompare), 1, fpbin);

    GraphicsController::RasterOperationRotateCount_t rasterOperationRotateCount = GraphicsController::RasterOperationRotateCount::Read();
    fprintf(fp, "Data rotate                   : 0x%02X\n", rasterOperationRotateCount);
    if (fpbin != nullptr) fwrite(&rasterOperationRotateCount, sizeof(rasterOperationRotateCount), 1, fpbin);

    GraphicsController::ReadPlaneSelect_t readPlaneSelect = GraphicsController::ReadPlaneSelect::Read();
    fprintf(fp, "Read map select               : 0x%02X\n", readPlaneSelect);
    if (fpbin != nullptr) fwrite(&readPlaneSelect, sizeof(readPlaneSelect), 1, fpbin);

    GraphicsController::GraphicsControllerMode_t graphicsControllerMode = GraphicsController::GraphicsControllerMode::Read();
    fprintf(fp, "Graphics mode                 : 0x%02X\n", graphicsControllerMode);
    if (fpbin != nullptr) fwrite(&graphicsControllerMode, sizeof(graphicsControllerMode), 1, fpbin);

    GraphicsController::MemoryMapModeControl_t memoryMapModeControl = GraphicsController::MemoryMapModeControl::Read();
    fprintf(fp, "Miscellaneous                 : 0x%02X\n", memoryMapModeControl);
    if (fpbin != nullptr) fwrite(&memoryMapModeControl, sizeof(memoryMapModeControl), 1, fpbin);

    GraphicsController::ColorDontCare_t colorDontCare = GraphicsController::ColorDontCare::Read();
    fprintf(fp, "Color don't care              : 0x%02X\n", colorDontCare);
    if (fpbin != nullptr) fwrite(&colorDontCare, sizeof(colorDontCare), 1, fpbin);

    GraphicsController::BitMask_t bitMask = GraphicsController::BitMask::Read();
    fprintf(fp, "Bit mask                      : 0x%02X\n", bitMask);
    if (fpbin != nullptr) fwrite(&bitMask, sizeof(bitMask), 1, fpbin);


    fprintf(fp, "\nRAMDAC registers:\n");
    DACReadIndex::Write(0);
    for (uint16_t color = 0; color < 256; ++color)
    {
        uint8_t red = RAMDACData::Read();
        uint8_t green = RAMDACData::Read();
        uint8_t blue = RAMDACData::Read();
        fprintf(fp, "0x%02X: 0x%02X, 0x%02X, 0x%02X\n", color, red, green, blue);
        if (fpbin != nullptr)
        {
            fwrite(&red, 1, 1, fpbin);
            fwrite(&green, 1, 1, fpbin);
            fwrite(&blue, 1, 1, fpbin);
        }
    }

    fprintf(fp, "\n");
}

struct Device
{
    uint16_t DeviceID;
    const char* Name;
};

struct ScanContext
{
    FILE* fp;
    FILE* fpbin;
    Device* devices;
    uint16_t deviceCount;
    uint16_t vendor;
};

bool pciDeviceFound = false;
uint8_t pciDeviceBus = 0;
uint8_t pciDeviceSlot = 0;
uint8_t pciDeviceFunction = 0;

bool FindDevices(uint8_t bus, uint8_t slot, uint8_t function, void* context)
{
    using namespace Hag::System;

    ScanContext* ctx = (ScanContext*)context;

    if (PCI::GetVendorId(bus, slot, function) == ctx->vendor)
    {
        if (!pciDeviceFound)
        {
            pciDeviceFound = true;
            pciDeviceBus = bus;
            pciDeviceSlot = slot;
            pciDeviceFunction = function;
        }
        fprintf(ctx->fp, "\nBus: 0x%02X, Slot: 0x%02X, Function: 0x%02X\n", bus, slot, function);
        uint16_t deviceID = PCI::GetDeviceId(bus, slot, function);
        bool found = false;
        for (uint16_t deviceIdx = 0; deviceIdx < ctx->deviceCount; ++deviceIdx)
        {
            if (ctx->devices[deviceIdx].DeviceID == deviceID)
            {
                found = true;
                fprintf(ctx->fp, "%s:\n", ctx->devices[deviceIdx].Name);
                break;
            }
        }
        if (!found)
        {
            fprintf(ctx->fp, "Unknown Matrox device 0x%04X:\n", deviceID);
        }
        for (uint8_t pciRegIdx = 0; pciRegIdx < 64; ++pciRegIdx)
        {
            uint32_t pciReg = PCI::Read32(bus, slot, function, pciRegIdx << 2);
            fprintf(ctx->fp, "0x%02X: 0x%02X, 0x%02X, 0x%02X, 0x%02X,    0x%08X\n",
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
        fprintf(ctx->fp, "\n");
    }
    return true;
}

void PCIDump(FILE* fp, FILE* fpbin, uint16_t vendor, Device* devices, uint16_t deviceCount)
{
    ScanContext context;
    context.fp = fp;
    context.fpbin = fpbin;
    context.devices = devices;
    context.deviceCount = deviceCount;
    context.vendor = vendor;

    Hag::System::PCI::ScanBus(0, FindDevices, &context);
}

void MatroxDump(FILE* fp, FILE* fpbin, uint16_t baseIOPort)
{
    using namespace Hag;
    using namespace Hag::Matrox::Shared;

    System::PCI::Device_t device = (uint16_t(pciDeviceBus) << 8) | (pciDeviceSlot << 3) | pciDeviceFunction;
    fprintf(fp, "\nMatrox specific registers:\n");

    /*
    fprintf(fp, "\nCRTC:\n");
    CRTController::CPUReadLatch_t cpuReadLatch = CRTController::CPUReadLatch::Read(baseIOPort);
    fprintf(fp, "CPU read latch                : 0x%02X\n", cpuReadLatch);
    fwrite(&cpuReadLatch, sizeof(cpuReadLatch), 1, fpbin);

    CRTController::AttributeAddressDataSelect_t attributeAddressDataSelect = CRTController::AttributeAddressDataSelect::Read(baseIOPort);
    fprintf(fp, "Attributes addres/data select : 0x%02X\n", attributeAddressDataSelect);
    fwrite(&attributeAddressDataSelect, sizeof(attributeAddressDataSelect), 1, fpbin);

    CRTController::AttributeAddress_t attributeAddress = CRTController::AttributeAddress::Read(baseIOPort);
    fprintf(fp, "Attributes address            : 0x%02X\n", attributeAddress);
    fwrite(&attributeAddress, sizeof(attributeAddress), 1, fpbin);
    */

    fprintf(fp, "\nCRTC Extension:\n");
    fprintf(fp, "Address generator extensions  : 0x%02X\n", CRTCExtension::AddressGeneratorExtensions::Read());
    fprintf(fp, "Horizontal counter extensions : 0x%02X\n", CRTCExtension::HorizontalCounterExtensions::Read());
    fprintf(fp, "Miscellaneous                 : 0x%02X\n", CRTCExtension::Miscellaneous::Read());
    fprintf(fp, "Memory page                   : 0x%02X\n", CRTCExtension::MemoryPage::Read());
    fprintf(fp, "Horizontal video half count   : 0x%02X\n", CRTCExtension::HorizontalCounterExtensions::Read());
    CRTCExtensionIndex::Write(0x06);
    fprintf(fp, "Priority request control      : 0x%02X\n", CRTCExtensionData::Read());
    CRTCExtensionIndex::Write(0x07);
    fprintf(fp, "Requester control             : 0x%02X\n", CRTCExtensionData::Read());
    CRTCExtensionIndex::Write(0x08);
    fprintf(fp, "Address extension             : 0x%02X\n", CRTCExtensionData::Read());

    if (fpbin != nullptr)
    {
        for (uint8_t idx = 0; idx < 0x08; ++idx)
        {
            CRTCExtensionIndex::Write(idx);
            CRTCExtensionData_t data = CRTCExtensionData::Read();
            fwrite(&data, sizeof(data), 1, fpbin);
        }
    }

    fprintf(fp, "\nIndexed registers:\n");
    fprintf(fp, "Cursor base address low       : 0x%02X\n", PCI::Indexed::CursorBaseAddressLow::Read(device));
    fprintf(fp, "Cursor base address high      : 0x%02X\n", PCI::Indexed::CursorBaseAddressHigh::Read(device));
    fprintf(fp, "Cursor control                : 0x%02X\n", PCI::Indexed::CursorControl::Read(device));
    fprintf(fp, "Cursor color 0 red            : 0x%02X\n", PCI::Indexed::CursorColor::ReadRed0(device));
    fprintf(fp, "Cursor color 0 green          : 0x%02X\n", PCI::Indexed::CursorColor::ReadGreen0(device));
    fprintf(fp, "Cursor color 0 blue           : 0x%02X\n", PCI::Indexed::CursorColor::ReadBlue0(device));
    fprintf(fp, "Cursor color 1 red            : 0x%02X\n", PCI::Indexed::CursorColor::ReadRed1(device));
    fprintf(fp, "Cursor color 1 green          : 0x%02X\n", PCI::Indexed::CursorColor::ReadGreen1(device));
    fprintf(fp, "Cursor color 1 blue           : 0x%02X\n", PCI::Indexed::CursorColor::ReadBlue1(device));
    fprintf(fp, "Cursor color 2 red            : 0x%02X\n", PCI::Indexed::CursorColor::ReadRed2(device));
    fprintf(fp, "Cursor color 2 green          : 0x%02X\n", PCI::Indexed::CursorColor::ReadGreen2(device));
    fprintf(fp, "Cursor color 2 blue           : 0x%02X\n", PCI::Indexed::CursorColor::ReadBlue2(device));
    fprintf(fp, "Voltage reference control     : 0x%02X\n", PCI::Indexed::VoltageReferenceControl::Read(device));
    fprintf(fp, "Multiplex control             : 0x%02X\n", PCI::Indexed::MultiplexControl::Read(device));
    fprintf(fp, "Pixel clock control           : 0x%02X\n", PCI::Indexed::PixelClockControl::Read(device));
    fprintf(fp, "General control               : 0x%02X\n", PCI::Indexed::GeneralControl::Read(device));
    fprintf(fp, "Miscellaneous control         : 0x%02X\n", PCI::Indexed::MiscellaneousControl::Read(device));
    
    fprintf(fp, "Panel mode (G400)             : 0x%02X\n", PCI::IndexedData::Read(device, 0x1F));
    fprintf(fp, "MAFC delay (G400)             : 0x%02X\n", PCI::IndexedData::Read(device, 0x20));
    
    fprintf(fp, "General purpose IO control    : 0x%02X\n", PCI::Indexed::GeneralPurposeIOControl::Read(device));
    fprintf(fp, "General purpose data          : 0x%02X\n", PCI::Indexed::GeneralPurposeIOData::Read(device));
    fprintf(fp, "System PLL M value            : 0x%02X\n", PCI::Indexed::SystemPLLM::Read(device));
    fprintf(fp, "System PLL N value            : 0x%02X\n", PCI::Indexed::SystemPLLN::Read(device));
    fprintf(fp, "System PLL P value            : 0x%02X\n", PCI::Indexed::SystemPLLP::Read(device));
    fprintf(fp, "System PLL status             : 0x%02X\n", PCI::Indexed::SystemPLLStatus::Read(device));
    fprintf(fp, "Zoom control                  : 0x%02X\n", PCI::Indexed::ZoomControl::Read(device));
    fprintf(fp, "Sense test                    : 0x%02X\n", PCI::Indexed::SenseTest::Read(device));
    fprintf(fp, "CRC remainder low             : 0x%02X\n", PCI::Indexed::CRCRemainderLow::Read(device));
    fprintf(fp, "CRC remainder high            : 0x%02X\n", PCI::Indexed::CRCRemainderHigh::Read(device));
    fprintf(fp, "CRC bit select                : 0x%02X\n", PCI::Indexed::CRCBitSelect::Read(device));
    fprintf(fp, "Color key mask low            : 0x%02X\n", PCI::Indexed::ColorKeyMaskLow::Read(device));
    fprintf(fp, "Color key mask high (MYST)    : 0x%02X\n", PCI::Indexed::ColorKeyMaskHigh::Read(device));
    fprintf(fp, "Color key low                 : 0x%02X\n", PCI::Indexed::ColorKeyLow::Read(device));
    fprintf(fp, "Color key high (MYST)         : 0x%02X\n", PCI::Indexed::ColorKeyHigh::Read(device));
    fprintf(fp, "Pixel PLL A M value           : 0x%02X\n", PCI::Indexed::PixelPLLM::ReadA(device));
    fprintf(fp, "Pixel PLL A N value           : 0x%02X\n", PCI::Indexed::PixelPLLN::ReadA(device));
    fprintf(fp, "Pixel PLL A P value           : 0x%02X\n", PCI::Indexed::PixelPLLP::ReadA(device));
    fprintf(fp, "Pixel PLL B M value           : 0x%02X\n", PCI::Indexed::PixelPLLM::ReadB(device));
    fprintf(fp, "Pixel PLL B N value           : 0x%02X\n", PCI::Indexed::PixelPLLN::ReadB(device));
    fprintf(fp, "Pixel PLL B P value           : 0x%02X\n", PCI::Indexed::PixelPLLP::ReadB(device));
    fprintf(fp, "Pixel PLL C M value           : 0x%02X\n", PCI::Indexed::PixelPLLM::ReadC(device));
    fprintf(fp, "Pixel PLL C N value           : 0x%02X\n", PCI::Indexed::PixelPLLN::ReadC(device));
    fprintf(fp, "Pixel PLL C P value           : 0x%02X\n", PCI::Indexed::PixelPLLP::ReadC(device));
    fprintf(fp, "Pixel PLL status              : 0x%02X\n", PCI::Indexed::PixelPLLStatus::Read(device));

    fprintf(fp, "Keying operating mode (G200)  : 0x%02X\n", PCI::IndexedData::Read(device, 0x51));
    fprintf(fp, "Color mask 0 red (G200)       : 0x%02X\n", PCI::IndexedData::Read(device, 0x52));
    fprintf(fp, "Color mask 0 green (G200)     : 0x%02X\n", PCI::IndexedData::Read(device, 0x53));
    fprintf(fp, "Color mask 0 blue (G200)      : 0x%02X\n", PCI::IndexedData::Read(device, 0x54));
    fprintf(fp, "Color key 0 red (G200)        : 0x%02X\n", PCI::IndexedData::Read(device, 0x55));
    fprintf(fp, "Color key 0 green (G200)      : 0x%02X\n", PCI::IndexedData::Read(device, 0x56));
    fprintf(fp, "Color key 0 blue (G200)       : 0x%02X\n", PCI::IndexedData::Read(device, 0x57));

    for (uint16_t index = 0x00; index < 0x26; index += 3)
    {
        fprintf(fp, "Cursor color %02i red (G200)    : 0x%02X\n", (index/3) + 3, PCI::IndexedData::Read(device, 0x60 + index + 0));
        fprintf(fp, "Cursor color %02i green (G200)  : 0x%02X\n", (index/3) + 3, PCI::IndexedData::Read(device, 0x60 + index + 1));
        fprintf(fp, "Cursor color %02i blue (G200)   : 0x%02X\n", (index/3) + 3, PCI::IndexedData::Read(device, 0x60 + index + 2));
    }

    if (fpbin != nullptr)
    {
        for (uint16_t index = 0; index < 0x86; ++index)
        {
            PCI::IndexedData_t data = PCI::IndexedData::Read(device, index);
            fwrite(&data, sizeof(data), 1, fpbin);
        }
    }

    fprintf(fp, "\n");
}

void MemDump(FILE* fp)
{
    using namespace Hag::VGA;

    Sequencer::MemoryModeControl_t originialMemoryModeControl = Sequencer::MemoryModeControl::Read();
    Sequencer::MemoryModeControl::Write(Sequencer::MemoryModeControl::ExtendedMemoryAddress |
                                        Sequencer::MemoryModeControl::SequentialAddressingMode);
    
    GraphicsController::MemoryMapModeControl_t originalMemoryMapModeControl = GraphicsController::MemoryMapModeControl::Read();
    GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);

    GraphicsController::ReadPlaneSelect_t originalPlane = GraphicsController::ReadPlaneSelect::Read();

    for (uint8_t plane = 0; plane < 4; ++plane)
    {
        uint8_t* ptr = (uint8_t*)(0xA0000);
        GraphicsController::ReadPlaneSelect::Write(plane);
        fwrite(ptr, 0x10000, 1, fp);
    }
    GraphicsController::ReadPlaneSelect::Write(originalPlane);
    Sequencer::MemoryModeControl::Write(originialMemoryModeControl);
    GraphicsController::MemoryMapModeControl::Write(originalMemoryMapModeControl);
}

void BIOSDump()
{
    uint8_t* ptr = (uint8_t*)(0xC0000);
    if ((ptr[0] != 0x55) && (ptr[1] != 0xAA))
        return;
    uint32_t size = uint32_t(ptr[2]) * 512;
    char filename[50];
    sprintf(filename, "BIOS%02X.bin", ptr[5]);
    FILE* fp = fopen(filename, "wb");

    fwrite(ptr, 1, size, fp);

    fclose(fp);
}


uint8_t modes[] = 
{
            //          text/   text    pixel   pixel       colors  display screen
            //          grph    resol   box     resolution          pages    addr
    0x00,   // 00h =    T       40x25   9x16    360x400      16       8     B800
    0x01,   // 01h =    T       40x25   9x16    360x400      16       8     B800
    0x02,   // 02h =    T       80x25   9x16    720x400      16       8     B800
    0x03,   // 03h =    T       80x25   9x16    720x400      16       8     B800
            //     =    T       80x50   8x8     640x400      16       4     B800
    0x04,   // 04h =    G       40x25   8x8     320x200       4       .     B800
    0x05,   // 05h =    G       40x25   8x8     320x200       4       .     B800
    0x06,   // 06h =    G       80x25   8x8     640x200       2       .     B800
    0x07,   // 07h =    T       80x25   9x16    720x400     mono      .     B000
            // 08h =    ?
            // 09h =    ?
            // 0Ah =    ?
            // 0Bh =    Reserved
            // 0Ch =    Reserved
    0x0D,   // 0Dh =    G       40x25   8x8     320x200      16       8     A000
    0x0E,   // 0Eh =    G       80x25   8x8     640x200      16       4     A000
    0x0F,   // 0Fh =    G       80x25   8x14    640x350     mono      2     A000
    0x10,   // 10h =    G         .      .      640x350      16       .     A000
    0x11,   // 11h =    G       80x30   8x16    640x480     mono      .     A000
    0x12,   // 12h =    G       80x30   8x16    640x480      16/256K  .     A000
    0x13    // 13h =    G       40x25   8x8     320x200     256/256K  .     A000
};

uint16_t modesCount = sizeof(modes);

int main(int argc, char** argv)
{
    using namespace Hag::System;
    REGPACK r;
    memset(&r, 0, sizeof(r));

    static Device devices[] =
    {
        { 0x0519, "Matrox Millennium 2064W PCI" },
        { 0x051A, "Matrox Mystique 1064SG PCI", },
        { 0x051B, "Matrox Millennium II 2164W PCI" },
        { 0x051F, "Matrox Millennium II 2164W AGP" },
        { 0x0520, "Matrox G200 PCI" },
        { 0x0521, "Matrox G200 AGP" },
        { 0x0525, "Matrox G400/G450 AGP" },
        { 0x0D10, "Matrox Ultima/Impression PCI" },
        { 0x1001, "Matrox Productiva G100 AGP" },
        { 0x1000, "Matrox Productiva G100 PCI" }
    };

    for (uint16_t modesIdx = 0; modesIdx < modesCount; ++modesIdx)
    {
        uint8_t mode = modes[modesIdx];

        memset(&r, 0, sizeof(r));
        r.h.ah = 0x00;
        r.h.al = 0x03;
        intr(0x10, &r);

        //Reset palette to prevent garbage from contaminating our dumps
        Hag::VGA::DACWriteIndex::Write(0);
        for (uint16_t i = 0; i < 256; ++i)
        {
            Hag::VGA::RAMDACData::Write(0);
            Hag::VGA::RAMDACData::Write(0);
            Hag::VGA::RAMDACData::Write(0);
        }
    
        memset(&r, 0, sizeof(r));
        r.h.ah = 0x00;
        r.h.al = mode;
        intr(0x10, &r);
        char filename[50];
        FILE* fp = nullptr;
        FILE* fpbin = nullptr;

        sprintf(filename, "mode00%02X.txt", mode);
        fp = fopen(filename, "w");
        
        //sprintf(filename, "bda00%02X.txt", mode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "bda00%02X.bin", mode);
        //fpbin = fopen(filename, "wb");
        BDADump(fp, fpbin);
        //fclose(fp);
        //fclose(fpbin);
        
        //sprintf(filename, "vga00%02X.txt", mode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "vga00%02X.bin", mode);
        //fpbin = fopen(filename, "wb");
        VGADump(fp, fpbin, BDA::VideoBaseIOPort::Get());
        //fclose(fp);
        //fclose(fpbin);

        //sprintf(filename, "pci00%02X.txt", mode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "pci00%02X.bin", mode);
        //fpbin = fopen(filename, "wb");
        PCIDump(fp, fpbin, 0x102B, devices, sizeof(devices) / sizeof(Device));
        //fclose(fp);
        //fclose(fpbin);

        if (pciDeviceFound)
        {
            //sprintf(filename, "mtx00%02X.txt", mode);
            //fp = fopen(filename, "w");
            //sprintf(filename, "mtx00%02X.bin", mode);
            //fpbin = fopen(filename, "wb");
            MatroxDump(fp, fpbin, BDA::VideoBaseIOPort::Get());
            //fclose(fp);
            //fclose(fpbin);
        }

        //sprintf(filename, "mem00%02X.bin", mode);
        //fp = fopen(filename, "wb");
        //MemDump(fp);
        //fclose(fp);

        fclose(fp);

    }

    uint16_t vesaModes[] =
    {
        0x100, //640x400x256
        0x101, //640x480x256
        0x102, //800x600x16
        0x103, //800x600x256
        0x104, //1024x768x16
        0x105, //1024x768x256
        0x106, //1280x1024x16
        0x107, //1280x1024x256
        0x108, //80x60 text
        0x109, //132x25 text
        0x10A, //132x43 text
        0x10B, //132x50 text
        0x10C, //132x60 text
        0x10D, //320x200x32K
        0x10E, //320x200x64K
        0x10F, //320x200x16M
        0x110, //640x480x32K
        0x111, //640x480x64K
        0x112, //640x480x16M
        0x113, //800x600x32K
        0x114, //800x600x64K
        0x115, //800x600x16M
        0x116, //1024x768x32K
        0x117, //1024x768x64K
        0x118, //1024x768x16M
        0x119, //1280x1024x32K
        0x11A, //1280x1024x64K
        0x11B, //1280x1024x16M
        0x11C, //1600x1200x256
        0x11D, //1600x1200x32k
        0x11E, //1600x1200x64k
        0x120, //1600x1200x256 //Duplicates??
        0x121, //1600x1200x32K
        0x122, //1600x1200x64K
    };

    for (uint8_t modeIdx = 0; modeIdx < (sizeof(vesaModes) / sizeof(uint16_t)); ++modeIdx)
    {
        uint16_t vesaMode = vesaModes[modeIdx];
        
        memset(&r, 0, sizeof(r));
        r.h.ah = 0x00;
        r.h.al = 0x03;
        intr(0x10, &r);
    
        //Reset palette to prevent garbage from contaminating our dumps
        Hag::VGA::DACWriteIndex::Write(0);
        for (uint16_t i = 0; i < 256; ++i)
        {
            Hag::VGA::RAMDACData::Write(0);
            Hag::VGA::RAMDACData::Write(0);
            Hag::VGA::RAMDACData::Write(0);
        }
    
        memset(&r, 0, sizeof(r));
        r.w.ax = 0x4F02;
        r.w.bx = vesaMode;
        intr(0x10, &r);

        char filename[50];
        FILE* fp = nullptr;
        FILE* fpbin = nullptr;

        sprintf(filename, "mode%04X.txt", vesaMode);
        fp = fopen(filename, "w");

        //sprintf(filename, "bda%04X.txt", vesaMode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "bda%04X.bin", vesaMode);
        //fpbin = fopen(filename, "wb");
        BDADump(fp, fpbin);
        //fclose(fp);
        //fclose(fpbin);

        //sprintf(filename, "vesa%04X.txt", vesaMode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "vesa%04X.bin", vesaMode);
        //fpbin = fopen(filename, "wb");
        VGADump(fp, fpbin, BDA::VideoBaseIOPort::Get());
        //fclose(fp);
        //fclose(fpbin);

        //sprintf(filename, "pci%04X.txt", vesaMode);
        //fp = fopen(filename, "w");
        //sprintf(filename, "pci%04X.bin", vesaMode);
        //fpbin = fopen(filename, "wb");
        PCIDump(fp, fpbin, 0x102B, devices, sizeof(devices) / sizeof(Device));
        //fclose(fp);
        //fclose(fpbin);

        if (pciDeviceFound)
        {
            //sprintf(filename, "mtx%04X.txt", vesaMode);
            //fp = fopen(filename, "w");
            //sprintf(filename, "mtx%04X.bin", vesaMode);
            //fpbin = fopen(filename, "wb");
            MatroxDump(fp, fpbin, BDA::VideoBaseIOPort::Get());
            //fclose(fp);
            //fclose(fpbin);
        }

        //sprintf(filename, "mem%04X.bin", vesaMode);
        //fp = fopen(filename, "wb");
        //MemDump(fp);
        //fclose(fp);

        fclose(fp);

    }

    r.h.ah = 0x00;
    r.h.al = 0x03;
    intr(0x10, &r);

    //BIOSDump();

    return 0;
}
