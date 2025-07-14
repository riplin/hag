//Copyright 2025-Present riplin

#include <stdio.h>
#include <has/farptr.h>
#include <hag/testing/testpat.h>
#include <hag/drivers/vga/modeset.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>

namespace Hag::Testing::TestPatterns
{

void TestVideoModes()
{
    using namespace Hag::VGA::ModeSetting;

    EnumerateVideoModes([](uint16_t width, uint16_t height, uint16_t stride, BitsPerPixel_t bpp, Flags_t flags, uint16_t segment)
    {
        static RefreshRate_t RefreshRates[] = { RefreshRate::R60Hz, RefreshRate::R70Hz, RefreshRate::R72Hz };
        for (uint32_t i = 0; i < sizeof(RefreshRates) / sizeof(RefreshRate_t); ++i)
        {
            RefreshRate_t refreshRate = RefreshRates[i];
            SetVideoError_t error = HasVideoMode(width, height, bpp, flags, refreshRate);

            if (error == SetVideoError::RefreshRateNotSupported)
                continue;

            if (error != SetVideoError::Success)
            {
                SetVideoMode(80, 25, BitsPerPixel::Bpp4, Flags::Text);
                printf("Error setting video mode %ix%ix%ibpp@%iHz\n", width, height, bpp, refreshRate);
                printf("Aborting...\n");
                return false;
            }
            SetVideoMode(width, height, bpp, flags, refreshRate);

            if ((flags & Flags::Mode) == Flags::Text)
            {
                DrawTextPattern(width, height, FARPointer(segment, 0x0000).ToPointer<uint8_t>());
            }
            else
            {
                uint8_t* ptr;
                if ((flags & Flags::LinearFramebuffer) != 0)
                    ptr = GetLinearFrameBufferAs<uint8_t>();
                else
                    ptr = FARPointer(segment, 0x0000).ToPointer<uint8_t>();
                if (ptr == nullptr)
                {
                    SetVideoMode(80, 25, BitsPerPixel::Bpp4, Flags::Text);
                    printf("Error retrieving frame buffer for video mode %ix%ix%ibpp@%iHz\n", width, height, bpp, refreshRate);
                    printf("Aborting...\n");
                    return false;
                }
                switch (bpp)
                {
                case BitsPerPixel::Bpp1:
                    if (segment == 0xa000)
                        Draw1BppPattern(width, height, ptr);
                    else
                        Draw1BppPattern2(width, height, ptr);
                break;
                case BitsPerPixel::Bpp2:
                    if ((flags & Flags::MemoryOrganization) == Flags::Planar)
                        Draw2BppPlanarPattern(width, height, ptr);
                    else
                        Draw2BppPattern(width, height, ptr);
                break;
                case BitsPerPixel::Bpp4:
                    Draw4BppPattern(width, height, ptr);
                    break;
                case BitsPerPixel::Bpp8:
                    if ((flags & Flags::MemoryOrganization) == Flags::Planar)
                        Draw8BppPlanarPattern(width, height, ptr);
                    else
                        Draw8BppPattern(width, height, ptr);
                    break;
                case BitsPerPixel::Bpp15:
                    Draw15BppPattern(width, height, ptr);
                    break;
                case BitsPerPixel::Bpp16:
                    Draw16BppPattern(width, height, ptr);
                    break;
                case BitsPerPixel::Bpp24:
                    Draw24BppPattern(width, height, stride, ptr);
                    break;
                case BitsPerPixel::Bpp32:
                    Draw32BppPattern(width, height, ptr);
                    break;
                default:
                    break;
                }
            }
            printf("Mode %ix%ix%ibpp@%iHz\n", width, height, bpp, refreshRate);
            getchar();
            SetVideoMode(80, 25, BitsPerPixel::Bpp4, Flags::Text);
        }
        return true;
    });
}

struct CharAndAttr
{
    uint8_t Char;
    uint8_t Attr;
};

typedef uint8_t BorderThickness_t;
namespace BorderThickness
{
    enum
    {
        Left = 0x01,
        LeftThin = 0x00,
        LeftThick = 0x01,
        Top = 0x02,
        TopThin = 0x00,
        TopThick = 0x02,
        Right = 0x04,
        RightThin = 0x00,
        RightThick = 0x04,
        Bottom = 0x08,
        BottomThin = 0x00,
        BottomThick = 0x08
    };
}

uint8_t HorBorder[2]   = { 0xB3 /* │ */, 0xBA /* ║ */ };
uint8_t VerBorder[2]   = { 0xC4 /* ─ */, 0xCD /* ═ */ };
uint8_t TopLeft[4]     = { 0xDA /* ┌ */, 0xD6 /* ╓ */, 0xD5 /* ╒ */, 0xC9 /* ╔ */ };
uint8_t TopRight[4]    = { 0xBF /* ┐ */, 0xB7 /* ╖ */, 0xB8 /* ╕ */, 0xBB /* ╗ */ };
uint8_t BottomRight[4] = { 0xD9 /* ┘ */, 0xBD /* ╜ */, 0xBE /* ╛ */, 0xBC /* ╝ */ };
uint8_t BottomLeft[4]  = { 0xC0 /* └ */, 0xD3 /* ╙ */, 0xD4 /* ╘ */, 0xC8 /* ╚ */ };

void DrawTextBox(uint16_t x0, uint16_t y0,
                 uint16_t x1, uint16_t y1,
                 BorderThickness_t borderThickness,
                 uint8_t attribute,
                 uint16_t screenWidth, uint8_t* videoMemory)
{
    using namespace Has;
    
    uint16_t top = min<uint16_t>(y0, y1);
    uint16_t bottom = max<uint16_t>(y0, y1) - top;
    uint16_t left = min<uint16_t>(x0, x1);
    uint16_t right = max<uint16_t>(x0, x1) - left;

    if (bottom == 0 || right == 0)
        return;

    uint8_t leftIdx   = (borderThickness & BorderThickness::Left)   >> 0;
    uint8_t topIdx    = (borderThickness & BorderThickness::Top)    >> 1;
    uint8_t rightIdx  = (borderThickness & BorderThickness::Right)  >> 2;
    uint8_t bottomIdx = (borderThickness & BorderThickness::Bottom) >> 3;

    uint8_t topLeftIdx     = leftIdx  | (topIdx << 1);
    uint8_t topRightIdx    = rightIdx | (topIdx << 1);
    uint8_t bottomLeftIdx  = leftIdx  | (bottomIdx << 1);
    uint8_t bottomRightIdx = rightIdx | (bottomIdx << 1);

    uint8_t leftChar = HorBorder[leftIdx];
    uint8_t rightChar = HorBorder[rightIdx];
    uint8_t topChar = VerBorder[topIdx];
    uint8_t bottomChar = VerBorder[bottomIdx];

    uint8_t topLeftChar = TopLeft[topLeftIdx];
    uint8_t topRightChar = TopRight[topRightIdx];
    uint8_t bottomLeftChar = BottomLeft[bottomLeftIdx];
    uint8_t bottomRightChar = BottomRight[bottomRightIdx];

    CharAndAttr* screen = (CharAndAttr*)videoMemory;
    screen += (screenWidth * top) + left;

    screen[0].Char = topLeftChar;
    screen[0].Attr = attribute;
    screen[bottom * screenWidth].Char = bottomLeftChar;
    screen[bottom * screenWidth].Attr = attribute;

    screen[right].Char = topRightChar;
    screen[right].Attr = attribute;
    screen[right + bottom * screenWidth].Char = bottomRightChar;
    screen[right + bottom * screenWidth].Attr = attribute;

    for (uint16_t idx = 1; idx < right; ++idx)
    {
        screen[idx].Char = topChar;
        screen[idx].Attr = attribute;

        screen[idx + bottom * screenWidth].Char = bottomChar;
        screen[idx + bottom * screenWidth].Attr = attribute;
    }

    for (uint16_t idx = 1; idx < bottom; ++idx)
    {
        screen[idx * screenWidth].Char = leftChar;
        screen[idx * screenWidth].Attr = attribute;

        screen[right + idx * screenWidth].Char = rightChar;
        screen[right + idx * screenWidth].Attr = attribute;
    }

    for (uint16_t innerY = 1; innerY < bottom; ++innerY)
    {
        for (uint16_t innerX = 1; innerX < right; ++innerX)
        {
            screen[innerY * screenWidth + innerX].Char = 0x20;
            screen[innerY * screenWidth + innerX].Attr = attribute;
        }
    }
}

void DrawTextPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    DrawTextBox(0, 0, width - 1, height - 1,
        BorderThickness::LeftThin | BorderThickness::TopThin | BorderThickness::RightThick | BorderThickness::BottomThick,
        0x1E, width, videoMemory);

    DrawTextBox(3, 3, 20, 20,
        BorderThickness::LeftThick | BorderThickness::TopThick | BorderThickness::RightThin | BorderThickness::BottomThin,
        0x1C, width, videoMemory);

    CharAndAttr* screen = (CharAndAttr*)videoMemory;

    screen += 4 * width + 4;

    for (uint16_t y = 0; y < 16; ++y)
    {
        for (uint16_t x = 0; x < 16; ++x)
        {
            screen[y * width + x].Char =  y * 8 + x;
            screen[y * width + x].Attr = (y << 4) | x;
        }
    }
}

void Draw1BppPattern2(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 3;
    uint16_t divisor = pixelWidth >> 2;//4 color bands
    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t realY = y >> 1;
        uint8_t* realMemory = (y & 1) == 0x00 ? videoMemory : videoMemory + 0x2000;
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = 0;
            switch((x / divisor) & 0x0003)
            {
                case 1:
                    color = (y & 2) == 0 ? 0x33 : 0xCC;
                    break;
                case 2:
                    color = (y & 1) == 0 ? 0x55 : 0xAA;
                    break;
                case 3:
                    color = 0xFF;
                    break;
                default:
                    break;
            }
            realMemory[realY * pixelWidth + x] = color | borderColor;
        }
    }
}

void Draw1BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 3;
    uint16_t divisor = pixelWidth >> 2;//4 color bands
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = 0;
            switch((x / divisor) & 0x0003)
            {
                case 1:
                    color = (y & 2) == 0 ? 0x33 : 0xCC;
                    break;
                case 2:
                    color = (y & 1) == 0 ? 0x55 : 0xAA;
                    break;
                case 3:
                    color = 0xFF;
                    break;
                default:
                    break;
            }
            videoMemory[y * pixelWidth + x] = color | borderColor;
        }
    }
}

void Draw2BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 2;
    uint16_t divisor = pixelWidth >> 2;//4 color bands
    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t realY = y >> 1;
        uint8_t* realMemory = (y & 1) == 0x00 ? videoMemory : videoMemory + 0x2000;
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xC0 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x03 : 0x00;
            uint8_t color = x / divisor;
            color |= color << 2;
            color |= color << 4;
            realMemory[realY * pixelWidth + x] = color | borderColor;
        }
    }
}

void Draw2BppPlanarPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    using namespace Hag::VGA;

    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 3;
    uint16_t divisor = pixelWidth >> 4;//16 color bands

    Sequencer::EnableWritePlane::Write(0x01);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane0 = ((color & 1) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane0;
        }
    }

    Sequencer::EnableWritePlane::Write(0x04);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane1 = ((color & 2) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane1;
        }
    }
}


void Draw4BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    using namespace Hag::VGA;

    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 3;
    uint16_t divisor = pixelWidth >> 4;//16 color bands

    Sequencer::EnableWritePlane::Write(0x01);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane0 = ((color & 1) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane0;
        }
    }

    Sequencer::EnableWritePlane::Write(0x02);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane1 = ((color & 2) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane1;
        }
    }

    Sequencer::EnableWritePlane::Write(0x04);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane2 = ((color & 4) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane2;
        }
    }

    Sequencer::EnableWritePlane::Write(0x08);
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == uint32_t(pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane3 = ((color & 8) == 0 ? 0x00 : 0xFF) | borderColor;
            videoMemory[y * pixelWidth + x] = plane3;
        }
    }
}

void Draw8BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0x0F : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0x0F : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x0F : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0x0F : 0x00;
            uint8_t color = borderColor == 0x00 ? uint8_t(x) : borderColor;
            videoMemory[y * width + x] = color;
        }
    }
}

void Draw8BppPlanarPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    using namespace Hag::VGA;

    if (videoMemory == nullptr)
        return;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0x0F : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0x0F : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x0F : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0x0F : 0x00;
            uint8_t color = borderColor == 0x00 ? uint8_t(x) : borderColor;
            Sequencer::EnableWritePlane::Write(0x01 << ((y * width + x) & 0x03));
            videoMemory[(y * width + x) >> 2] = color;
        }
    }
}

void Draw15BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t* mem = (uint16_t*)videoMemory;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t borderColorY = 0x0000;
        borderColorY = y == 0 ? 0x7FFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0x7FFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint16_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x7FFF : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0x7FFF : 0x00;
            uint32_t blue = ((y + 1) * 0x1F) / height;
            uint32_t red = 0x1f - blue;
            uint32_t green = ((x + 1) * 0x1F) / width;
            uint16_t color = uint16_t(blue << 10) | uint16_t(green << 5) | uint16_t(red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void Draw16BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t* mem = (uint16_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t borderColorY = 0x0000;
        borderColorY = y == 0 ? 0xFFFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint16_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFF : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0xFFFF : 0x00;
            uint32_t blue = ((y + 1) * 0x1F) / height;
            uint32_t red = 0x1f - blue;
            uint32_t green = ((x + 1) * 0x3F) / width;
            uint16_t color = uint16_t(blue << 11) | uint16_t(green << 5) | uint16_t(red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void Draw24BppPattern(uint16_t width, uint16_t height, uint16_t stride, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFF : borderColorY;
        uint8_t* mem = videoMemory + y * stride;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFFFFFF : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0xFFFFFFFF : 0x00;
            uint8_t blue = uint8_t(((y + 1) * 0xFF) / height);
            uint32_t red = uint8_t(0xFF - blue);
            uint32_t green = uint8_t(((x + 1) * 0xFF) / width);
            *(mem++) = borderColor == 0x00 ? red : borderColor;
            *(mem++) = borderColor == 0x00 ? green : borderColor;
            *(mem++) = borderColor == 0x00 ? blue : borderColor;
        }
    }
}

void Draw32BppPattern(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint32_t* mem = (uint32_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint32_t borderColorY = 0x00000000;
        borderColorY = y == 0 ? 0xFFFFFFFF : borderColorY;
        borderColorY = y == uint32_t(height - 1) ? 0xFFFFFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint32_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFFFFFF : 0x00;
            borderColor |= x == uint32_t(width - 1) ? 0xFFFFFFFF : 0x00;
            uint32_t blue = ((y + 1) * 0xFF) / height;
            uint32_t red = 0xFF - blue;
            uint32_t green = ((x + 1) * 0xFF) / width;
            uint32_t color = (blue << 16) | (green << 8) | (red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

}