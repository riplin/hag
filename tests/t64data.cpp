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

uint16_t ApplyVideoParametersTest_ignorePorts[] =
{ 
    0x3B4,
    0x3B5,
    0x3BA,
    0x3C0,
    0x3C4,
    0x3C5,
    0x3CC,
    0x3D4,
    0x3D5,
    0x3DA
};

uint16_t ApplyVideoParametersTest_ignorePortsCount = sizeof(ApplyVideoParametersTest_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_ignoreIndexedPorts[] =
{
    { 0x03C0, 0x11 },
    { 0x03C0, 0x14 },
    { 0x03C0, 0x15 },
    { 0x03C5, 0x03 },
    { 0x03D5, 0x08 },
    { 0x03D5, 0x0C },
    { 0x03D5, 0x0D },
    { 0x03D5, 0x0E },
    { 0x03D5, 0x0F },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 }
};

uint16_t ApplyVideoParametersTest_ignoreIndexedPortsCount = sizeof(ApplyVideoParametersTest_ignoreIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndex);

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts0[] =
{
    { 0x0000, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts1[] =
{
    { 0x03C2, 0x67 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts2[] =
{
    { 0x03C2, 0x63 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts3[] =
{
    { 0x03C2, 0x66 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts4[] =
{
    { 0x03C2, 0x00 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts5[] =
{
    { 0x03C2, 0x23 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts6[] =
{
    { 0x03C2, 0x62 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts7[] =
{
    { 0x03C2, 0xA2 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts8[] =
{
    { 0x03C2, 0xA3 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts9[] =
{
    { 0x03C2, 0xE3 },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts10[] =
{
    { 0x03C2, 0xEF },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts11[] =
{
    { 0x03C2, 0x6F },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue ApplyVideoParametersTest_modifiedPorts12[] =
{
    { 0x03C2, 0x2F },
    { 0x03CE, 0x00 },
    { 0x03CF, 0x00 }
};

Hag::Testing::Mock::PortAndValue* ApplyVideoParametersTest_modifiedPorts[] =
{
    ApplyVideoParametersTest_modifiedPorts0, //0
    ApplyVideoParametersTest_modifiedPorts1, //1
    ApplyVideoParametersTest_modifiedPorts1, //2
    ApplyVideoParametersTest_modifiedPorts1, //3
    ApplyVideoParametersTest_modifiedPorts2, //4
    ApplyVideoParametersTest_modifiedPorts2, //5
    ApplyVideoParametersTest_modifiedPorts2, //6
    ApplyVideoParametersTest_modifiedPorts3, //7
    ApplyVideoParametersTest_modifiedPorts2, //8
    ApplyVideoParametersTest_modifiedPorts4, //9
    ApplyVideoParametersTest_modifiedPorts5, //10
    ApplyVideoParametersTest_modifiedPorts6, //11
    ApplyVideoParametersTest_modifiedPorts2, //12
    ApplyVideoParametersTest_modifiedPorts2, //13
    ApplyVideoParametersTest_modifiedPorts2, //14
    ApplyVideoParametersTest_modifiedPorts7, //15
    ApplyVideoParametersTest_modifiedPorts8, //16
    ApplyVideoParametersTest_modifiedPorts9, //17
    ApplyVideoParametersTest_modifiedPorts9, //18
    ApplyVideoParametersTest_modifiedPorts2, //19
    ApplyVideoParametersTest_modifiedPorts10, //20
    ApplyVideoParametersTest_modifiedPorts10, //21
    ApplyVideoParametersTest_modifiedPorts10, //22
    ApplyVideoParametersTest_modifiedPorts10, //23
    ApplyVideoParametersTest_modifiedPorts10, //24
    ApplyVideoParametersTest_modifiedPorts10, //25
    ApplyVideoParametersTest_modifiedPorts10, //26
    ApplyVideoParametersTest_modifiedPorts10, //27
    ApplyVideoParametersTest_modifiedPorts11, //28
    ApplyVideoParametersTest_modifiedPorts11, //29
    ApplyVideoParametersTest_modifiedPorts10, //30
    ApplyVideoParametersTest_modifiedPorts10, //31
    ApplyVideoParametersTest_modifiedPorts10, //32
    ApplyVideoParametersTest_modifiedPorts10, //33
    ApplyVideoParametersTest_modifiedPorts10, //34
    ApplyVideoParametersTest_modifiedPorts10, //35
    ApplyVideoParametersTest_modifiedPorts10, //36
    ApplyVideoParametersTest_modifiedPorts10, //37
    ApplyVideoParametersTest_modifiedPorts10, //38
    ApplyVideoParametersTest_modifiedPorts10, //39
    ApplyVideoParametersTest_modifiedPorts10, //40
    ApplyVideoParametersTest_modifiedPorts10, //41
    ApplyVideoParametersTest_modifiedPorts10, //42
    ApplyVideoParametersTest_modifiedPorts10, //43
    ApplyVideoParametersTest_modifiedPorts10, //44
    ApplyVideoParametersTest_modifiedPorts10, //45
    ApplyVideoParametersTest_modifiedPorts10, //46
    ApplyVideoParametersTest_modifiedPorts10, //47
    ApplyVideoParametersTest_modifiedPorts10, //48
    ApplyVideoParametersTest_modifiedPorts12 //49
};

int ApplyVideoParametersTest_modifiedPortsCount[] =
{
    sizeof(ApplyVideoParametersTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //0
    sizeof(ApplyVideoParametersTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //1
    sizeof(ApplyVideoParametersTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //2
    sizeof(ApplyVideoParametersTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //3
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //4
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //5
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //6
    sizeof(ApplyVideoParametersTest_modifiedPorts3) / sizeof(Hag::Testing::Mock::PortAndValue), //7
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //8
    sizeof(ApplyVideoParametersTest_modifiedPorts4) / sizeof(Hag::Testing::Mock::PortAndValue), //9
    sizeof(ApplyVideoParametersTest_modifiedPorts5) / sizeof(Hag::Testing::Mock::PortAndValue), //10
    sizeof(ApplyVideoParametersTest_modifiedPorts6) / sizeof(Hag::Testing::Mock::PortAndValue), //11
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //12
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //13
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //14
    sizeof(ApplyVideoParametersTest_modifiedPorts7) / sizeof(Hag::Testing::Mock::PortAndValue), //15
    sizeof(ApplyVideoParametersTest_modifiedPorts8) / sizeof(Hag::Testing::Mock::PortAndValue), //16
    sizeof(ApplyVideoParametersTest_modifiedPorts9) / sizeof(Hag::Testing::Mock::PortAndValue), //17
    sizeof(ApplyVideoParametersTest_modifiedPorts9) / sizeof(Hag::Testing::Mock::PortAndValue), //18
    sizeof(ApplyVideoParametersTest_modifiedPorts2) / sizeof(Hag::Testing::Mock::PortAndValue), //19
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //20
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //21
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //22
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //23
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //24
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //25
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //26
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //27
    sizeof(ApplyVideoParametersTest_modifiedPorts11) / sizeof(Hag::Testing::Mock::PortAndValue), //28
    sizeof(ApplyVideoParametersTest_modifiedPorts11) / sizeof(Hag::Testing::Mock::PortAndValue), //29
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //30
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //31
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //32
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //33
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //34
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //35
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //36
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //37
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //38
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //39
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //40
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //41
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //42
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //43
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //44
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //45
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //46
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //47
    sizeof(ApplyVideoParametersTest_modifiedPorts10) / sizeof(Hag::Testing::Mock::PortAndValue), //48
    sizeof(ApplyVideoParametersTest_modifiedPorts12) / sizeof(Hag::Testing::Mock::PortAndValue) //49
};

uint16_t ApplyVideoParametersTest_readPorts[] = { 0 };
int ApplyVideoParametersTest_readPortsCount = sizeof(ApplyVideoParametersTest_readPorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts0[] =
{
    { 0x0000, 0x00, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts1[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x28 },
    { 0x03C5, 0x04, 0x02 },
    { 0x03D5, 0x00, 0x2D },
    { 0x03D5, 0x01, 0x27 },
    { 0x03D5, 0x02, 0x28 },
    { 0x03D5, 0x03, 0x90 },
    { 0x03D5, 0x04, 0x2B },
    { 0x03D5, 0x05, 0xA0 },
    { 0x03D5, 0x13, 0x14 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts2[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x20 },
    { 0x03C5, 0x04, 0x02 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts3[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x29 },
    { 0x03C5, 0x04, 0x02 },
    { 0x03D5, 0x00, 0x2D },
    { 0x03D5, 0x01, 0x27 },
    { 0x03D5, 0x02, 0x28 },
    { 0x03D5, 0x03, 0x90 },
    { 0x03D5, 0x04, 0x2B },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0xC1 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x13, 0x14 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x17, 0xA2 },
    { 0x03C0, 0x01, 0x13 },
    { 0x03C0, 0x02, 0x15 },
    { 0x03C0, 0x03, 0x17 },
    { 0x03C0, 0x04, 0x02 },
    { 0x03C0, 0x05, 0x04 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x12, 0x03 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts4[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x01 },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0xC1 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x17, 0xC2 },
    { 0x03C0, 0x01, 0x17 },
    { 0x03C0, 0x02, 0x17 },
    { 0x03C0, 0x03, 0x17 },
    { 0x03C0, 0x04, 0x17 },
    { 0x03C0, 0x05, 0x17 },
    { 0x03C0, 0x06, 0x17 },
    { 0x03C0, 0x07, 0x17 },
    { 0x03C0, 0x08, 0x17 },
    { 0x03C0, 0x09, 0x17 },
    { 0x03C0, 0x0A, 0x17 },
    { 0x03C0, 0x0B, 0x17 },
    { 0x03C0, 0x0C, 0x17 },
    { 0x03C0, 0x0D, 0x17 },
    { 0x03C0, 0x0E, 0x17 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x12, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts5[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x20 },
    { 0x03C5, 0x04, 0x02 },
    { 0x03B5, 0x00, 0x5F },
    { 0x03B5, 0x01, 0x4F },
    { 0x03B5, 0x02, 0x50 },
    { 0x03B5, 0x03, 0x82 },
    { 0x03B5, 0x04, 0x55 },
    { 0x03B5, 0x05, 0x81 },
    { 0x03B5, 0x06, 0xBF },
    { 0x03B5, 0x07, 0x1F },
    { 0x03B5, 0x08, 0x00 },
    { 0x03B5, 0x09, 0x4F },
    { 0x03B5, 0x0A, 0x0D },
    { 0x03B5, 0x0B, 0x0E },
    { 0x03B5, 0x0C, 0x00 },
    { 0x03B5, 0x0D, 0x00 },
    { 0x03B5, 0x0E, 0x00 },
    { 0x03B5, 0x0F, 0x00 },
    { 0x03B5, 0x10, 0x9C },
    { 0x03B5, 0x11, 0x8E },
    { 0x03B5, 0x12, 0x8F },
    { 0x03B5, 0x13, 0x28 },
    { 0x03B5, 0x14, 0x0F },
    { 0x03B5, 0x15, 0x96 },
    { 0x03B5, 0x16, 0xB9 },
    { 0x03B5, 0x17, 0xA3 },
    { 0x03C0, 0x00, 0x16 },
    { 0x03C0, 0x08, 0x14 },
    { 0x03C0, 0x0E, 0x11 },
    { 0x03C0, 0x0F, 0x13 },
    { 0x03C0, 0x10, 0x09 },
    { 0x03C0, 0x18, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts6[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 },
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts7[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x20 },
    { 0x03C5, 0x02, 0x00 },
    { 0x03C5, 0x04, 0x00 },
    { 0x03D5, 0x00, 0x00 },
    { 0x03D5, 0x01, 0x00 },
    { 0x03D5, 0x02, 0x00 },
    { 0x03D5, 0x03, 0x00 },
    { 0x03D5, 0x04, 0x00 },
    { 0x03D5, 0x05, 0x00 },
    { 0x03D5, 0x06, 0x00 },
    { 0x03D5, 0x07, 0x00 },
    { 0x03D5, 0x09, 0x00 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x00 },
    { 0x03D5, 0x11, 0x00 },
    { 0x03D5, 0x12, 0x00 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0x00 },
    { 0x03D5, 0x16, 0x00 },
    { 0x03D5, 0x17, 0x00 },
    { 0x03D5, 0x18, 0x00 },
    { 0x03C0, 0x01, 0x00 },
    { 0x03C0, 0x02, 0x00 },
    { 0x03C0, 0x03, 0x00 },
    { 0x03C0, 0x04, 0x00 },
    { 0x03C0, 0x05, 0x00 },
    { 0x03C0, 0x06, 0x00 },
    { 0x03C0, 0x07, 0x00 },
    { 0x03C0, 0x08, 0x00 },
    { 0x03C0, 0x09, 0x00 },
    { 0x03C0, 0x0A, 0x00 },
    { 0x03C0, 0x0B, 0x00 },
    { 0x03C0, 0x0C, 0x00 },
    { 0x03C0, 0x0D, 0x00 },
    { 0x03C0, 0x0E, 0x00 },
    { 0x03C0, 0x0F, 0x00 },
    { 0x03C0, 0x10, 0x00 },
    { 0x03C0, 0x12, 0x00 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts8[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x20 },
    { 0x03C5, 0x02, 0x00 },
    { 0x03C5, 0x04, 0x03 },
    { 0x03D5, 0x00, 0x37 },
    { 0x03D5, 0x01, 0x27 },
    { 0x03D5, 0x02, 0x2D },
    { 0x03D5, 0x03, 0x37 },
    { 0x03D5, 0x04, 0x31 },
    { 0x03D5, 0x05, 0x15 },
    { 0x03D5, 0x06, 0x04 },
    { 0x03D5, 0x07, 0x11 },
    { 0x03D5, 0x09, 0x47 },
    { 0x03D5, 0x0A, 0x06 },
    { 0x03D5, 0x0B, 0x07 },
    { 0x03D5, 0x10, 0xE1 },
    { 0x03D5, 0x11, 0x24 },
    { 0x03D5, 0x12, 0xC7 },
    { 0x03D5, 0x13, 0x14 },
    { 0x03D5, 0x14, 0x08 },
    { 0x03D5, 0x15, 0xE0 },
    { 0x03D5, 0x16, 0xF0 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x08 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts9[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x29 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x01, 0x00 },
    { 0x03C0, 0x02, 0x00 },
    { 0x03C0, 0x03, 0x00 },
    { 0x03C0, 0x04, 0x00 },
    { 0x03C0, 0x05, 0x00 },
    { 0x03C0, 0x06, 0x00 },
    { 0x03C0, 0x07, 0x00 },
    { 0x03C0, 0x08, 0x00 },
    { 0x03C0, 0x09, 0x00 },
    { 0x03C0, 0x0A, 0x00 },
    { 0x03C0, 0x0B, 0x00 },
    { 0x03C0, 0x0C, 0x00 },
    { 0x03C0, 0x0D, 0x00 },
    { 0x03C0, 0x0E, 0x00 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 },
    { 0x03C0, 0x16, 0x0F }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts10[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x29 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x00, 0x2D },
    { 0x03D5, 0x01, 0x27 },
    { 0x03D5, 0x02, 0x28 },
    { 0x03D5, 0x03, 0x90 },
    { 0x03D5, 0x04, 0x2B },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0xC0 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x13, 0x14 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts11[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0xC0 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts12[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03B5, 0x00, 0x5F },
    { 0x03B5, 0x01, 0x4F },
    { 0x03B5, 0x02, 0x50 },
    { 0x03B5, 0x03, 0x82 },
    { 0x03B5, 0x04, 0x54 },
    { 0x03B5, 0x05, 0x80 },
    { 0x03B5, 0x06, 0xBF },
    { 0x03B5, 0x07, 0x1F },
    { 0x03B5, 0x08, 0x00 },
    { 0x03B5, 0x09, 0x40 },
    { 0x03B5, 0x0A, 0x00 },
    { 0x03B5, 0x0B, 0x00 },
    { 0x03B5, 0x0C, 0x00 },
    { 0x03B5, 0x0D, 0x00 },
    { 0x03B5, 0x0E, 0x00 },
    { 0x03B5, 0x0F, 0x00 },
    { 0x03B5, 0x10, 0x83 },
    { 0x03B5, 0x11, 0x85 },
    { 0x03B5, 0x12, 0x5D },
    { 0x03B5, 0x13, 0x28 },
    { 0x03B5, 0x14, 0x0F },
    { 0x03B5, 0x15, 0x63 },
    { 0x03B5, 0x16, 0xBA },
    { 0x03B5, 0x17, 0xE3 },
    { 0x03C0, 0x00, 0x16 },
    { 0x03C0, 0x05, 0x13 },
    { 0x03C0, 0x08, 0x0A },
    { 0x03C0, 0x0B, 0x11 },
    { 0x03C0, 0x18, 0x0E }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts13[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x83 },
    { 0x03D5, 0x11, 0x85 },
    { 0x03D5, 0x12, 0x5D },
    { 0x03D5, 0x14, 0x0F },
    { 0x03D5, 0x15, 0x63 },
    { 0x03D5, 0x16, 0xBA },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts14[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x06, 0x0B },
    { 0x03D5, 0x07, 0x3E },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0xEA },
    { 0x03D5, 0x11, 0x8C },
    { 0x03D5, 0x12, 0xDF },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0xE7 },
    { 0x03D5, 0x16, 0x04 },
    { 0x03D5, 0x17, 0xC3 },
    { 0x03C0, 0x01, 0x3F },
    { 0x03C0, 0x02, 0x3F },
    { 0x03C0, 0x03, 0x3F },
    { 0x03C0, 0x04, 0x3F },
    { 0x03C0, 0x05, 0x3F },
    { 0x03C0, 0x06, 0x3F },
    { 0x03C0, 0x07, 0x3F },
    { 0x03C0, 0x08, 0x3F },
    { 0x03C0, 0x09, 0x3F },
    { 0x03C0, 0x0A, 0x3F },
    { 0x03C0, 0x0B, 0x3F },
    { 0x03C0, 0x0C, 0x3F },
    { 0x03C0, 0x0D, 0x3F },
    { 0x03C0, 0x0E, 0x3F },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts15[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x06, 0x0B },
    { 0x03D5, 0x07, 0x3E },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0xEA },
    { 0x03D5, 0x11, 0x8C },
    { 0x03D5, 0x12, 0xDF },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0xE7 },
    { 0x03D5, 0x16, 0x04 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts16[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x0E },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0x41 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x14, 0x40 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x08 },
    { 0x03C0, 0x09, 0x09 },
    { 0x03C0, 0x0A, 0x0A },
    { 0x03C0, 0x0B, 0x0B },
    { 0x03C0, 0x0C, 0x0C },
    { 0x03C0, 0x0D, 0x0D },
    { 0x03C0, 0x0E, 0x0E },
    { 0x03C0, 0x0F, 0x0F },
    { 0x03C0, 0x10, 0x41 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts17[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x0E },
    { 0x03D5, 0x03, 0x02 },
    { 0x03D5, 0x04, 0x53 },
    { 0x03D5, 0x05, 0x9F },
    { 0x03D5, 0x06, 0x0B },
    { 0x03D5, 0x07, 0x3E },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0xEA },
    { 0x03D5, 0x11, 0x8C },
    { 0x03D5, 0x12, 0xDF },
    { 0x03D5, 0x13, 0x80 },
    { 0x03D5, 0x14, 0x60 },
    { 0x03D5, 0x15, 0xE7 },
    { 0x03D5, 0x16, 0x04 },
    { 0x03D5, 0x17, 0xAB },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x41 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts18[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x0E },
    { 0x03D5, 0x00, 0x7B },
    { 0x03D5, 0x01, 0x63 },
    { 0x03D5, 0x02, 0x63 },
    { 0x03D5, 0x03, 0x80 },
    { 0x03D5, 0x04, 0x67 },
    { 0x03D5, 0x05, 0x10 },
    { 0x03D5, 0x06, 0x6F },
    { 0x03D5, 0x07, 0xF0 },
    { 0x03D5, 0x09, 0x60 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x58 },
    { 0x03D5, 0x11, 0x8A },
    { 0x03D5, 0x12, 0x57 },
    { 0x03D5, 0x13, 0x80 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0x57 },
    { 0x03D5, 0x16, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts19[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x00, 0x9A },
    { 0x03D5, 0x01, 0x7F },
    { 0x03D5, 0x02, 0x7F },
    { 0x03D5, 0x03, 0x9D },
    { 0x03D5, 0x04, 0x81 },
    { 0x03D5, 0x05, 0x17 },
    { 0x03D5, 0x06, 0x97 },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x80 },
    { 0x03D5, 0x11, 0x84 },
    { 0x03D5, 0x12, 0x7F },
    { 0x03D5, 0x13, 0x80 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0x80 },
    { 0x03D5, 0x16, 0x00 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts20[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x00, 0xB1 },
    { 0x03D5, 0x01, 0x8F },
    { 0x03D5, 0x02, 0x90 },
    { 0x03D5, 0x03, 0x14 },
    { 0x03D5, 0x04, 0x93 },
    { 0x03D5, 0x05, 0x9F },
    { 0x03D5, 0x06, 0x9A },
    { 0x03D5, 0x07, 0xFF },
    { 0x03D5, 0x09, 0x60 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x70 },
    { 0x03D5, 0x11, 0x83 },
    { 0x03D5, 0x12, 0x5F },
    { 0x03D5, 0x13, 0x90 },
    { 0x03D5, 0x14, 0x60 },
    { 0x03D5, 0x15, 0x6F },
    { 0x03D5, 0x16, 0x8D },
    { 0x03D5, 0x17, 0xEB },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts21[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x00, 0xC0 },
    { 0x03D5, 0x01, 0x9F },
    { 0x03D5, 0x02, 0xA0 },
    { 0x03D5, 0x03, 0x83 },
    { 0x03D5, 0x04, 0xA4 },
    { 0x03D5, 0x05, 0x19 },
    { 0x03D5, 0x06, 0x18 },
    { 0x03D5, 0x07, 0xB2 },
    { 0x03D5, 0x09, 0x60 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x01 },
    { 0x03D5, 0x11, 0x85 },
    { 0x03D5, 0x12, 0xFF },
    { 0x03D5, 0x13, 0x80 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0x00 },
    { 0x03D5, 0x16, 0x18 },
    { 0x03D5, 0x17, 0xE3 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts22[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x0E },
    { 0x03D5, 0x04, 0x54 },
    { 0x03D5, 0x05, 0x80 },
    { 0x03D5, 0x09, 0x40 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x13, 0x50 },
    { 0x03D5, 0x14, 0x40 },
    { 0x03C0, 0x06, 0x06 },
    { 0x03C0, 0x08, 0x10 },
    { 0x03C0, 0x09, 0x11 },
    { 0x03C0, 0x0A, 0x12 },
    { 0x03C0, 0x0B, 0x13 },
    { 0x03C0, 0x0C, 0x14 },
    { 0x03C0, 0x0D, 0x15 },
    { 0x03C0, 0x0E, 0x16 },
    { 0x03C0, 0x0F, 0x17 },
    { 0x03C0, 0x10, 0x41 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts23[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x04, 0x02 },
    { 0x03D5, 0x00, 0x9A },
    { 0x03D5, 0x01, 0x83 },
    { 0x03D5, 0x02, 0x84 },
    { 0x03D5, 0x03, 0x9C },
    { 0x03D5, 0x04, 0x88 },
    { 0x03D5, 0x05, 0x1A },
    { 0x03D5, 0x09, 0x47 },
    { 0x03D5, 0x0A, 0x06 },
    { 0x03D5, 0x0B, 0x07 },
    { 0x03D5, 0x10, 0x83 },
    { 0x03D5, 0x11, 0x85 },
    { 0x03D5, 0x12, 0x58 },
    { 0x03D5, 0x13, 0x42 },
    { 0x03D5, 0x15, 0x63 },
    { 0x03D5, 0x16, 0xBA },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts24[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x04, 0x02 },
    { 0x03D5, 0x00, 0x9A },
    { 0x03D5, 0x01, 0x83 },
    { 0x03D5, 0x02, 0x84 },
    { 0x03D5, 0x03, 0x9C },
    { 0x03D5, 0x04, 0x88 },
    { 0x03D5, 0x05, 0x1A },
    { 0x03D5, 0x13, 0x42 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue ApplyVideoParametersTest_modifiedIndexedPorts25[] =
{
    { 0x03C5, 0x00, 0x03 },
    { 0x03C5, 0x01, 0x21 },
    { 0x03C5, 0x02, 0x0F },
    { 0x03C5, 0x04, 0x06 },
    { 0x03D5, 0x00, 0x01 },
    { 0x03D5, 0x01, 0xC7 },
    { 0x03D5, 0x02, 0xC8 },
    { 0x03D5, 0x03, 0x81 },
    { 0x03D5, 0x04, 0xD3 },
    { 0x03D5, 0x05, 0x13 },
    { 0x03D5, 0x06, 0x74 },
    { 0x03D5, 0x07, 0xE0 },
    { 0x03D5, 0x09, 0x60 },
    { 0x03D5, 0x0A, 0x00 },
    { 0x03D5, 0x0B, 0x00 },
    { 0x03D5, 0x10, 0x58 },
    { 0x03D5, 0x11, 0x00 },
    { 0x03D5, 0x12, 0x57 },
    { 0x03D5, 0x13, 0x64 },
    { 0x03D5, 0x14, 0x00 },
    { 0x03D5, 0x15, 0x58 },
    { 0x03D5, 0x16, 0x00 },
    { 0x03C0, 0x10, 0x01 },
    { 0x03C0, 0x13, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue* ApplyVideoParametersTest_modifiedIndexedPorts[] =
{
    ApplyVideoParametersTest_modifiedIndexedPorts0, //0
    ApplyVideoParametersTest_modifiedIndexedPorts1, //1
    ApplyVideoParametersTest_modifiedIndexedPorts2, //2
    ApplyVideoParametersTest_modifiedIndexedPorts2, //3
    ApplyVideoParametersTest_modifiedIndexedPorts3, //4
    ApplyVideoParametersTest_modifiedIndexedPorts3, //5
    ApplyVideoParametersTest_modifiedIndexedPorts4, //6
    ApplyVideoParametersTest_modifiedIndexedPorts5, //7
    ApplyVideoParametersTest_modifiedIndexedPorts6, //8
    ApplyVideoParametersTest_modifiedIndexedPorts7, //9
    ApplyVideoParametersTest_modifiedIndexedPorts8, //10
    ApplyVideoParametersTest_modifiedIndexedPorts9, //11
    ApplyVideoParametersTest_modifiedIndexedPorts9, //12
    ApplyVideoParametersTest_modifiedIndexedPorts10, //13
    ApplyVideoParametersTest_modifiedIndexedPorts11, //14
    ApplyVideoParametersTest_modifiedIndexedPorts12, //15
    ApplyVideoParametersTest_modifiedIndexedPorts13, //16
    ApplyVideoParametersTest_modifiedIndexedPorts14, //17
    ApplyVideoParametersTest_modifiedIndexedPorts15, //18
    ApplyVideoParametersTest_modifiedIndexedPorts16, //19
    ApplyVideoParametersTest_modifiedIndexedPorts17, //20
    ApplyVideoParametersTest_modifiedIndexedPorts18, //21
    ApplyVideoParametersTest_modifiedIndexedPorts18, //22
    ApplyVideoParametersTest_modifiedIndexedPorts19, //23
    ApplyVideoParametersTest_modifiedIndexedPorts19, //24
    ApplyVideoParametersTest_modifiedIndexedPorts20, //25
    ApplyVideoParametersTest_modifiedIndexedPorts21, //26
    ApplyVideoParametersTest_modifiedIndexedPorts22, //27
    ApplyVideoParametersTest_modifiedIndexedPorts23, //28
    ApplyVideoParametersTest_modifiedIndexedPorts24, //29
    ApplyVideoParametersTest_modifiedIndexedPorts22, //30
    ApplyVideoParametersTest_modifiedIndexedPorts17, //31
    ApplyVideoParametersTest_modifiedIndexedPorts18, //32
    ApplyVideoParametersTest_modifiedIndexedPorts18, //33
    ApplyVideoParametersTest_modifiedIndexedPorts19, //34
    ApplyVideoParametersTest_modifiedIndexedPorts19, //35
    ApplyVideoParametersTest_modifiedIndexedPorts21, //36
    ApplyVideoParametersTest_modifiedIndexedPorts21, //37
    ApplyVideoParametersTest_modifiedIndexedPorts17, //38
    ApplyVideoParametersTest_modifiedIndexedPorts17, //39
    ApplyVideoParametersTest_modifiedIndexedPorts17, //40
    ApplyVideoParametersTest_modifiedIndexedPorts18, //41
    ApplyVideoParametersTest_modifiedIndexedPorts18, //42
    ApplyVideoParametersTest_modifiedIndexedPorts18, //43
    ApplyVideoParametersTest_modifiedIndexedPorts19, //44
    ApplyVideoParametersTest_modifiedIndexedPorts19, //45
    ApplyVideoParametersTest_modifiedIndexedPorts19, //46
    ApplyVideoParametersTest_modifiedIndexedPorts21, //47
    ApplyVideoParametersTest_modifiedIndexedPorts21, //48
    ApplyVideoParametersTest_modifiedIndexedPorts25 //49
};

int ApplyVideoParametersTest_modifiedIndexedPortsCount[] =
{
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //0
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //1
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //2
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //3
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //4
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //5
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //6
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //7
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //8
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //9
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //10
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //11
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //12
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //13
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts11) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //14
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //15
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts13) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //16
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts14) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //17
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //18
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts16) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //19
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //20
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //21
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //22
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //23
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //24
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts20) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //25
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //26
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts22) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //27
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts23) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //28
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts24) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //29
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts22) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //30
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //31
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //32
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //33
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //34
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //35
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //36
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //37
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //38
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //39
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //40
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //41
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //42
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //43
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //44
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //45
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //46
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //47
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //48
    sizeof(ApplyVideoParametersTest_modifiedIndexedPorts25) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue) //49
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts0[] =
{
    { 0, 0 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts1[] =
{
    { 0x03C5, 0x02 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x09 },
    { 0x03D5, 0x0A },
    { 0x03D5, 0x0B },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x10 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x13 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts2[] =
{
    { 0x03C5, 0x02 },
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x04 },
    { 0x03D5, 0x05 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x09 },
    { 0x03D5, 0x0A },
    { 0x03D5, 0x0B },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x10 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x13 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts3[] =
{
    { 0x03C5, 0x02 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts4[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts5[] =
{
    { 0x03C5, 0x02 },
    { 0x03B5, 0x18 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts6[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x04 },
    { 0x03D5, 0x05 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts7[] =
{
    { 0x03C0, 0x00 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts8[] =
{
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts9[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x04 },
    { 0x03D5, 0x05 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts10[] =
{
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts11[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts12[] =
{
    { 0x03B5, 0x18 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts13[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts14[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts15[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts16[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x13 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts17[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts18[] =
{
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts19[] =
{
    { 0x03D5, 0x07 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts20[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x01 },
    { 0x03D5, 0x02 },
    { 0x03D5, 0x03 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts21[] =
{
    { 0x03C5, 0x02 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x10 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts22[] =
{
    { 0x03C5, 0x02 },
    { 0x03D5, 0x06 },
    { 0x03D5, 0x07 },
    { 0x03D5, 0x09 },
    { 0x03D5, 0x0A },
    { 0x03D5, 0x0B },
    { 0x03D5, 0x10 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x12 },
    { 0x03D5, 0x14 },
    { 0x03D5, 0x15 },
    { 0x03D5, 0x16 },
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x10 },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex ApplyVideoParametersTest_readIndexedPorts23[] =
{
    { 0x03D5, 0x17 },
    { 0x03D5, 0x18 },
    { 0x03C0, 0x00 },
    { 0x03C0, 0x01 },
    { 0x03C0, 0x02 },
    { 0x03C0, 0x03 },
    { 0x03C0, 0x04 },
    { 0x03C0, 0x05 },
    { 0x03C0, 0x06 },
    { 0x03C0, 0x07 },
    { 0x03C0, 0x08 },
    { 0x03C0, 0x09 },
    { 0x03C0, 0x0A },
    { 0x03C0, 0x0B },
    { 0x03C0, 0x0C },
    { 0x03C0, 0x0D },
    { 0x03C0, 0x0E },
    { 0x03C0, 0x0F },
    { 0x03C0, 0x12 },
    { 0x03C0, 0x16 }
};

Hag::Testing::Mock::PortAndIndex* ApplyVideoParametersTest_readIndexedPorts[] =
{
    ApplyVideoParametersTest_readIndexedPorts0, //0
    ApplyVideoParametersTest_readIndexedPorts1, //1
    ApplyVideoParametersTest_readIndexedPorts2, //2
    ApplyVideoParametersTest_readIndexedPorts2, //3
    ApplyVideoParametersTest_readIndexedPorts3, //4
    ApplyVideoParametersTest_readIndexedPorts3, //5
    ApplyVideoParametersTest_readIndexedPorts4, //6
    ApplyVideoParametersTest_readIndexedPorts5, //7
    ApplyVideoParametersTest_readIndexedPorts6, //8
    ApplyVideoParametersTest_readIndexedPorts7, //9
    ApplyVideoParametersTest_readIndexedPorts8, //10
    ApplyVideoParametersTest_readIndexedPorts9, //11
    ApplyVideoParametersTest_readIndexedPorts9, //12
    ApplyVideoParametersTest_readIndexedPorts10, //13
    ApplyVideoParametersTest_readIndexedPorts11, //14
    ApplyVideoParametersTest_readIndexedPorts12, //15
    ApplyVideoParametersTest_readIndexedPorts13, //16
    ApplyVideoParametersTest_readIndexedPorts14, //17
    ApplyVideoParametersTest_readIndexedPorts15, //18
    ApplyVideoParametersTest_readIndexedPorts16, //19
    ApplyVideoParametersTest_readIndexedPorts17, //20
    ApplyVideoParametersTest_readIndexedPorts18, //21
    ApplyVideoParametersTest_readIndexedPorts18, //22
    ApplyVideoParametersTest_readIndexedPorts19, //23
    ApplyVideoParametersTest_readIndexedPorts19, //24
    ApplyVideoParametersTest_readIndexedPorts18, //25
    ApplyVideoParametersTest_readIndexedPorts18, //26
    ApplyVideoParametersTest_readIndexedPorts20, //27
    ApplyVideoParametersTest_readIndexedPorts21, //28
    ApplyVideoParametersTest_readIndexedPorts22, //29
    ApplyVideoParametersTest_readIndexedPorts20, //30
    ApplyVideoParametersTest_readIndexedPorts17, //31
    ApplyVideoParametersTest_readIndexedPorts18, //32
    ApplyVideoParametersTest_readIndexedPorts18, //33
    ApplyVideoParametersTest_readIndexedPorts19, //34
    ApplyVideoParametersTest_readIndexedPorts19, //35
    ApplyVideoParametersTest_readIndexedPorts18, //36
    ApplyVideoParametersTest_readIndexedPorts18, //37
    ApplyVideoParametersTest_readIndexedPorts17, //38
    ApplyVideoParametersTest_readIndexedPorts17, //39
    ApplyVideoParametersTest_readIndexedPorts17, //40
    ApplyVideoParametersTest_readIndexedPorts18, //41
    ApplyVideoParametersTest_readIndexedPorts18, //42
    ApplyVideoParametersTest_readIndexedPorts18, //43
    ApplyVideoParametersTest_readIndexedPorts19, //44
    ApplyVideoParametersTest_readIndexedPorts19, //45
    ApplyVideoParametersTest_readIndexedPorts19, //46
    ApplyVideoParametersTest_readIndexedPorts18, //47
    ApplyVideoParametersTest_readIndexedPorts18, //48
    ApplyVideoParametersTest_readIndexedPorts23 //49
};

int ApplyVideoParametersTest_readIndexedPortsCount[] =
{
    sizeof(ApplyVideoParametersTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //0
    sizeof(ApplyVideoParametersTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //1
    sizeof(ApplyVideoParametersTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //2
    sizeof(ApplyVideoParametersTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //3
    sizeof(ApplyVideoParametersTest_readIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndex), //4
    sizeof(ApplyVideoParametersTest_readIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndex), //5
    sizeof(ApplyVideoParametersTest_readIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndex), //6
    sizeof(ApplyVideoParametersTest_readIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndex), //7
    sizeof(ApplyVideoParametersTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //8
    sizeof(ApplyVideoParametersTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //9
    sizeof(ApplyVideoParametersTest_readIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndex), //10
    sizeof(ApplyVideoParametersTest_readIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndex), //11
    sizeof(ApplyVideoParametersTest_readIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndex), //12
    sizeof(ApplyVideoParametersTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //13
    sizeof(ApplyVideoParametersTest_readIndexedPorts11) / sizeof(Hag::Testing::Mock::PortAndIndex), //14
    sizeof(ApplyVideoParametersTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //15
    sizeof(ApplyVideoParametersTest_readIndexedPorts13) / sizeof(Hag::Testing::Mock::PortAndIndex), //16
    sizeof(ApplyVideoParametersTest_readIndexedPorts14) / sizeof(Hag::Testing::Mock::PortAndIndex), //17
    sizeof(ApplyVideoParametersTest_readIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndex), //18
    sizeof(ApplyVideoParametersTest_readIndexedPorts16) / sizeof(Hag::Testing::Mock::PortAndIndex), //19
    sizeof(ApplyVideoParametersTest_readIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndex), //20
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //21
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //22
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //23
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //24
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //25
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //26
    sizeof(ApplyVideoParametersTest_readIndexedPorts20) / sizeof(Hag::Testing::Mock::PortAndIndex), //27
    sizeof(ApplyVideoParametersTest_readIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndex), //28
    sizeof(ApplyVideoParametersTest_readIndexedPorts22) / sizeof(Hag::Testing::Mock::PortAndIndex), //29
    sizeof(ApplyVideoParametersTest_readIndexedPorts20) / sizeof(Hag::Testing::Mock::PortAndIndex), //30
    sizeof(ApplyVideoParametersTest_readIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndex), //31
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //32
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //33
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //34
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //35
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //36
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //37
    sizeof(ApplyVideoParametersTest_readIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndex), //38
    sizeof(ApplyVideoParametersTest_readIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndex), //39
    sizeof(ApplyVideoParametersTest_readIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndex), //40
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //41
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //42
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //43
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //44
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //45
    sizeof(ApplyVideoParametersTest_readIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndex), //46
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //47
    sizeof(ApplyVideoParametersTest_readIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndex), //48
    sizeof(ApplyVideoParametersTest_readIndexedPorts23) / sizeof(Hag::Testing::Mock::PortAndIndex) //49

};

Hag::Testing::Mock::PortAndValue CRTControllerInitData_modifiedPorts[] =
{
    {0,0}
};

int CRTControllerInitData_modifiedPortsCount = sizeof(CRTControllerInitData_modifiedPorts) / sizeof(Hag::Testing::Mock::PortAndValue);

uint16_t CRTControllerInitData_readPorts[] =
{
    0
};

int CRTControllerInitData_readPortsCount = sizeof(CRTControllerInitData_readPorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndexAndValue CRTControllerInitData_modifiedIndexedPorts[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03D5, 0x32, 0x40 },
    { 0x03D5, 0x3B, 0x5A },
    { 0x03D5, 0x3C, 0x10 },
    { 0x03D5, 0x40, 0xD0 },
    { 0x03D5, 0x45, 0x00 },
    { 0x03D5, 0x54, 0x38 },
    { 0x03D5, 0x60, 0x07 },
    { 0x03D5, 0x61, 0x80 },
    { 0x03D5, 0x62, 0xA1 }
};

int CRTControllerInitData_modifiedIndexedPortsCount = sizeof(CRTControllerInitData_modifiedIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue);

Hag::Testing::Mock::PortAndIndex CRTControllerInitData_readIndexedPorts[] =
{
    { 0x03C5, 0x0B },
    { 0x03C5, 0x14 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x33 },
    { 0x03D5, 0x34 },
    { 0x03D5, 0x35 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x42 },
    { 0x03D5, 0x43 },
    { 0x03D5, 0x50 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x53 },
    { 0x03D5, 0x55 },
    { 0x03D5, 0x58 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E },
    { 0x03D5, 0x67 },
    { 0x03D5, 0x69 },
    { 0x03D5, 0x6A }
};

int CRTControllerInitData_readIndexedPortsCount = sizeof(CRTControllerInitData_readIndexedPorts) / sizeof(Hag::Testing::Mock::PortAndIndex);

uint16_t CRTControllerInitData_ignorePorts[] =
{
    0x03C4,
    0x03C5,
    0x03CC,
    0x03D4,
    0x03D5
};

int CRTControllerInitData_ignorePortsCount = sizeof(CRTControllerInitData_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts0[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x42 },
    { 0x03C5, 0x13, 0x2B },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};


Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts1[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x61 },
    { 0x03C5, 0x13, 0x33 },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts2[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x44 },
    { 0x03C5, 0x13, 0x51 },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts3[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x22 },
    { 0x03C5, 0x13, 0x2B },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts4[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x24 },
    { 0x03C5, 0x13, 0x6F },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts5[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x61 },
    { 0x03C5, 0x13, 0x28 },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x00 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts6[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x20 },
    { 0x03C5, 0x13, 0x13 },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x20 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts7[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x42 },
    { 0x03C5, 0x13, 0x30 },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x20 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts8[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x22 },
    { 0x03C5, 0x13, 0x2B },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x20 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetupClocksTest_modifiedIndexedPorts9[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x12, 0x24 },
    { 0x03C5, 0x13, 0x6B },
    { 0x03C5, 0x15, 0x02 },
    { 0x03D5, 0x42, 0x20 }
};

Hag::Testing::Mock::PortAndIndexAndValue* SetupClocksTest_modifiedIndexedPorts[] =
{
    SetupClocksTest_modifiedIndexedPorts0, //0
    SetupClocksTest_modifiedIndexedPorts0, //1
    SetupClocksTest_modifiedIndexedPorts0, //2
    SetupClocksTest_modifiedIndexedPorts0, //3
    SetupClocksTest_modifiedIndexedPorts0, //4
    SetupClocksTest_modifiedIndexedPorts0, //5
    SetupClocksTest_modifiedIndexedPorts0, //6
    SetupClocksTest_modifiedIndexedPorts0, //7
    SetupClocksTest_modifiedIndexedPorts0, //8
    SetupClocksTest_modifiedIndexedPorts0, //9
    SetupClocksTest_modifiedIndexedPorts0, //10
    SetupClocksTest_modifiedIndexedPorts0, //11
    SetupClocksTest_modifiedIndexedPorts0, //12
    SetupClocksTest_modifiedIndexedPorts0, //13
    SetupClocksTest_modifiedIndexedPorts0, //14
    SetupClocksTest_modifiedIndexedPorts0, //15
    SetupClocksTest_modifiedIndexedPorts0, //16
    SetupClocksTest_modifiedIndexedPorts0, //17
    SetupClocksTest_modifiedIndexedPorts0, //18
    SetupClocksTest_modifiedIndexedPorts0, //19
    SetupClocksTest_modifiedIndexedPorts1, //20
    SetupClocksTest_modifiedIndexedPorts2, //21
    SetupClocksTest_modifiedIndexedPorts2, //22
    SetupClocksTest_modifiedIndexedPorts3, //23
    SetupClocksTest_modifiedIndexedPorts3, //24
    SetupClocksTest_modifiedIndexedPorts3, //25
    SetupClocksTest_modifiedIndexedPorts4, //26
    SetupClocksTest_modifiedIndexedPorts5, //27
    SetupClocksTest_modifiedIndexedPorts0, //28
    SetupClocksTest_modifiedIndexedPorts0, //29
    SetupClocksTest_modifiedIndexedPorts5, //30
    SetupClocksTest_modifiedIndexedPorts1, //31
    SetupClocksTest_modifiedIndexedPorts2, //32
    SetupClocksTest_modifiedIndexedPorts2, //33
    SetupClocksTest_modifiedIndexedPorts3, //34
    SetupClocksTest_modifiedIndexedPorts3, //35
    SetupClocksTest_modifiedIndexedPorts6, //36
    SetupClocksTest_modifiedIndexedPorts4, //37
    SetupClocksTest_modifiedIndexedPorts1, //38
    SetupClocksTest_modifiedIndexedPorts1, //39
    SetupClocksTest_modifiedIndexedPorts1, //40
    SetupClocksTest_modifiedIndexedPorts2, //41
    SetupClocksTest_modifiedIndexedPorts2, //42
    SetupClocksTest_modifiedIndexedPorts2, //43
    SetupClocksTest_modifiedIndexedPorts3, //44
    SetupClocksTest_modifiedIndexedPorts3, //45
    SetupClocksTest_modifiedIndexedPorts7, //46
    SetupClocksTest_modifiedIndexedPorts8, //47
    SetupClocksTest_modifiedIndexedPorts8, //48
    SetupClocksTest_modifiedIndexedPorts9 //49
};

int SetupClocksTest_modifiedIndexedPortsCount[] =
{
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //0
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //1
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //2
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //3
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //4
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //5
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //6
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //7
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //8
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //9
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //10
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //11
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //12
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //13
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //14
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //15
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //16
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //17
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //18
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //19
    sizeof(SetupClocksTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //20
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //21
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //22
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //23
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //24
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //25
    sizeof(SetupClocksTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //26
    sizeof(SetupClocksTest_modifiedIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //27
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //28
    sizeof(SetupClocksTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //29
    sizeof(SetupClocksTest_modifiedIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //30
    sizeof(SetupClocksTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //31
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //32
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //33
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //34
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //35
    sizeof(SetupClocksTest_modifiedIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //36
    sizeof(SetupClocksTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //37
    sizeof(SetupClocksTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //38
    sizeof(SetupClocksTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //39
    sizeof(SetupClocksTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //40
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //41
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //42
    sizeof(SetupClocksTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //43
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //44
    sizeof(SetupClocksTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //45
    sizeof(SetupClocksTest_modifiedIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //46
    sizeof(SetupClocksTest_modifiedIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //47
    sizeof(SetupClocksTest_modifiedIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //48
    sizeof(SetupClocksTest_modifiedIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue) //49
};

uint16_t SetupClocksTest_ignorePorts[] =
{
    0x03C4,
    0x03C5,
    0x03CC,
    0x03D4,
    0x03D5
};

uint16_t SetupClocksTest_ignorePortsCount = sizeof(SetupClocksTest_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndex SetupClocksTest_ignoreIndexedPorts[] =
{
    { 0x03C5, 0x18 },
    { 0x03D5, 0x2E },
    { 0x03D5, 0x2F },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 }
};

uint16_t SetupClocksTest_ignoreIndexedPortsCount = sizeof(SetupClocksTest_ignoreIndexedPorts) / sizeof (Hag::Testing::Mock::PortAndIndex);

uint16_t ConfigureExtraVESAModeSettingsTest_ignorePorts[] =
{
    0x03C4,
    0x03C5,
    0x03CC,
    0x03D4,
    0x03D5
};

uint16_t ConfigureExtraVESAModeSettingsTest_ignorePortsCount = sizeof(ConfigureExtraVESAModeSettingsTest_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts1[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0x88 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts3[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0xC0 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts4[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xC8 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts5[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xC0 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts6[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0x48 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts7[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0x48 },
    { 0x03D5, 0x60, 0x48 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts8[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x01 },
    { 0x03D5, 0x54, 0x40 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts9[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0x80 },
    { 0x03D5, 0x5E, 0x15 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts10[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0x80 },
    { 0x03D5, 0x5E, 0x55 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts11[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xC0 },
    { 0x03D5, 0x5E, 0x55 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts12[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts13[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x70 },
    { 0x03D5, 0x54, 0x08 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts14[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0x08 },
    { 0x03D5, 0x60, 0x38 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x40 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x5D, 0x08 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x82 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x5D, 0x08 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts18[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x80 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts19[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0xC0 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts20[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0xC8 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts21[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x02 },
    { 0x03D5, 0x54, 0x48 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts22[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x54, 0x48 },
    { 0x03D5, 0x60, 0x48 },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts23[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xC2 },
    { 0x03D5, 0x54, 0x18 },
    { 0x03D5, 0x5E, 0x15 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts24[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xC0 },
    { 0x03D5, 0x54, 0x20 },
    { 0x03D5, 0x5E, 0x55 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts25[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xC0 },
    { 0x03D5, 0x54, 0x28 },
    { 0x03D5, 0x5E, 0x55 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts26[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xC0 },
    { 0x03D5, 0x54, 0x50 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x01 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x50 },
    { 0x03D5, 0x54, 0xF8 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts28[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x50 },
    { 0x03D5, 0x54, 0x58 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts29[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x70 },
    { 0x03D5, 0x54, 0x30 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts30[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x70 },
    { 0x03D5, 0x54, 0x30 },
    { 0x03D5, 0x60, 0x19 },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts31[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x70 },
    { 0x03D5, 0x54, 0x38 },
    { 0x03D5, 0x60, 0x1C },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x92 },
    { 0x03D5, 0x54, 0x38 },
    { 0x03D5, 0x5D, 0x01 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts33[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x92 },
    { 0x03D5, 0x54, 0x48 },
    { 0x03D5, 0x5D, 0x01 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts34[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xB2 },
    { 0x03D5, 0x5D, 0x08 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts35[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0xB2 },
    { 0x03D5, 0x54, 0x10 },
    { 0x03D5, 0x5D, 0x08 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts36[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x12 },
    { 0x03D5, 0x54, 0x10 },
    { 0x03D5, 0x5D, 0x5D },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts37[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x10 },
    { 0x03D5, 0x54, 0x10 },
    { 0x03D5, 0x5D, 0x7D },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts38[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x10 },
    { 0x03D5, 0x54, 0x18 },
    { 0x03D5, 0x5D, 0x7D },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts39[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x10 },
    { 0x03D5, 0x54, 0x28 },
    { 0x03D5, 0x5D, 0x75 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts40[] =
{
    { 0x03D5, 0x02, 0x51 },
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0x2F },
    { 0x03D5, 0x50, 0x30 },
    { 0x03D5, 0x54, 0x38 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts41[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0xD0 },
    { 0x03D5, 0x54, 0x10 },
    { 0x03D5, 0x5D, 0x7F },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts42[] =
{
    { 0x03D5, 0x04, 0x57 },
    { 0x03D5, 0x05, 0x83 },
    { 0x03D5, 0x13, 0x00 },
    { 0x03D5, 0x34, 0x10 },
    { 0x03D5, 0x3C, 0xAF },
    { 0x03D5, 0x50, 0x81 },
    { 0x03D5, 0x5D, 0x01 },
    { 0x03D5, 0x60, 0xFF },
    { 0x03D5, 0x67, 0x10 }
};


Hag::Testing::Mock::PortAndIndexAndValue* ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts[] =
{
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //0
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //1
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts1, //2
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2, //3
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2, //4
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2, //5
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //6
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //7
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //8
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //9
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //10
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts3, //11
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts4, //12
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //13
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts5, //14
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts6, //15
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts7, //16
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //17
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //18
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts8, //19
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts9, //20
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts10, //21
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts11, //22
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts12, //23
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts13, //24
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts14, //25
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts14, //26
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15, //27
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15, //28
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15, //29
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15, //30
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16, //31
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16, //32
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16, //33
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //34
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17, //35
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17, //36
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17, //37
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts18, //38
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts19, //39
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts20, //40
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //41
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //42
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts21, //43
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts22, //44
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //45
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //46
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0, //47
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts23, //48
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts24, //49
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts25, //50
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts26, //51
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27, //52
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27, //53
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts28, //54
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27, //55
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27, //56
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts28, //57
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts29, //58
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts30, //59
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts31, //60
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32, //61
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32, //62
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts33, //63
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32, //64
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32, //65
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts33, //66
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts34, //67
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts34, //68
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts35, //69
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts36, //70
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts37, //71
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts38, //72
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts39, //73
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts36, //74
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts37, //75
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts38, //76
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts39, //77
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts40, //78
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts41, //79
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts41, //80
    ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts42 //81
};

int ConfigureExtraVESAModeSettingsTest_modifiedIndexedPortsCount[] =
{
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //0
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //1
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //2
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //3
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //4
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //5
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //6
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //7
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //8
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //9
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //10
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //11
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //12
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //13
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //14
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //15
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //16
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //17
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //18
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //19
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //20
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //21
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts11) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //22
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //23
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts13) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //24
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts14) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //25
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts14) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //26
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //27
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //28
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //29
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts15) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //30
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //31
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //32
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts16) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //33
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //34
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //35
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //36
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts17) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //37
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts18) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //38
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts19) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //39
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts20) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //40
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //41
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //42
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts21) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //43
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts22) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //44
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //45
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //46
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //47
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts23) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //48
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts24) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //49
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts25) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //50
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts26) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //51
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //52
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //53
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts28) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //54
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //55
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts27) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //56
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts28) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //57
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts29) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //58
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts30) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //59
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts31) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //60
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //61
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //62
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts33) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //63
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //64
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts32) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //65
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts33) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //66
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts34) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //67
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts34) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //68
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts35) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //69
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts36) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //70
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts37) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //71
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts38) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //72
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts39) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //73
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts36) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //74
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts37) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //75
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts38) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //76
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts39) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //77
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts40) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //78
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts41) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //79
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts41) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //80
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedIndexedPorts42) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue) //81
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts0[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x50 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E },
    { 0x03D5, 0x67 }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts1[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E },
    { 0x03D5, 0x67 }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts2[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E },
    { 0x03D5, 0x67 }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts3[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts4[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x50 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts5[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x50 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts6[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts7[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5E },
    { 0x03D5, 0x67 }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts8[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts9[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5D }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts10[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts11[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x54 },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts12[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex ConfigureExtraVESAModeSettingsTest_readIndexedPorts13[] =
{
    { 0x03D5, 0x00 },
    { 0x03D5, 0x11 },
    { 0x03D5, 0x31 },
    { 0x03D5, 0x36 },
    { 0x03D5, 0x38 },
    { 0x03D5, 0x39 },
    { 0x03D5, 0x3A },
    { 0x03D5, 0x40 },
    { 0x03D5, 0x51 },
    { 0x03D5, 0x54 },
    { 0x03D5, 0x5E }
};

Hag::Testing::Mock::PortAndIndex* ConfigureExtraVESAModeSettingsTest_readIndexedPorts[] =
{
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //0
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //1
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //2
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts1, //3
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts1, //4
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts1, //5
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //6
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //7
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //8
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //9
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //10
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts1, //11
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //12
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //13
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //14
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts1, //15
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //16
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //17
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //18
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //19
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts3, //20
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts4, //21
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts4, //22
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts5, //23
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //24
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //25
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //26
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //27
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //28
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //29
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //30
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //31
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //32
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //33
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //34
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //35
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //36
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts7, //37
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts2, //38
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts8, //39
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts5, //40
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //41
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //42
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts8, //43
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts5, //44
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //45
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //46
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts0, //47
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts3, //48
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts9, //49
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts9, //50
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //51
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //52
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //53
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //54
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //55
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //56
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //57
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //58
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //59
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //60
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //61
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //62
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //63
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //64
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //65
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //66
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts11, //67
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts11, //68
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //69
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //70
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //71
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //72
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //73
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts10, //74
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //75
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //76
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //77
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts6, //78
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //79
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts12, //80
    ConfigureExtraVESAModeSettingsTest_readIndexedPorts13 //81
};

int ConfigureExtraVESAModeSettingsTest_readIndexedPortsCount[] =
{
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //0
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //1
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //2
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //3
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //4
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //5
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //6
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //7
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //8
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //9
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //10
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //11
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //12
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //13
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //14
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //15
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //16
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //17
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //18
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //19
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndex), //20
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndex), //21
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndex), //22
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndex), //23
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //24
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //25
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //26
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //27
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //28
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //29
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //30
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //31
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //32
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //33
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //34
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //35
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //36
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts7) / sizeof(Hag::Testing::Mock::PortAndIndex), //37
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //38
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndex), //39
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndex), //40
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //41
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //42
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts8) / sizeof(Hag::Testing::Mock::PortAndIndex), //43
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts5) / sizeof(Hag::Testing::Mock::PortAndIndex), //44
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //45
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //46
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //47
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndex), //48
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndex), //49
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts9) / sizeof(Hag::Testing::Mock::PortAndIndex), //50
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //51
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //52
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //53
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //54
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //55
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //56
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //57
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //58
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //59
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //60
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //61
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //62
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //63
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //64
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //65
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //66
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts11) / sizeof(Hag::Testing::Mock::PortAndIndex), //67
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts11) / sizeof(Hag::Testing::Mock::PortAndIndex), //68
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //69
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //70
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //71
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //72
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //73
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts10) / sizeof(Hag::Testing::Mock::PortAndIndex), //74
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //75
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //76
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //77
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts6) / sizeof(Hag::Testing::Mock::PortAndIndex), //78
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //79
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts12) / sizeof(Hag::Testing::Mock::PortAndIndex), //80
    sizeof(ConfigureExtraVESAModeSettingsTest_readIndexedPorts13) / sizeof(Hag::Testing::Mock::PortAndIndex) //81
};

Hag::Testing::Mock::PortAndValue ConfigureExtraVESAModeSettingsTest_modifiedPorts0[] =
{
    { 0x0000, 0x00 }
};

Hag::Testing::Mock::PortAndValue ConfigureExtraVESAModeSettingsTest_modifiedPorts1[] =
{
    { 0x03C2, 0x27 }
};

Hag::Testing::Mock::PortAndValue* ConfigureExtraVESAModeSettingsTest_modifiedPorts[] =
{
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //0
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //1
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //2
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //3
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //4
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //5
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //6
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //7
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //8
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //9
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //10
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //11
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //12
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //13
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //14
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //15
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //16
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //17
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //18
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //19
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //20
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //21
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //22
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //23
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //24
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //25
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //26
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //27
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //28
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //29
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //30
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //31
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //32
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //33
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //34
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //35
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //36
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //37
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //38
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //39
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //40
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //41
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //42
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //43
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //44
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //45
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //46
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //47
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //48
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //49
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //50
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //51
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //52
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //53
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //54
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //55
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //56
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //57
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //58
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //59
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //60
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //61
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //62
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //63
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //64
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //65
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //66
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //67
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //68
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //69
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //70
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //71
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //72
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //73
    ConfigureExtraVESAModeSettingsTest_modifiedPorts1, //74
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //75
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //76
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //77
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //78
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //79
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0, //80
    ConfigureExtraVESAModeSettingsTest_modifiedPorts0 //81
};

int ConfigureExtraVESAModeSettingsTest_modifiedPortsCount[] =
{
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //0
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //1
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //2
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //3
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //4
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //5
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //6
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //7
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //8
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //9
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //10
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //11
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //12
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //13
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //14
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //15
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //16
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //17
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //18
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //19
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //20
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //21
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //22
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //23
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //24
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //25
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //26
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //27
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //28
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //29
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //30
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //31
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //32
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //33
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //34
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //35
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //36
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //37
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //38
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //39
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //40
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //41
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //42
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //43
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //44
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //45
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //46
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //47
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //48
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //49
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //50
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //51
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //52
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //53
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //54
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //55
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //56
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //57
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //58
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //59
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //60
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //61
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //62
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //63
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //64
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //65
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //66
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //67
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //68
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //69
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //70
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //71
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //72
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //73
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts1) / sizeof(Hag::Testing::Mock::PortAndValue), //74
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //75
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //76
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //77
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //78
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //79
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //80
    sizeof(ConfigureExtraVESAModeSettingsTest_modifiedPorts0) / sizeof(Hag::Testing::Mock::PortAndValue), //81
};

uint16_t EnableOver256KAddressingAndSetAddressWindowTest_ignorePorts[] =
{
    0x03D4,
    0x03D5
};

int EnableOver256KAddressingAndSetAddressWindowTest_ignorePortsCount = sizeof(EnableOver256KAddressingAndSetAddressWindowTest_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndex EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0[] =
{
    { 0x03D5, 0x59 },
    { 0x03D5, 0x5A },
    { 0x03D5, 0x6B },
    { 0x03D5, 0x6C }
};

Hag::Testing::Mock::PortAndIndex EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1[] =
{
    { 0x03D5, 0x32 },
    { 0x03D5, 0x59 },
    { 0x03D5, 0x5A },
    { 0x03D5, 0x6B },
    { 0x03D5, 0x6C }
};

Hag::Testing::Mock::PortAndIndex* EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts[] =
{
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //0
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //1
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //2
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //3
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //4
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //5
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //6
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //7
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //8
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //9
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //10
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //11
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //12
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //13
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //14
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //15
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //16
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //17
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //18
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0, //19
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //20
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //21
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //22
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //23
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //24
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //25
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //26
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //27
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //28
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //29
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //30
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //31
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //32
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //33
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //34
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //35
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //36
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //37
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //38
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //39
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //40
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //41
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //42
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //43
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //44
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //45
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //46
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //47
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1, //48
    EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1 //49
};

int EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPortsCount[] =
{
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //0
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //1
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //2
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //3
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //4
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //5
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //6
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //7
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //8
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //9
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //10
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //11
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //12
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //13
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //14
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //15
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //16
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //17
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //18
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //19
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //20
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //21
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //22
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //23
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //24
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //25
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //26
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //27
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //28
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //29
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //30
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //31
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //32
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //33
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //34
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //35
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //36
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //37
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //38
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //39
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //40
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //41
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //42
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //43
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //44
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //45
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //46
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //47
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //48
    sizeof(EnableOver256KAddressingAndSetAddressWindowTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex) //49
};

uint16_t SetColorModeTest_ignorePorts[] =
{
    0x03C4,
    0x03C5,
    0x03CC,
    0x03D4,
    0x03D5
};

int SetColorModeTest_ignorePortsCount = sizeof(SetColorModeTest_ignorePorts) / sizeof(uint16_t);

Hag::Testing::Mock::PortAndIndex SetColorModeTest_readIndexedPorts0[] =
{
    { 0x0000, 0x00 }
};

Hag::Testing::Mock::PortAndIndex SetColorModeTest_readIndexedPorts1[] =
{
    { 0x03C5, 0x15 },
    { 0x03D5, 0x67 }
};

Hag::Testing::Mock::PortAndIndex SetColorModeTest_readIndexedPorts2[] =
{
    { 0x03C5, 0x15 }
};

Hag::Testing::Mock::PortAndIndex* SetColorModeTest_readIndexedPorts[] =
{
    SetColorModeTest_readIndexedPorts1, //0
    SetColorModeTest_readIndexedPorts1, //1
    SetColorModeTest_readIndexedPorts1, //2
    SetColorModeTest_readIndexedPorts1, //3
    SetColorModeTest_readIndexedPorts1, //4
    SetColorModeTest_readIndexedPorts1, //5
    SetColorModeTest_readIndexedPorts1, //6
    SetColorModeTest_readIndexedPorts1, //7
    SetColorModeTest_readIndexedPorts1, //8
    SetColorModeTest_readIndexedPorts1, //9
    SetColorModeTest_readIndexedPorts1, //10
    SetColorModeTest_readIndexedPorts1, //11
    SetColorModeTest_readIndexedPorts1, //12
    SetColorModeTest_readIndexedPorts1, //13
    SetColorModeTest_readIndexedPorts1, //14
    SetColorModeTest_readIndexedPorts1, //15
    SetColorModeTest_readIndexedPorts1, //16
    SetColorModeTest_readIndexedPorts1, //17
    SetColorModeTest_readIndexedPorts1, //18
    SetColorModeTest_readIndexedPorts1, //19
    SetColorModeTest_readIndexedPorts1, //20
    SetColorModeTest_readIndexedPorts1, //21
    SetColorModeTest_readIndexedPorts1, //22
    SetColorModeTest_readIndexedPorts1, //23
    SetColorModeTest_readIndexedPorts1, //24
    SetColorModeTest_readIndexedPorts1, //25
    SetColorModeTest_readIndexedPorts0, //26
    SetColorModeTest_readIndexedPorts2, //27
    SetColorModeTest_readIndexedPorts1, //28
    SetColorModeTest_readIndexedPorts1, //29
    SetColorModeTest_readIndexedPorts1, //30
    SetColorModeTest_readIndexedPorts1, //31
    SetColorModeTest_readIndexedPorts1, //32
    SetColorModeTest_readIndexedPorts1, //33
    SetColorModeTest_readIndexedPorts1, //34
    SetColorModeTest_readIndexedPorts1, //35
    SetColorModeTest_readIndexedPorts1, //36
    SetColorModeTest_readIndexedPorts0, //37
    SetColorModeTest_readIndexedPorts2, //38
    SetColorModeTest_readIndexedPorts2, //39
    SetColorModeTest_readIndexedPorts2, //40
    SetColorModeTest_readIndexedPorts2, //41
    SetColorModeTest_readIndexedPorts2, //42
    SetColorModeTest_readIndexedPorts2, //43
    SetColorModeTest_readIndexedPorts2, //44
    SetColorModeTest_readIndexedPorts2, //45
    SetColorModeTest_readIndexedPorts2, //46
    SetColorModeTest_readIndexedPorts2, //47
    SetColorModeTest_readIndexedPorts2, //48
    SetColorModeTest_readIndexedPorts0 //49
};

int SetColorModeTest_readIndexedPortsCount[] =
{
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //0
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //1
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //2
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //3
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //4
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //5
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //6
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //7
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //8
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //9
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //10
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //11
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //12
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //13
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //14
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //15
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //16
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //17
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //18
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //19
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //20
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //21
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //22
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //23
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //24
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //25
    sizeof(SetColorModeTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //26
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //27
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //28
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //29
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //30
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //31
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //32
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //33
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //34
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //35
    sizeof(SetColorModeTest_readIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndex), //36
    sizeof(SetColorModeTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex), //37
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //38
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //39
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //40
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //41
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //42
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //43
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //44
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //45
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //46
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //47
    sizeof(SetColorModeTest_readIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndex), //48
    sizeof(SetColorModeTest_readIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndex) //49
};


Hag::Testing::Mock::PortAndIndexAndValue SetColorModeTest_modifiedIndexedPorts0[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x18, 0x40 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetColorModeTest_modifiedIndexedPorts1[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x15, 0x10 },
    { 0x03C5, 0x18, 0xC0 },
    { 0x03D5, 0x67, 0x10 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetColorModeTest_modifiedIndexedPorts2[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x18, 0x40 },
    { 0x03D5, 0x67, 0xD0 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetColorModeTest_modifiedIndexedPorts3[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x18, 0x40 },
    { 0x03D5, 0x67, 0x30 }
};

Hag::Testing::Mock::PortAndIndexAndValue SetColorModeTest_modifiedIndexedPorts4[] =
{
    { 0x03C5, 0x08, 0x06 },
    { 0x03C5, 0x18, 0x40 },
    { 0x03D5, 0x67, 0x50 }
};

Hag::Testing::Mock::PortAndIndexAndValue* SetColorModeTest_modifiedIndexedPorts[] =
{
    SetColorModeTest_modifiedIndexedPorts0, //0
    SetColorModeTest_modifiedIndexedPorts0, //1
    SetColorModeTest_modifiedIndexedPorts0, //2
    SetColorModeTest_modifiedIndexedPorts0, //3
    SetColorModeTest_modifiedIndexedPorts0, //4
    SetColorModeTest_modifiedIndexedPorts0, //5
    SetColorModeTest_modifiedIndexedPorts0, //6
    SetColorModeTest_modifiedIndexedPorts0, //7
    SetColorModeTest_modifiedIndexedPorts0, //8
    SetColorModeTest_modifiedIndexedPorts0, //9
    SetColorModeTest_modifiedIndexedPorts0, //10
    SetColorModeTest_modifiedIndexedPorts0, //11
    SetColorModeTest_modifiedIndexedPorts0, //12
    SetColorModeTest_modifiedIndexedPorts0, //13
    SetColorModeTest_modifiedIndexedPorts0, //14
    SetColorModeTest_modifiedIndexedPorts0, //15
    SetColorModeTest_modifiedIndexedPorts0, //16
    SetColorModeTest_modifiedIndexedPorts0, //17
    SetColorModeTest_modifiedIndexedPorts0, //18
    SetColorModeTest_modifiedIndexedPorts0, //19
    SetColorModeTest_modifiedIndexedPorts0, //20
    SetColorModeTest_modifiedIndexedPorts0, //21
    SetColorModeTest_modifiedIndexedPorts0, //22
    SetColorModeTest_modifiedIndexedPorts0, //23
    SetColorModeTest_modifiedIndexedPorts0, //24
    SetColorModeTest_modifiedIndexedPorts0, //25
    SetColorModeTest_modifiedIndexedPorts1, //26
    SetColorModeTest_modifiedIndexedPorts2, //27
    SetColorModeTest_modifiedIndexedPorts0, //28
    SetColorModeTest_modifiedIndexedPorts0, //29
    SetColorModeTest_modifiedIndexedPorts0, //30
    SetColorModeTest_modifiedIndexedPorts0, //31
    SetColorModeTest_modifiedIndexedPorts0, //32
    SetColorModeTest_modifiedIndexedPorts0, //33
    SetColorModeTest_modifiedIndexedPorts0, //34
    SetColorModeTest_modifiedIndexedPorts0, //35
    SetColorModeTest_modifiedIndexedPorts0, //36
    SetColorModeTest_modifiedIndexedPorts1, //37
    SetColorModeTest_modifiedIndexedPorts3, //38
    SetColorModeTest_modifiedIndexedPorts4, //39
    SetColorModeTest_modifiedIndexedPorts2, //40
    SetColorModeTest_modifiedIndexedPorts3, //41
    SetColorModeTest_modifiedIndexedPorts4, //42
    SetColorModeTest_modifiedIndexedPorts2, //43
    SetColorModeTest_modifiedIndexedPorts3, //44
    SetColorModeTest_modifiedIndexedPorts4, //45
    SetColorModeTest_modifiedIndexedPorts2, //46
    SetColorModeTest_modifiedIndexedPorts3, //47
    SetColorModeTest_modifiedIndexedPorts4, //48
    SetColorModeTest_modifiedIndexedPorts1 //49
};

int SetColorModeTest_modifiedIndexedPortsCount[] =
{
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //0
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //1
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //2
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //3
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //4
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //5
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //6
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //7
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //8
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //9
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //10
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //11
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //12
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //13
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //14
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //15
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //16
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //17
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //18
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //19
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //20
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //21
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //22
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //23
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //24
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //25
    sizeof(SetColorModeTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //26
    sizeof(SetColorModeTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //27
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //28
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //29
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //30
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //31
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //32
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //33
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //34
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //35
    sizeof(SetColorModeTest_modifiedIndexedPorts0) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //36
    sizeof(SetColorModeTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //37
    sizeof(SetColorModeTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //38
    sizeof(SetColorModeTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //39
    sizeof(SetColorModeTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //40
    sizeof(SetColorModeTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //41
    sizeof(SetColorModeTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //42
    sizeof(SetColorModeTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //43
    sizeof(SetColorModeTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //44
    sizeof(SetColorModeTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //45
    sizeof(SetColorModeTest_modifiedIndexedPorts2) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //46
    sizeof(SetColorModeTest_modifiedIndexedPorts3) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //47
    sizeof(SetColorModeTest_modifiedIndexedPorts4) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue), //48
    sizeof(SetColorModeTest_modifiedIndexedPorts1) / sizeof(Hag::Testing::Mock::PortAndIndexAndValue) //49
};
