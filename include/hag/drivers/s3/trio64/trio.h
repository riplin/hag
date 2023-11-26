//Copyright 2023-Present riplin

#pragma once

#include <hag/driver.h>
#include <hag/system/bda.h>
#include <hag/drivers/s3/trio64/trio64.h>

#pragma pack(push, 1);

namespace Hag { namespace S3 { namespace Trio64 
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

struct VideoParameters
{
    uint8_t DisplayedCharacterColumns;
    uint8_t DisplayedRowsMinus1;
    uint8_t CharacterMatrixHeightPoints;
    uint16_t VideoBufferSize;
    SequencerData_t SequencerRegisters[4];//SR1-SR4
    MiscellaneousOutput_t MiscOutputRegisterValues;
    CRTControllerData_t CRTCRegisters[25];//CR0-CR18
    AttributeControllerData_t AttributeControllerRegs[20];
    GraphicsControllerData_t GraphicsControllerRegs[9];//GR0-GR8
};

typedef uint8_t VESAModeAttributes_t;
namespace VESAModeAttributes
{
    enum
    {
        Supported = 0x01,               // bit 0 = Mode supported by present hardware configuration
        OptionalInfoAvailable = 0x02,   // bit 1 = Optional information available (must be =1 for VBE 1.2+)
        BIOSOutputSupported = 0x04,     // bit 2 = BIOS output supported
        Color = 0x08,                   // bit 3 = Set if color, clear if monochrome
        Graphics = 0x10,                // bit 4 = Set if graphics mode, clear if text mode
    };
}

typedef uint8_t VESAModeMemoryModel_t;
namespace VESAModeMemoryModel
{
    enum
    {
        Text = 0x00,                        // 00h    text
        CGAGraphics = 0x01,                 // 01h    CGA graphics
        HGCGraphics = 0x02,                 // 02h    HGC graphics
        EGA16ColorGraphics = 0x03,          // 03h    16-color (EGA) graphics
        PackedPixelGraphics = 0x04,         // 04h    packed pixel graphics
        Sequ256NonChain4Graphics = 0x05,    // 05h    "sequ 256" (non-chain 4) graphics
        DirectColor24bitGraphics = 0x06,    // 06h    direct color (HiColor, 24-bit color)
        YUVGraphics = 0x07                  // 07h    YUV (luminance-chrominance, also called YIQ)
        // 08h-0Fh reserved for VESA
        // 10h-FFh OEM memory models
    };
}

struct VESAModeInfo
{
    VESAModeAttributes_t ModeAttributes;
    uint16_t BytesPerScanline;
    uint16_t WidthInPixels;
    uint16_t HeightInPixels;
    uint8_t HeightOfCharacterCellInPixels;
    uint8_t MemoryPlanes;
    uint8_t BitsPerPixel;
    uint8_t Banks;
    VESAModeMemoryModel_t MemoryModelType;
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
        Color = 0x02,       // 1 = Always 1
        Palette256 = 0x04,  // 2 = unknown
        Unknown3 = 0x30     // 4 = 800x600 and text modes don't have these two bits set
    };
}

struct VESAVideoModeData
{
    VideoParameters* OverrideTable;
    VESAResolutionVariant* VariantData;
    VESAModeInfo* ModeInfo;
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
        Color = 0x01,
        Unknown1 = 0x02,
        Unknown2 = 0x04,
        Unknown3 = 0x08,
        Unknown4 = 0x10,
        Unknown5 = 0x20,
        Unknown6 = 0x40,
        Unknown7 = 0x80
    };
}

extern VESAVideoModeData VesaVideoModes[];


Register_t GetCRTControllerIndexRegister();
VESAVideoModeData* FindVideoModeData(VideoMode_t mode);
bool GetVideoModeFlags(VideoMode_t mode, VESAVideoModeFlags_t& flags);
void ModeSetBDA(VideoMode_t& mode);
bool VerifyBDAOrDeactivate(VideoMode_t& mode);
VideoParameters* GetVideoModeOverrideTable(VideoMode_t mode);

template<typename T>
static T* GetLinearFrameBufferAs()
{
    return (T*)GetLinearFrameBuffer();
}

VideoModeError_t SetLegacyVideoModeInternal(VideoMode_t mode);
VideoModeError_t SetVesaVideoModeInternal(Vesa::VideoMode_t mode);
uint16_t GetDisplayMemoryInKiB();
void* GetLinearFrameBuffer();
VideoParameters* GetCurrentVideoModeOverrideTable();
VideoParameters* SetTextModeBiosData(uint8_t mode);
void PrepareAttributeController();
void SaveDynamicParameterData(VideoParameters* overrideTable);
void ApplyVideoParameters(VideoParameters* overrideTable);
uint8_t FetchBusSpecificSystemConfig(VGA::Register_t crtcPort);
void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, VGA::Register_t crtcPort);
void WaitGraphicsEngineReady(VGA::Register_t crtcPort);
void ClearMemory(VGA::Register_t crtcPort);
void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data);
void SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig);
uint8_t GetMemorySizeInMiB(VGA::Register_t crtcPort);
void ConfigureExtraVESAModeSettings(VideoMode_t mode, VGA::Register_t crtcPort, VESAVideoModeData* overrideTable, VESAResolutionVariant* modeData);
void Configure256KAddressingAndAddressWindow(VideoMode_t mode, VGA::Register_t crtcPort);
void SetColorMode(VideoMode_t mode, ColorMode_t colorMode, VGA::Register_t crtcPort);
void ApplyVESAOverrideData(VideoMode_t mode, VGA::Register_t crtcPort, VESAVideoModeData* overrideTable);
void UploadFont(uint8_t* src, uint8_t* dst, uint16_t characters, uint8_t bytesPerCharacter, uint16_t destWidth);
void PatchFont(uint8_t flags);
void SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank);
void ConfigureCursorPropertiesAndVerticalDisplayEnd(VideoMode_t mode, uint8_t characterPointHeight);
void SetFont();
void ConfigureFontAndCursor(VideoMode_t mode, System::BDA::AlphanumericCharSet* fontDefinition);
void ConfigureFontRamBank(System::BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* fontDefinition);
void ClearScreen(VideoMode_t mode);
void SetPaletteProfile(Hag::VGA::Register_t crtcPort);

void WaitForVSync();
void SetDisplayStart(uint16_t x, uint16_t y);

void WaitGraphicsEngineReadyFast();
void SetScissors(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);

VideoMode_t ConvertVesaModeToLegacy(Vesa::VideoMode_t mode);
VideoModeError_t CheckValidVideoMode(VideoMode_t mode);

struct VesaModeTranslation
{
    Vesa::VideoMode_t VesaMode;
    VideoMode_t LegacyMode;
};

 extern VesaModeTranslation VideoModeTranslation[];
 extern VideoParameters* VesaResolutions[];
 extern VideoParameters LegacyVideoModes[];
 extern FirmwareFlag_t FirmwareFlags;

extern uint8_t VideoModeOverrideTranslationTable1[];
extern uint8_t VideoModeOverrideTranslationTable2[];
extern uint8_t VideoModeOverrideTranslationTable3[];

extern uint8_t CRTControllerInitData[];
extern uint8_t SequenceInitData[];
extern uint8_t PCISystemConfig;
extern uint8_t VLBSystemConfig;

extern uint8_t ClockData[];
extern uint8_t ClockDataRev3[];

class Class : public Hag::Driver//TODO: Remove this class.
{
public:
    Class(IAllocator& allocator)
        : Hag::Driver(allocator)
    {

    }

    virtual ~Class();

};

}}}

#pragma pack(pop);
