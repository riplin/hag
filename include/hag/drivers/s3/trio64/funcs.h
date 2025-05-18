//Copyright 2023-Present riplin

#pragma once

#include <hag/system/bda.h>
#include <hag/drivers/s3/shared/data.h>
#include <hag/drivers/s3/trio64/data.h>
#include <hag/drivers/s3/trio64/trio64.h>

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


bool GetVideoParameterBlockElement(uint16_t index, uint8_t*& returnPointer, uint16_t size = sizeof(FARPointer));
bool CheckValidInCurrentMode(uint8_t* ptr);
void SetGraphicsCharacterFont(System::BDA::GraphicsCharacterSet* graphicsCharacterFontDefinition);
void ModeSetBDA(VGA::VideoMode_t& mode, bool colorHardware, bool vesaModeNotColor);
bool VerifyBDAOrDeactivate(VGA::VideoMode_t& mode, bool isVesa, bool isVesaColor);

template<typename T>
bool GetVideoParameterBlockElementAs(uint16_t index, T*& returnPointer, uint16_t size = sizeof(FARPointer))
{
    uint8_t* ptr = NULL;
    bool ret = GetVideoParameterBlockElement(index, ptr, size);
    returnPointer = (T*)ptr;
    return ret;
}

Register_t GetCRTControllerIndexRegister();

void EnableVideoDisplay();
void PrepareAttributeController();

void ApplyVideoParameters(VGA::VideoParameters* parameters);
void SaveDynamicParameterData(VGA::VideoParameters* parameters);
void SetTextModeBiosData(VideoMode_t mode, VGA::VideoParameters* parameters);

void ClearScreen(VideoMode_t mode, bool ifVesaIsColor);

void UploadFont(uint8_t* src, uint8_t* dst, uint16_t characters, uint8_t bytesPerCharacter, uint16_t destWidth);
void PatchFont(uint8_t flags);
void SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank);
void ConfigureCursorPropertiesAndVerticalDisplayEnd(VideoMode_t mode, uint8_t characterPointHeight);
void SetFont();
void ConfigureFontAndCursor(VideoMode_t mode, System::BDA::AlphanumericCharSet* fontDefinition);
void ConfigureFontRamBank(System::BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* fontDefinition);

uint8_t* DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue);
uint16_t SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue);
uint16_t ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData);
void Set248ColorPalette();
void SetPalette(VideoMode_t mode, bool ifVesaIsColor, bool ifVesaIs256Color);
void SetPaletteProfile(Register_t crtcPort);

Shared::VESAVideoModeData* FindVideoModeData(VideoMode_t mode);
VGA::VideoParameters* GetVideoParameters(VideoMode_t mode);

void* GetLinearFrameBufferInternal(Shared::VESAVideoModeData* vesaData);

inline void* GetLinearFrameBuffer()
{
    return GetLinearFrameBufferInternal(FindVideoModeData(Hag::System::BDA::DisplayMode::Get()));
}

template<typename T>
T* GetLinearFrameBufferAs()
{
    return (T*)GetLinearFrameBuffer();
}

VideoModeError_t SetLegacyVideoModeInternal(VideoMode_t mode, Shared::VESAVideoModeData* vesaData);

inline VideoModeError_t SetLegacyVideoMode(VideoMode_t mode)
{
    return SetLegacyVideoModeInternal(mode, FindVideoModeData(mode & ~VideoMode::DontClearDisplay));
}

VideoModeError_t SetVesaVideoModeInternal(Vesa::VideoMode_t mode);
uint16_t GetDisplayMemoryInKiB();
uint8_t FetchBusSpecificSystemConfig(VGA::Register_t crtcPort);
void InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, VGA::Register_t crtcPort);
void WaitGraphicsEngineReady(VGA::Register_t crtcPort);
void ClearMemory(VGA::Register_t crtcPort);
void ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data);
void SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig);
uint8_t GetMemorySizeInMiB(VGA::Register_t crtcPort);
void ConfigureExtraVESAModeSettings(VideoMode_t mode, VGA::Register_t crtcPort, Shared::VESAVideoModeData* overrideTable, Shared::VESAResolutionVariant* modeData);
void Configure256KAddressingAndAddressWindow(VideoMode_t mode, VGA::Register_t crtcPort);
void SetColorMode(VideoMode_t mode, Shared::VESAColorMode_t colorMode, VGA::Register_t crtcPort);
void ApplyVESAOverrideData(VideoMode_t mode, VGA::Register_t crtcPort, Shared::VESAVideoModeData* overrideTable);

void WaitForVSync();
void SetDisplayStart(uint16_t x, uint16_t y);

void WaitGraphicsEngineReadyFast();
void SetScissors(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);
void DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void DrawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);

VideoMode_t ConvertVesaModeToLegacy(Vesa::VideoMode_t mode);
VideoModeError_t CheckValidVideoMode(VideoMode_t mode);

}}}
