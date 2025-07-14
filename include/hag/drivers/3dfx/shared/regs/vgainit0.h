//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::TDfx::Shared
{

namespace Register
{
    enum
    {
        VGAInit0 = 0x28
    };
}

typedef uint32_t VGAInit0_t;
namespace VGAInit0
{
    enum //Mask
    {
        Operation =                      0x000001,  // VGA disable. Setting this bit to 1 shuts off all access to the VGA core.
        Enable =                         0x000000,  // 0 = Enable
        Disable =                        0x000001,  // 1 = Disable
        TimingSource =                   0x000002,  // Use external video timing. This bit is used to retrieve SYNC information
        TimingInternal =                 0x000000,  // through the normal VGA mechanism when the VGA CRTC is not providing
        TimingExternal =                 0x000002,  // timing control.
        LutBitDepth =                    0x000004,  // VGA 6/8 bit CLUT.
        LutBitDepth6 =                   0x000000,  // 0 = 6 bit
        LutBitDepth8 =                   0x000004,  // 1 = 8 bit
        Extensions =                     0x000040,  // Enable VGA Extensions 
        ExtensionsDisabled =             0x000000,
        ExtensionsEnabled =              0x000040,
        WakeUpSelect =                   0x000100,  // 0x46e8/0x3C3 Wake up select. VGA add in cards that use 0x46e8 while 
                                                    // mother board VGA uses 0x3C3. When Banshee is a multimedia device,
                                                    // this bit should be set to ‘1’ and the VGA subsystem should be enabled
                                                    // with IO Base + 0xC3.
        WakeUpSelect46e8 =               0x000000,  // 0 = use 0x46e8
        WakeUpSelect3c3 =                0x000100,  // 1 = use 0x3C3 or IO Base + 0xC3
        LegacyIOMemoryDecode =           0x000200,  // Disable VGA Legacy Memory/IO Decode
        LegacyDecodeEnable =             0x000000,  // 0 = Enable
        LegacyDecodeDisable =            0x000200,  // 1 = Disable
        AlternateConfigReadback =        0x000400,  // Use alternate VGA Config read back. Setting this bit to 0 allows the
                                                    // VGA to read back configuration through CRTC index 0x1c.
        AlternateReadbackEnable =        0x000000,  // 0 = Enable
        AlternateReadbackDisable =       0x000400,  // 1 = Disable
        FastBlink =                      0x000800,  // Enable Fast Blink (test bit).
        FastBlinkDisable =               0x000000,  // 0 = normal blink
        FastBlinkEnable =                0x000800,  // 1 = fast blink
        MemoryAccess =                   0x001000,  // Use extended video shift out. Set this bit to 1 to disable all VGA memory
        MemoryAccessEnabled =            0x000000,  // access when video processor is shifting out data.
        MemoryAccessDisabled =           0x001000,
        Decode3c6 =                      0x002000,  // Decode 3c6. (test bit) 
        Decode3c6Disabled =              0x000000,  // 0 = Disable
        Decode3c6Enabled =               0x002000,  // 1 = Enable
        BaseOffset =                     0x3FC000,  // Vga base offset in 64k quantities
        SGRAMHBlankRefresh =             0x400000,  // Disable SGRAM refresh requests on HBLANK. When set to 1, the VGA
        SGRAMHBlankRefreshDisable =      0x000000,  // does not produce memory refreshes during horizontal blanking.
        SGRAMHBlankRefreshEnable =       0x400000
    };

    namespace Shift
    {
        enum
        {
            Operation = 0x00,
            TimingSource = 0x01,
            LutBitDepth = 0x02,
            Extensions = 0x03,
            WakeUpSelect = 0x08,
            LegacyIOMemoryDecode = 0x09,
            AlternateConfigReadback = 0x0a,
            FastBlink = 0x0b,
            MemoryAccess = 0x0c,
            Decode3c6 = 0x0d,
            BaseOffset = 0x0e,
            SGRAMHBlankRefresh = 0x16
        };
    }
}

}