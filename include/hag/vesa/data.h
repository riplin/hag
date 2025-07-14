//Copyright 2023-Present riplin

#pragma once

#include <has/types.h>

namespace Hag::Vesa
{

typedef uint8_t ModeAttributes_t;

namespace ModeAttributes
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

typedef uint8_t ModeMemoryModel_t;
namespace ModeMemoryModel
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

#pragma pack(push, 1)

struct ModeInfo
{
    ModeAttributes_t ModeAttributes;
    uint16_t BytesPerScanline;
    uint16_t WidthInPixels;
    uint16_t HeightInPixels;
    uint8_t HeightOfCharacterCellInPixels;
    uint8_t MemoryPlanes;
    uint8_t BitsPerPixel;
    uint8_t Banks;
    ModeMemoryModel_t MemoryModelType;
};

#pragma pack(pop)

}