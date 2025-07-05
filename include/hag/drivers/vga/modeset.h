//Copyright 2025-Present riplin

#pragma once

#include <functional>
#include <hag/types.h>
#include <hag/ialloc.h>

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

    typedef uint16_t Flags_t;
    namespace Flags
    {
        enum
        {
            Mode = 0x01,                //Mode type is text or graphics
            Text = 0x01,
            Graphics = 0x00,

            Chromacity = 0x02,          //Monochrome or color
            Monochrome = 0x02,
            Color = 0x00,

            MemoryOrganization = 0x04,  //Sequential or planar memory organization, only applies to graphical modes
            Sequential = 0x00,
            Planar = 0x04,

            LinearFramebuffer = 0x08,   //Linear framebuffer access
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
        };
    }

    typedef uint8_t SetupBuffersError_t;
    namespace SetupBuffersError
    {
        enum
        {
            Success = 0x00,
            IllegalBufferCount = 0x01,
            ModeNotSet = 0x02,
            DepthBufferNotSupported = 0x03,
            DepthBufferFormatNotSupported = 0x04,
            NotEnoughMemory = 0x05,
        };
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

    bool Initialize(IAllocator& allocator);
    void Shutdown();

    typedef std::function<bool(uint16_t width, uint16_t height, uint16_t stride, BitsPerPixel_t bpp, Flags_t flags, uint16_t segment)> VideoModeCallback_t;//Return true to continue receiving modes.
    void EnumerateVideoModes(const VideoModeCallback_t& callback);

    SetVideoError_t HasVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare);
    SetVideoError_t SetVideoMode(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags = Flags::Sequential, RefreshRate_t refreshRate = RefreshRate::DontCare, bool clearDisplay = true);
    SetupBuffersError_t SetupBuffers(Buffers_t buffers);

    void SwapScreen2D(bool waitForVSync);

    void* GetLinearFrameBuffer();

    template <typename T> T* GetLinearFrameBufferAs() { return (T*)GetLinearFrameBuffer(); }
}