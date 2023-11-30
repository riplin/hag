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
