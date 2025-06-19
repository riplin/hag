//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VideoProcessorConfiguration = 0x5c
    };
}

typedef uint32_t VideoProcessorConfiguration_t;
namespace VideoProcessorConfiguration
{
    enum //Mask
    {
        Processor                           = 0x00000001,
        Disable                             = 0x00000000,   // Video Processor off, VGA mode on.
        Enable                              = 0x00000001,   // Video Processor on, VGA mode off.
        CursorMode                          = 0x00000002,
        WindowsCursor                       = 0x00000000,
        X11Cursor                           = 0x00000002,
        OverlayStereo                       = 0x00000004,
        OverlayStereoDisable                = 0x00000000,
        OverlayStereoEnable                 = 0x00000004,
        Interlacing                         = 0x00000008,
        InterlaceDisable                    = 0x00000000,   // Interlace is not supported.
        HalfMode                            = 0x00000010,
        HalfModeDisabled                    = 0x00000000,
        HalfModeEnabled                     = 0x00000010,   // Enabled where desktop stride is added every other lines.
        ChromaKey                           = 0x00000020,
        ChromaKeyDisabled                   = 0x00000000,
        ChromaKeyEnabled                    = 0x00000020,
        ChromaKeyResult                     = 0x00000040,   // ChromaKeyResultInversion
        ChromaKeyResultNormal               = 0x00000000,   // Desktop transparent if desktop color matches or falls
                                                            // within the chroma-key color range.
        ChromaKeyResultInverted             = 0x00000040,   // Desktop transparent if desktop color does not
                                                            // match or fall within the chroma-key range.
        DesktopSurface                      = 0x00000080,
        DesktopDisable                      = 0x00000000,   // do not fetch the desktop surface
        DesktopEnable                       = 0x00000080,   // fetch desktop surface
        OverlaySurface                      = 0x00000100,
        OverlayDisable                      = 0x00000000,   // do not fetch the overlay surface
        OverlayEnable                       = 0x00000100,   // fetch overlay surface
        VideoInAsOverlay                    = 0x00000200,   // Video-in data displayed as overlay
        VideoInAsOverlayDisable             = 0x00000000,   // do not display the video-in buffer directly as overlay.
        VideoInAsOverlayEnable              = 0x00000200,   // use the video-in buffer address as the overlay start address (auto-flipping)
        DesktopCLUT                         = 0x00000400,   // Desktop CLUT bypass.
        DesktopCLUTDisable                  = 0x00000400,   // bypass the CLUT
        DesktopCLUTEnable                   = 0x00000000,   // do not bypass the CLUT in the RAMDAC
        OverlayCLUT                         = 0x00000800,   // Overlay CLUT bypass
        OverlayCLUTDisable                  = 0x00000800,   // bypass the CLUT
        OverlayCLUTEnable                   = 0x00000000,   // do not bypass the CLUT in the RAMDAC
        DesktopCLUTSelect                   = 0x00001000,
        DesktopCLUTLower                    = 0x00000000,   // use the lower 256 entries of the clut
        DesktopCLUTUpper                    = 0x00001000,   // use the upper 256 entries
        OverlayCLUTSelect                   = 0x00002000,
        OverlayCLUTLower                    = 0x00000000,   // use the lower 256 entries of the clut
        OverlayCLUTUpper                    = 0x00002000,   // use the upper 256 entries
        OverlayHorizontalScaling            = 0x00004000,   // Overlay horizontal scaling enable. Magnification factor determined by vidOverlayDudx.
        OverlayHorizontalScalingDisabled    = 0x00000000,
        OverlayHorizontalScalingEnabled     = 0x00004000,
        OverlayVerticalScaling              = 0x00008000,   // Overlay vertical scaling enable. Magnification factor determined by vidOverlayDvdy.
        OverlayVerticalScalingDisabled      = 0x00000000,
        OverlayVerticalScalingEnabled       = 0x00008000,
        OverlayFilter                       = 0x00030000,
        OverlayFilterPointSample            = 0x00000000,
        OverlayFilter2x2Dither              = 0x00010000,   // 2x2 dither subtract followed by 2x2 box filter (for 3d only)
        OverlayFilter4x4Dither              = 0x00020000,   // 4x4 dither subtract followed by 4x1 tap filter (for 3d only)
        OverlayFilterBilinearScaling        = 0x00030000
    };

    namespace Shift
    {
        enum
        {
            Processor                           = 0x00,
            CursorMode                          = 0x01,
            OverlayStereo                       = 0x02,
            Interlacing                         = 0x03,
            HalfMode                            = 0x04,
            ChromaKey                           = 0x05,
            ChromaKeyResult                     = 0x06,
            DesktopSurface                      = 0x07,
            OverlaySurface                      = 0x08,
            VideoInAsOverlay                    = 0x09,
            DesktopCLUT                         = 0x0a,
            OverlayCLUT                         = 0x0b,
            DesktopCLUTSelect                   = 0x0c,
            OverlayCLUTSelect                   = 0x0d,
            OverlayHorizontalScaling            = 0x0e,
            OverlayVerticalScaling              = 0x0f,
            OverlayFilter                       = 0x10
        };
    }
}

}