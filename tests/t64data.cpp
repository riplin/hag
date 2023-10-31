//Copyright 2023-Present riplin

#include <hag/types.h>
#include <hag/testing/mock.h>
#include <hag/system/bda.h>

uint8_t modes[] = 
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x49, 0x4A, 0x4B, 0x4C,
    0x4D, 0x4E, 0x4F, 0x52, 0x54, 0x55, 0x68, 0x69,
    0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7A, 0x7C
};

uint16_t modesCount = sizeof(modes) / sizeof(uint8_t);

uint8_t legacyModesOnly[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13
};

uint16_t legacyModesOnlyCount = sizeof(legacyModesOnly) / sizeof(uint8_t);

uint8_t vesaModesOnly[] = 
{
    0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x52,
    0x54, 0x55, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75,
    0x76, 0x77, 0x78, 0x79, 0x7A, 0x7C
};

uint16_t vesaModesOnlyCount = sizeof(vesaModesOnly) / sizeof(uint8_t);

Hag::Testing::Mock::BDAFieldsAndValues ModeSetBDA_modifiedDefault[] =
{
    { 0, 0 }
};

uint16_t ModeSetBDA_modifiedDefaultCount = sizeof(ModeSetBDA_modifiedDefault) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues);

Hag::Testing::Mock::BDAFieldsAndValues ModeSetBDA_modified_0x07_0x0F[] =
{
    {
        Hag::System::BDA::Offset::DetectedHardware,
        Hag::System::BDA::DetectedHardware::Monochrome80x25 | 0x06
    },
    {
        Hag::System::BDA::Offset::VideoModeOptions,
        Hag::System::BDA::VideoModeOptions::Memory256k | Hag::System::BDA::VideoModeOptions::Monochrome
    },
    {
        Hag::System::BDA::Offset::EGAFeatureBitSwitches,
        Hag::System::BDA::EGAFeatureBitSwitches::CGAMono80x25_2
    }
};

uint16_t ModeSetBDA_modified_0x07_0x0FCount = sizeof(ModeSetBDA_modified_0x07_0x0F) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues);

uint8_t ModeSetBDA_readDefault[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoBaseIOPort,
    Hag::System::BDA::Offset::VideoBaseIOPort + 1, //Word size
    Hag::System::BDA::Offset::EGAFeatureBitSwitches,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t ModeSetBDA_readDefaultCount = sizeof(ModeSetBDA_readDefault) / sizeof(uint8_t);

uint8_t ModeSetBDA_read_0x03[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t ModeSetBDA_read_0x03Count = sizeof(ModeSetBDA_read_0x03) / sizeof(uint8_t);

uint8_t ModeSetBDA_read_0x07_0x0F[] =
{
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoBaseIOPort,
    Hag::System::BDA::Offset::VideoBaseIOPort + 1,  //Word size
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t ModeSetBDA_read_0x07_0x0FCount = sizeof(ModeSetBDA_read_0x07_0x0F) / sizeof(uint8_t);

uint8_t VerifyBDAOrDeactivate_readDefault[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoBaseIOPort,
    Hag::System::BDA::Offset::VideoBaseIOPort + 1, //Word size
    Hag::System::BDA::Offset::VideoModeOptions,
    Hag::System::BDA::Offset::EGAFeatureBitSwitches,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t VerifyBDAOrDeactivate_readDefaultCount = sizeof(VerifyBDAOrDeactivate_readDefault) / sizeof(uint8_t);

uint8_t VerifyBDAOrDeactivate_read_0x03[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoModeOptions,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t VerifyBDAOrDeactivate_read_0x03Count = sizeof(VerifyBDAOrDeactivate_read_0x03) / sizeof(uint8_t);

uint8_t VerifyBDAOrDeactivate_read_0x07_0x0F[] =
{
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::VideoBaseIOPort,
    Hag::System::BDA::Offset::VideoBaseIOPort + 1,  //Word size
    Hag::System::BDA::Offset::VideoModeOptions,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t VerifyBDAOrDeactivate_read_0x07_0x0FCount = sizeof(VerifyBDAOrDeactivate_read_0x07_0x0F) / sizeof(uint8_t);


Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified0[] =
{
    {
        Hag::System::BDA::Offset::NumberOfScreenColumns, 40
    },
    {
        Hag::System::BDA::Offset::VideoBufferSize + 1, 0x08 //2048
    },
    {
        Hag::System::BDA::Offset::CursorEndScanLine, 14
    },
    {
        Hag::System::BDA::Offset::CursorStartScanLine, 13
    },
    {
        Hag::System::BDA::Offset::CRTModeControlRegValue,
        Hag::System::BDA::CRTModeControlRegValue::VideoEnabled |
        Hag::System::BDA::CRTModeControlRegValue::Blinking
    }
};

uint8_t SetTextModeBiosDataTest_read0[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::VideoBufferSize,
    Hag::System::BDA::Offset::CGAColorPaletteMaskSetting,
    Hag::System::BDA::Offset::RowsOnScreen,
    Hag::System::BDA::Offset::PointHeightOfCharacterMatrix,
    Hag::System::BDA::Offset::VideoModeOptions
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified1[] =
{
    {
        Hag::System::BDA::Offset::CursorEndScanLine, 14
    },
    {
        Hag::System::BDA::Offset::CursorStartScanLine, 13
    },
    {
        Hag::System::BDA::Offset::CRTModeControlRegValue,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text |
        Hag::System::BDA::CRTModeControlRegValue::Monochrome |
        Hag::System::BDA::CRTModeControlRegValue::VideoEnabled |
        Hag::System::BDA::CRTModeControlRegValue::Blinking
    }
};

uint8_t SetTextModeBiosDataTest_read1[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::NumberOfScreenColumns,
    Hag::System::BDA::Offset::VideoBufferSize,
    Hag::System::BDA::Offset::VideoBufferSize + 1,
    Hag::System::BDA::Offset::CGAColorPaletteMaskSetting,
    Hag::System::BDA::Offset::RowsOnScreen,
    Hag::System::BDA::Offset::PointHeightOfCharacterMatrix,
    Hag::System::BDA::Offset::VideoModeOptions
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified2[] =
{
    {
        Hag::System::BDA::Offset::CursorEndScanLine, 14
    },
    {
        Hag::System::BDA::Offset::CursorStartScanLine, 13
    }
};

uint8_t SetTextModeBiosDataTest_read2[] =
{
    Hag::System::BDA::Offset::DetectedHardware,
    Hag::System::BDA::Offset::NumberOfScreenColumns,
    Hag::System::BDA::Offset::VideoBufferSize,
    Hag::System::BDA::Offset::VideoBufferSize + 1,
    Hag::System::BDA::Offset::CRTModeControlRegValue,
    Hag::System::BDA::Offset::CGAColorPaletteMaskSetting,
    Hag::System::BDA::Offset::RowsOnScreen,
    Hag::System::BDA::Offset::PointHeightOfCharacterMatrix,
    Hag::System::BDA::Offset::VideoModeOptions
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified3[] =
{
    {
        Hag::System::BDA::Offset::NumberOfScreenColumns, 40
    },
    {
        Hag::System::BDA::Offset::VideoBufferSize + 1, 0x40 //16384
    },
    {
        Hag::System::BDA::Offset::CursorEndScanLine, 0x00
    },
    {
        Hag::System::BDA::Offset::CursorStartScanLine, 0x00
    },
    {
        Hag::System::BDA::Offset::CRTModeControlRegValue,
        Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics |
        Hag::System::BDA::CRTModeControlRegValue::VideoEnabled |
        Hag::System::BDA::CRTModeControlRegValue::Blinking
    },
    {
        Hag::System::BDA::Offset::PointHeightOfCharacterMatrix, 8
    }
};

uint8_t SetTextModeBiosDataTest_read3[] =
{
    0x10,
    0x4C,
    0x66,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified4[] =
{
    { 0x4A, 0x28 },
    { 0x4D, 0x40 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x65, 0x2E },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read4[] =
{
    0x10,
    0x4C,
    0x66,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified5[] =
{
    { 0x4D, 0x40 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x65, 0x1E },
    { 0x66, 0x3F },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read5[] =
{
    0x10,
    0x4A,
    0x4C,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified6[] =
{
    { 0x10, 0x36 },
    { 0x60, 0x0E },
    { 0x61, 0x0D },
    { 0x87, 0x62 }
};

uint8_t SetTextModeBiosDataTest_read6[] =
{
    0x4A,
    0x4C,
    0x4D,
    0x65,
    0x66,
    0x84,
    0x85
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified7[] =
{
    { 0x4D, 0x7D },
    { 0x60, 0x00 },
    { 0x61, 0x00 }
};

uint8_t SetTextModeBiosDataTest_read7[] =
{
    0x10,
    0x4A,
    0x4C,
    0x84,
    0x85,
    0x87
};


Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified8[] =
{
    { 0x4A, 0x00 },
    { 0x4D, 0x00 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x00 },
    { 0x85, 0x00 }
};

uint8_t SetTextModeBiosDataTest_read8[] =
{
    0x10,
    0x4C,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified9[] =
{
    { 0x4A, 0x28 },
    { 0x4D, 0x40 },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read9[] =
{
    0x10,
    0x4C,
    0x60,
    0x61,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified10[] =
{
    { 0x4D, 0x00 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x00 },
    { 0x85, 0x00 }
};

uint8_t SetTextModeBiosDataTest_read10[] =
{
    0x10,
    0x4A,
    0x4C,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified11[] =
{
    { 0x4A, 0x28 },
    { 0x4D, 0x20 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read11[] =
{
    0x10,
    0x4C,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified12[] =
{
    { 0x4D, 0x40 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read12[] =
{
    0x10,
    0x4A,
    0x4C,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified13[] =
{
    { 0x10, 0x36 },
    { 0x4D, 0x80 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x85, 0x0E },
    { 0x87, 0x62 }
};

uint8_t SetTextModeBiosDataTest_read13[] =
{
    0x4A,
    0x4C,
    0x84
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified14[] =
{
    { 0x4D, 0x80 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x85, 0x0E }
};

uint8_t SetTextModeBiosDataTest_read14[] =
{
    0x10,
    0x4A,
    0x4C,
    0x84,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified15[] =
{
    { 0x4D, 0xA0 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x1D }
};

uint8_t SetTextModeBiosDataTest_read15[] =
{
    0x10,
    0x4A,
    0x4C,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified16[] =
{
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x1D }
};

uint8_t SetTextModeBiosDataTest_read16[] =
{
    0x10,
    0x4A,
    0x4C,
    0x4D,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified17[] =
{
    { 0x4A, 0x64 },
    { 0x4D, 0xFA },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x4A },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read17[] =
{
    0x10,
    0x4C,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified18[] =
{
    { 0x4A, 0x80 },
    { 0x4C, 0xFF },
    { 0x4D, 0xFF },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x2F }
};

uint8_t SetTextModeBiosDataTest_read18[] =
{
    0x10,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified19[] =
{
    { 0x4A, 0x90 },
    { 0x4C, 0xFF },
    { 0x4D, 0xFF },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x36 }
};

uint8_t SetTextModeBiosDataTest_read19[] =
{
    0x10,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified20[] =
{
    { 0x4A, 0xA0 },
    { 0x4D, 0x01 },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x3F }
};

uint8_t SetTextModeBiosDataTest_read20[] =
{
    0x10,
    0x4C,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified21[] =
{
    { 0x60, 0x00 },
    { 0x61, 0x00 }
};

uint8_t SetTextModeBiosDataTest_read21[] =
{
    0x10,
    0x4A,
    0x4C,
    0x4D,
    0x84,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified22[] =
{
    { 0x4A, 0x84 },
    { 0x4D, 0x40 },
    { 0x84, 0x2A },
    { 0x85, 0x08 }
};

uint8_t SetTextModeBiosDataTest_read22[] =
{
    0x10,
    0x4C,
    0x60,
    0x61,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified23[] =
{
    { 0x4A, 0x84 },
    { 0x4D, 0x20 },
    { 0x60, 0x0E },
    { 0x61, 0x0D }
};

uint8_t SetTextModeBiosDataTest_read23[] =
{
    0x10,
    0x4C,
    0x84,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues SetTextModeBiosDataTest_modified24[] =
{
    { 0x4A, 0xC8 },
    { 0x4C, 0xFF },
    { 0x4D, 0xFF },
    { 0x60, 0x00 },
    { 0x61, 0x00 },
    { 0x84, 0x4B }
};

uint8_t SetTextModeBiosDataTest_read24[] =
{
    0x10,
    0x85,
    0x87
};

Hag::Testing::Mock::BDAFieldsAndValues* SetTextModeBiosDataTest_modified[] =
{
    SetTextModeBiosDataTest_modified0,
    SetTextModeBiosDataTest_modified1,
    SetTextModeBiosDataTest_modified2,
    SetTextModeBiosDataTest_modified3,
    SetTextModeBiosDataTest_modified4,
    SetTextModeBiosDataTest_modified5,
    SetTextModeBiosDataTest_modified6,
    SetTextModeBiosDataTest_modified7,
    SetTextModeBiosDataTest_modified8,
    SetTextModeBiosDataTest_modified9,
    SetTextModeBiosDataTest_modified10,
    SetTextModeBiosDataTest_modified11,
    SetTextModeBiosDataTest_modified12,
    SetTextModeBiosDataTest_modified13,
    SetTextModeBiosDataTest_modified14,
    SetTextModeBiosDataTest_modified15,
    SetTextModeBiosDataTest_modified16,
    SetTextModeBiosDataTest_modified17,
    SetTextModeBiosDataTest_modified18,
    SetTextModeBiosDataTest_modified19,
    SetTextModeBiosDataTest_modified20,
    SetTextModeBiosDataTest_modified21,
    SetTextModeBiosDataTest_modified22,
    SetTextModeBiosDataTest_modified23,
    SetTextModeBiosDataTest_modified24
};

int SetTextModeBiosDataTest_modifiedSize[] =
{
    sizeof(SetTextModeBiosDataTest_modified0) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified1) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified2) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified3) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified4) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified5) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified6) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified7) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified8) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified9) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified10) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified11) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified12) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified13) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified14) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified15) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified16) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified17) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified18) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified19) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified20) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified21) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified22) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified23) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues),
    sizeof(SetTextModeBiosDataTest_modified24) / sizeof(Hag::Testing::Mock::BDAFieldsAndValues)
};

uint8_t* SetTextModeBiosDataTest_read[] =
{
    SetTextModeBiosDataTest_read0,
    SetTextModeBiosDataTest_read1,
    SetTextModeBiosDataTest_read2,
    SetTextModeBiosDataTest_read3,
    SetTextModeBiosDataTest_read4,
    SetTextModeBiosDataTest_read5,
    SetTextModeBiosDataTest_read6,
    SetTextModeBiosDataTest_read7,
    SetTextModeBiosDataTest_read8,
    SetTextModeBiosDataTest_read9,
    SetTextModeBiosDataTest_read10,
    SetTextModeBiosDataTest_read11,
    SetTextModeBiosDataTest_read12,
    SetTextModeBiosDataTest_read13,
    SetTextModeBiosDataTest_read14,
    SetTextModeBiosDataTest_read15,
    SetTextModeBiosDataTest_read16,
    SetTextModeBiosDataTest_read17,
    SetTextModeBiosDataTest_read18,
    SetTextModeBiosDataTest_read19,
    SetTextModeBiosDataTest_read20,
    SetTextModeBiosDataTest_read21,
    SetTextModeBiosDataTest_read22,
    SetTextModeBiosDataTest_read23,
    SetTextModeBiosDataTest_read24
};

int SetTextModeBiosDataTest_readSize[] =
{
    sizeof(SetTextModeBiosDataTest_read0),
    sizeof(SetTextModeBiosDataTest_read1),
    sizeof(SetTextModeBiosDataTest_read2),
    sizeof(SetTextModeBiosDataTest_read3),
    sizeof(SetTextModeBiosDataTest_read4),
    sizeof(SetTextModeBiosDataTest_read5),
    sizeof(SetTextModeBiosDataTest_read6),
    sizeof(SetTextModeBiosDataTest_read7),
    sizeof(SetTextModeBiosDataTest_read8),
    sizeof(SetTextModeBiosDataTest_read9),
    sizeof(SetTextModeBiosDataTest_read10),
    sizeof(SetTextModeBiosDataTest_read11),
    sizeof(SetTextModeBiosDataTest_read12),
    sizeof(SetTextModeBiosDataTest_read13),
    sizeof(SetTextModeBiosDataTest_read14),
    sizeof(SetTextModeBiosDataTest_read15),
    sizeof(SetTextModeBiosDataTest_read16),
    sizeof(SetTextModeBiosDataTest_read17),
    sizeof(SetTextModeBiosDataTest_read18),
    sizeof(SetTextModeBiosDataTest_read19),
    sizeof(SetTextModeBiosDataTest_read20),
    sizeof(SetTextModeBiosDataTest_read21),
    sizeof(SetTextModeBiosDataTest_read22),
    sizeof(SetTextModeBiosDataTest_read23),
    sizeof(SetTextModeBiosDataTest_read24)
};

uint8_t SetTextModeBiosDataTest_idxToData[] =
{
    0,  // 0    0x00
    0,  // 1    0x01
    1,  // 2    0x02
    2,  // 3    0x03
    3,  // 4    0x04
    4,  // 5    0x05
    5,  // 6    0x06
    6,  // 7    0x07
    7,  // 8    0x08
    8,  // 9    0x09
    9,  // 10   0x0A
    10,  // 11   0x0B
    10,  // 12   0x0C
    11,  // 13   0x0D
    12,  // 14   0x0E
    13,  // 15   0x0F
    14,  // 16   0x10
    15,  // 17   0x11
    15,  // 18   0x12
    11,  // 19   0x13
    16,  // 20   0x49
    17,  // 21   0x4A
    17,  // 22   0x4B
    18,  // 23   0x4C
    18,  // 24   0x4D
    19,  // 25   0x4E
    20,  // 26   0x4F
    21,  // 27   0x52
    22,  // 28   0x54
    23,  // 29   0x55
    21,  // 30   0x68
    16,  // 31   0x69
    17,  // 32   0x6A
    17,  // 33   0x6B
    18,  // 34   0x6C
    18,  // 35   0x6D
    20,  // 36   0x6E
    20,  // 37   0x6F
    16,  // 38   0x70
    16,  // 39   0x71
    16,  // 40   0x72
    17,  // 41   0x73
    17,  // 42   0x74
    17,  // 43   0x75
    18,  // 44   0x76
    18,  // 45   0x77
    18,  // 46   0x78
    20,  // 47   0x79
    20,  // 48   0x7A
    24,  // 49   0x7C
};

uint8_t SetTextModeBiosDataTest_ignore[] =
{
    Hag::System::BDA::Offset::DisplayMode,
    Hag::System::BDA::Offset::NumberOfScreenColumns +1,
    Hag::System::BDA::Offset::VideoBufferOffset,
    Hag::System::BDA::Offset::VideoBufferOffset + 1,
    Hag::System::BDA::Offset::CursorPositionPage0,
    Hag::System::BDA::Offset::CursorPositionPage0 + 1,
    Hag::System::BDA::Offset::CursorPositionPage1,
    Hag::System::BDA::Offset::CursorPositionPage1 + 1,
    Hag::System::BDA::Offset::CursorPositionPage2,
    Hag::System::BDA::Offset::CursorPositionPage2 + 1,
    Hag::System::BDA::Offset::CursorPositionPage3,
    Hag::System::BDA::Offset::CursorPositionPage3 + 1,
    Hag::System::BDA::Offset::CursorPositionPage4,
    Hag::System::BDA::Offset::CursorPositionPage4 + 1,
    Hag::System::BDA::Offset::CursorPositionPage5,
    Hag::System::BDA::Offset::CursorPositionPage5 + 1,
    Hag::System::BDA::Offset::CursorPositionPage6,
    Hag::System::BDA::Offset::CursorPositionPage6 + 1,
    Hag::System::BDA::Offset::CursorPositionPage7,
    Hag::System::BDA::Offset::CursorPositionPage7 + 1,
    Hag::System::BDA::Offset::ActiveDisplayNumber,
    Hag::System::BDA::Offset::VideoBaseIOPort,
    Hag::System::BDA::Offset::VideoBaseIOPort + 1,
    Hag::System::BDA::Offset::EGAFeatureBitSwitches,
    Hag::System::BDA::Offset::VideoDisplayDataArea
};

uint16_t SetTextModeBiosDataTest_ignoreCount = sizeof(SetTextModeBiosDataTest_ignore) / sizeof(uint8_t);

extern uint8_t Characters8x8[];
extern uint8_t Characters8x14[];
extern uint8_t Characters8x16[];
extern uint8_t LegacyVideoParameterTable[][64];
extern uint8_t VESAVideoParameters132x43[];
extern uint8_t VESAVideoParameters132x25[];
extern uint8_t VESAVideoParameters640x400[];
extern uint8_t VESAVideoParameters640x480[];
extern uint8_t VESAVideoParameters800x600[];
extern uint8_t VESAVideoParameters1024x768[];
extern uint8_t VESAVideoParameters1280x1024[];
extern uint8_t VESAVideoParameters1152x864[];
extern uint8_t VESAVideoParameters1600x1200[];

struct SetTextModeBiosDataTest_DataVerify
{
    uint8_t* font;
    uint8_t* overrideTable;
    uint8_t modeDataIndex;
};

SetTextModeBiosDataTest_DataVerify SetTextModeBiosDataTest_Verify[] =
{
    { NULL, NULL, 0 },
    { Characters8x8, LegacyVideoParameterTable[23], 0 },
    { Characters8x8, LegacyVideoParameterTable[24], 0 },
    { Characters8x8, LegacyVideoParameterTable[24], 0 },
    { Characters8x8, LegacyVideoParameterTable[4], 0 },
    { Characters8x8, LegacyVideoParameterTable[5], 0 },
    { Characters8x8, LegacyVideoParameterTable[6], 0 },
    { Characters8x8, LegacyVideoParameterTable[25], 0 },
    { Characters8x16, LegacyVideoParameterTable[8], 0 },
    { Characters8x8, LegacyVideoParameterTable[9], 0 },
    { Characters8x8, LegacyVideoParameterTable[10], 0 },
    { Characters8x8, LegacyVideoParameterTable[11], 0 },
    { Characters8x8, LegacyVideoParameterTable[12], 0 },
    { Characters8x8, LegacyVideoParameterTable[13], 0 },
    { Characters8x8, LegacyVideoParameterTable[14], 0 },
    { Characters8x14, LegacyVideoParameterTable[17], 0 },
    { Characters8x14, LegacyVideoParameterTable[18], 0 },
    { Characters8x16, LegacyVideoParameterTable[26], 0 },
    { Characters8x16, LegacyVideoParameterTable[27], 0 },
    { Characters8x8, LegacyVideoParameterTable[28], 0 },
    { Characters8x16, VESAVideoParameters640x480, 46 },
    { Characters8x8, VESAVideoParameters800x600, 49 },
    { Characters8x8, VESAVideoParameters800x600, 53 },
    { Characters8x16, VESAVideoParameters1024x768, 57 },
    { Characters8x16, VESAVideoParameters1024x768, 61 },
    { Characters8x16, VESAVideoParameters1152x864, 65 },
    { Characters8x16, VESAVideoParameters1280x1024, 66 },
    { Characters8x16, VESAVideoParameters640x400, 70 },
    { Characters8x8, VESAVideoParameters132x43, 0 },
    { Characters8x8, VESAVideoParameters132x25, 1 },
    { Characters8x16, VESAVideoParameters640x400, 2 },
    { Characters8x16, VESAVideoParameters640x480, 3 },
    { Characters8x8, VESAVideoParameters800x600, 6 },
    { Characters8x8, VESAVideoParameters800x600, 10 },
    { Characters8x16, VESAVideoParameters1024x768, 14 },
    { Characters8x16, VESAVideoParameters1024x768, 18 },
    { Characters8x16, VESAVideoParameters1280x1024, 22 },
    { Characters8x16, VESAVideoParameters1280x1024, 23 },
    { Characters8x16, VESAVideoParameters640x480, 27 },
    { Characters8x16, VESAVideoParameters640x480, 27 },
    { Characters8x16, VESAVideoParameters640x480, 30 },
    { Characters8x8, VESAVideoParameters800x600, 33 },
    { Characters8x8, VESAVideoParameters800x600, 33 },
    { Characters8x8, VESAVideoParameters800x600, 36 },
    { Characters8x16, VESAVideoParameters1024x768, 39 },
    { Characters8x16, VESAVideoParameters1024x768, 39 },
    { Characters8x16, VESAVideoParameters1024x768, 43 },
    { Characters8x16, VESAVideoParameters1280x1024, 44 },
    { Characters8x16, VESAVideoParameters1280x1024, 44 },
    { Characters8x16, VESAVideoParameters1600x1200, 45 }
};
