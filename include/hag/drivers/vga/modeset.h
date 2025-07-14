//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <has/types.h>
#include <has/ialloc.h>

#define HAG_IMPORT_MODESETTING                                                     \
    using namespace VGA::ModeSetting;                                              \
    namespace Flags { using namespace VGA::ModeSetting::Flags; }                   \
    namespace RefreshRate { using namespace VGA::ModeSetting::RefreshRate; }       \
    namespace BitsPerPixel { using namespace VGA::ModeSetting::BitsPerPixel; }     \
    namespace SetVideoError { using namespace VGA::ModeSetting::SetVideoError; }

namespace Hag::VGA::ModeSetting
{

    typedef uint8_t BitsPerPixel_t;
    namespace BitsPerPixel
    {
        enum
        {
            Bpp1 = 0x01,
            Bpp2 = 0x02,
            Bpp4 = 0x04,
            Bpp8 = 0x08,
            Bpp15 = 0x0F,
            Bpp16 = 0x10,
            Bpp24 = 0x18,
            Bpp32 = 0x20
        };
    }

    typedef uint32_t Flags_t;
    namespace Flags
    {
        enum
        {
            Mode =                  0x00000001, // Mode type is text or graphics
            Graphics =              0x00000000,
            Text =                  0x00000001,

            Chromacity =            0x00000002, // Monochrome or color
            Color =                 0x00000000,
            Monochrome =            0x00000002,

            MemoryOrganization =    0x00000004, // Sequential or planar memory organization, only applies to graphical modes
            Sequential =            0x00000000,
            Planar =                0x00000004,

            LinearFramebuffer =     0x00000008, // Linear framebuffer access

            HardwareAcceleration =  0x00000030, // 2D acceleration, 3D acceleration (not mutually exclusive)
            Accelerate2D =          0x00000010,
            Accelerate3D =          0x00000020
        };
    }

    typedef uint8_t RefreshRate_t;
    namespace RefreshRate
    {
        enum
        {
            DontCare = 0,
            R60Hz = 60,
            R70Hz = 70,
            R72Hz = 72
        };
    }

    typedef uint8_t SetVideoError_t;
    namespace SetVideoError
    {
        enum
        {
            Success = 0x00,
            SystemNotInitialized = 0x01,
            UnknownMode = 0x02,
            InsufficientVideoMemory = 0x03,
            RefreshRateNotSupported = 0x04,
            NotSupportedByRamdac = 0x05,
            NotSupportedByMonitor = 0x06,
            UnsupportedBufferCount = 0x07,
            DepthBufferNotSupported = 0x08,
            DepthBufferFormatNotSupported = 0x09,
        };
        extern const char* ToString(SetVideoError_t error);
    }

    typedef uint8_t Buffers_t;
    namespace Buffers
    {
        enum
        {
            ImageBuffers = 0x03,
            SingleBuffer = 0x01,
            DoubleBuffer = 0x02,
            TripleBuffer = 0x03,
            DepthBuffer = 0x0c,
            DepthNone = 0x00,
            Depth8Bpp = 0x04,
            Depth16Bpp = 0x08,
            Depth32Bpp = 0x0c
        };
    }

    bool Initialize(Has::IAllocator& allocator);
    void Shutdown();

    typedef std::function<bool(uint16_t width, uint16_t height, uint16_t stride, BitsPerPixel_t bpp, Flags_t flags, uint16_t segment)> VideoModeCallback_t;//Return true to continue receiving modes.
    void EnumerateVideoModes(const VideoModeCallback_t& callback);

    SetVideoError_t HasVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare);
    SetVideoError_t SetVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare, bool clearDisplay = true, Buffers_t buffers = Buffers::SingleBuffer);

    void SwapScreen2D(bool waitForVSync);

    void* GetLinearFrameBuffer();

    template <typename T> T* GetLinearFrameBufferAs() { return (T*)GetLinearFrameBuffer(); }
}