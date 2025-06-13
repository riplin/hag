//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/types.h>
#include <hag/farptr.h>
#include <i86.h>
#include <string.h>
#include <hag/math/fp/fpmath.h>
#include <hag/system/bda.h>
#include <hag/system/pci.h>
#include <hag/system/sysasm.h>
#include <support/allocatr.h>
#include <hag/drivers/s3/trio64/funcs.h>

Hag::S3::Trio64::Register_t Registers[] =
{
    Hag::S3::Trio64::Register::InputStatus0,
    Hag::S3::Trio64::Register::FeatureControlR,
    Hag::S3::Trio64::Register::MiscellaneousR,
    Hag::S3::Trio64::Register::InputStatus1B,
    Hag::S3::Trio64::Register::InputStatus1D,
    Hag::S3::Trio64::Register::VideoSubsystemEnable,
    Hag::S3::Trio64::Register::DACMask,
    Hag::S3::Trio64::Register::DACStatus,
    Hag::S3::Trio64::Register::SetupOptionSelect,
    Hag::S3::Trio64::Register::VideoSubsystemEnable,
    Hag::S3::Trio64::Register::SubsystemStatus,
    Hag::S3::Trio64::Register::AdvancedFunctionControl,
    Hag::S3::Trio64::Register::GraphicsProcessorStatus,
};

bool SetVideoMode(uint8_t mode);

void regdump(const char* filename)
{
    using namespace Hag::System::BDA;
    using namespace Hag::S3::Trio64;
    //Write code to CR38 to provide access to the S3 VGA registers (CR30-CR3F)
    CRTController::RegisterLock1::SoftUnlock rl1(VideoBaseIOPort::Get());
    
    //Write code to CR39 to provide access to the System Control and System Extension registers (CR40-CRFF)
    CRTController::RegisterLock2::SoftUnlock rl2(VideoBaseIOPort::Get());

    //Set bit 0 in CR40 to enable access to the Enhanced Commands registers.
    CRTController::SystemConfiguration::SoftUnlock sc(VideoBaseIOPort::Get());

    //Enable write access to bits 1 and 6 of CR7
    //Enable access to RAMDAC register
    //Enable access to Palette/Overscan registers
    CRTController::BackwardCompatibility2::SoftUnlock bc2(VideoBaseIOPort::Get());

    //Enable write access to CR0-CR6, CR7 (bits 7,5,3,2,0), CR9 (bit5), CR10, CR11 (bits 3-0), CR15-CR16, CR17 (bit 2)
    CRTController::CRTRegisterLock::SoftUnlock crl(VideoBaseIOPort::Get());

    //Enable write access to CR0-CR7
    CRTController::VerticalRetraceEnd::SoftUnlock vre(VideoBaseIOPort::Get());

    //write code to SR8 to provide access to SR9-SR18.
    Sequencer::UnlockExtendedSequencer::SoftUnlock ues;

    FILE* fp = fopen(filename, "w");

    fprintf(fp, "VGA and S3 registers:\n");
    for (uint32_t i = 0; i < sizeof(Registers) / sizeof(Register_t); ++i)
    {
        Register_t reg = Registers[i];
        uint8_t value = SYS_ReadPortByte(reg);
        fprintf(fp, "{ 0x%04X, 0x%02X },\n", reg, value);
    }

    fprintf(fp, "\nAttribute Controller registers:\n");
    SYS_ReadPortByte(Register::InputStatus1D);//Reset attribute controller to index register.
    uint8_t orgAttribIdx = SYS_ReadPortByte(Register::AttributeControllerIndex);
    for (uint8_t i = 0; i < 32; ++i)
    {
        uint8_t idx = (orgAttribIdx & 0xE0) | i;
        SYS_ReadPortByte(Register::InputStatus1D);//Reset attribute controller to index register.
        SYS_WritePortByte(Register::AttributeControllerIndex, idx);
        uint8_t value = SYS_ReadPortByte(Register::AttributeControllerDataR);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }
    SYS_ReadPortByte(Register::InputStatus1D);//Reset attribute controller to index register.
    SYS_WritePortByte(Register::AttributeControllerIndex, orgAttribIdx);

    fprintf(fp, "\nRAMDAC registers:\n");
    SYS_WritePortByte(Register::DACReadIndex, 0x00);
    for (uint16_t i = 0; i < 256; ++i)
    {
        uint8_t red = SYS_ReadPortByte(Register::RAMDACData);
        uint8_t green = SYS_ReadPortByte(Register::RAMDACData);
        uint8_t blue = SYS_ReadPortByte(Register::RAMDACData);
        fprintf(fp, "{ 0x%02X, 0x%02X, 0x%02X }, //%i\n", red, green, blue, i);
    }

    fprintf(fp, "\nSequencer Controller registers:\n");
    for (uint8_t i = 0; i < 32; ++i)
    {
        SYS_WritePortByte(Register::SequencerIndex, i);
        uint8_t value = SYS_ReadPortByte(Register::SequencerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nGraphics Controller registers:\n");
    for (uint8_t i = 0; i < 16; ++i)
    {
        SYS_WritePortByte(Register::GraphicsControllerIndex, i);
        uint8_t value = SYS_ReadPortByte(Register::GraphicsControllerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nCRT Controller registers:\n");
    for (uint16_t i = 0; i < 256; ++i)
    {
        SYS_WritePortByte(Register::CRTControllerIndexD, uint8_t(i));
        uint8_t value = SYS_ReadPortByte(Register::CRTControllerDataD);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", uint8_t(i), value);
    }

    fclose(fp);
}

void memdump(const char* filename, uint32_t pointer = 0)
{
    uint8_t* ptr = (uint8_t*)pointer;
    FILE* fp = fopen(filename, "wb");

    fwrite(ptr, 1, 0x10000, fp);

    fclose(fp);
}

void drawTestPattern1bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
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
        borderColorY = y == (height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == (pixelWidth - 1) ? 0x01 : 0x00;
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

void drawTestPattern2bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
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
        borderColorY = y == (height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xC0 : 0x00;
            borderColor |= x == (pixelWidth - 1) ? 0x03 : 0x00;
            uint8_t color = x / divisor;
            color |= color << 2;
            color |= color << 4;
            realMemory[realY * pixelWidth + x] = color | borderColor;
        }
    }
}

void drawTestPattern4bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    using namespace Hag::S3::Trio64;

    if (videoMemory == nullptr)
        return;

    uint16_t pixelWidth = width >> 3;
    uint16_t divisor = pixelWidth >> 4;//16 color bands
    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0xFF : borderColorY;
        borderColorY = y == (height - 1) ? 0xFF : borderColorY;
        for (uint32_t x = 0; x < pixelWidth; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x80 : 0x00;
            borderColor |= x == (pixelWidth - 1) ? 0x01 : 0x00;
            uint8_t color = x / divisor;
            uint8_t plane0 = ((color & 1) == 0 ? 0x00 : 0xFF) | borderColor;
            uint8_t plane1 = ((color & 2) == 0 ? 0x00 : 0xFF) | borderColor;
            uint8_t plane2 = ((color & 4) == 0 ? 0x00 : 0xFF) | borderColor;
            uint8_t plane3 = ((color & 8) == 0 ? 0x00 : 0xFF) | borderColor;
            
            Sequencer::EnableWritePlane::Write(0x01);
            videoMemory[y * pixelWidth + x] = plane0;

            Sequencer::EnableWritePlane::Write(0x02);
            videoMemory[y * pixelWidth + x] = plane1;

            Sequencer::EnableWritePlane::Write(0x04);
            videoMemory[y * pixelWidth + x] = plane2;

            Sequencer::EnableWritePlane::Write(0x08);
            videoMemory[y * pixelWidth + x] = plane3;
        }
    }
}

void drawTestPattern8bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint8_t borderColorY = 0x00;
        borderColorY = y == 0 ? 0x0F : borderColorY;
        borderColorY = y == (height - 1) ? 0x0F : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint8_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x0F : 0x00;
            borderColor |= x == (width - 1) ? 0x0F : 0x00;
            uint8_t color = borderColor == 0x00 ? uint8_t(x) : borderColor;
            videoMemory[y * width + x] = color;
        }
    }
    for (uint32_t i = 0; i < 20; ++i)
    {
        uint32_t x0 = i;
        uint32_t y0 = i;
        uint32_t x1 = (width - 1) - i;
        uint32_t y1 = i;
        uint32_t x2 = i;
        uint32_t y2 = (height - 1) - i;
        uint32_t x3 = (width - 1) - i;
        uint32_t y3 = (height - 1) - i;

        videoMemory[y0 * width + x0] = 0x0F;
        videoMemory[y1 * width + x1] = 0x0F;
        videoMemory[y2 * width + x2] = 0x0F;
        videoMemory[y3 * width + x3] = 0x0F;
    }
}

struct CharAndAttr
{
    uint8_t Char;
    uint8_t Attr;
};

void drawTestPatternText(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    CharAndAttr* screen = (CharAndAttr*)videoMemory;

    //│ 0xB3 //─ 0xC4 //└ 0xC0 //┘ 0xD9
    //┌ 0xDA //┐ 0xBF //┴ 0xC1 //┬ 0xC2
    //├ 0xC3 //┼ 0xC5 //┤ 0xB4
    
    //╙ 0xD3 //╜ 0xBD //╓ 0xD6 //╖ 0xB7
    //╨ 0xD0 //╥ 0xD2 //╟ 0xC7 //╫ 0xD7
    //╢ 0xB6 //╘ 0xD4 //╛ 0xBE //╒ 0xD5
    //╕ 0xB8 //╧ 0xCF //╤ 0xD1 //╞ 0xC6
    //╪ 0xD8 //╡ 0xB5
    
    //║ 0xBA //═ 0xCD //╚ 0xC8 //╝ 0xBC
    //╔ 0xC9 //╗ 0xBB //╩ 0xCA //╦ 0xCB
    //╠ 0xCC //╬ 0xCE //╣ 0xB9
    
    uint16_t divisor = width >> 4;
    screen[0].Char = 0xC9;
    screen[0].Attr = 0x1E;
    screen[width-1].Char = 0xBB;
    screen[width-1].Attr = 0x1E;
    screen[(height - 1) * width].Char = 0xC8;
    screen[(height - 1) * width].Attr = 0x1E;
    screen[(height - 1) * width + (width - 1)].Char = 0xBC;
    screen[(height - 1) * width + (width - 1)].Attr = 0x1E;
    for (uint32_t x = 1; x < width - 1; ++x)
    {
        screen[x].Char = 0xCD;
        screen[x].Attr = 0x1E;
        screen[(height - 1) * width + x].Char = 0xCD;
        screen[(height - 1) * width + x].Attr = 0x1E;
    }

    for (uint32_t y = 1; y < height - 1; ++y)
    {
        screen[y * width].Char = 0xBA;
        screen[y * width].Attr = 0x1E;
        screen[y * width + (width - 1)].Char = 0xBA;
        screen[y * width + (width - 1)].Attr = 0x1E;
    }

    for (uint32_t y = 1; y < height - 1; ++y)
    {
        for (uint32_t x = 1; x < width - 1; ++x)
        {
            uint8_t ch = (width - 2) * (y - 1) + (x - 1);
            uint8_t fcolor = x / divisor;
            uint8_t bcolor = 0x0F - fcolor;
            uint8_t color = fcolor | (bcolor << 4);
            screen[y * width + x].Char = ch;
            screen[y * width + x].Attr = color;
        }
    }
}

void drawTestPattern15bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t* mem = (uint16_t*)videoMemory;

    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t borderColorY = 0x0000;
        borderColorY = y == 0 ? 0x7FFF : borderColorY;
        borderColorY = y == (height - 1) ? 0x7FFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint16_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0x7FFF : 0x00;
            borderColor |= x == (width - 1) ? 0x7FFF : 0x00;
            uint32_t blue = ((y + 1) * 0x1F) / height;
            uint32_t red = 0x1f - blue;
            uint32_t green = ((x + 1) * 0x1F) / width;
            uint16_t color = uint16_t(blue << 10) | uint16_t(green << 5) | uint16_t(red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void drawTestPattern16bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t* mem = (uint16_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t borderColorY = 0x0000;
        borderColorY = y == 0 ? 0xFFFF : borderColorY;
        borderColorY = y == (height - 1) ? 0xFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint16_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFF : 0x00;
            borderColor |= x == (width - 1) ? 0xFFFF : 0x00;
            uint32_t blue = ((y + 1) * 0x1F) / height;
            uint32_t red = 0x1f - blue;
            uint32_t green = ((x + 1) * 0x3F) / width;
            uint16_t color = uint16_t(blue << 11) | uint16_t(green << 5) | uint16_t(red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void drawTestPattern16bpp2(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint16_t* mem = (uint16_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint16_t borderColorY = 0x0000;
        borderColorY = y == 0 ? 0xFFFF : borderColorY;
        borderColorY = y == (height - 1) ? 0xFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint16_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFF : 0x00;
            borderColor |= x == (width - 1) ? 0xFFFF : 0x00;
            uint32_t red = ((y + 1) * 0x1F) / height;
            uint32_t blue = 0x1f - red;
            uint32_t green = 0x3F - (((x + 1) * 0x3F) / width);
            uint16_t color = uint16_t(blue << 11) | uint16_t(green << 5) | uint16_t(red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void drawTestPattern32bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint32_t* mem = (uint32_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint32_t borderColorY = 0x00000000;
        borderColorY = y == 0 ? 0xFFFFFFFF : borderColorY;
        borderColorY = y == (height - 1) ? 0xFFFFFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint32_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFFFFFF : 0x00;
            borderColor |= x == (width - 1) ? 0xFFFFFFFF : 0x00;
            uint32_t blue = ((y + 1) * 0xFF) / height;
            uint32_t red = 0xFF - blue;
            uint32_t green = ((x + 1) * 0xFF) / width;
            uint32_t color = (blue << 16) | (green << 8) | (red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

void drawTestPattern32bpp2(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    if (videoMemory == nullptr)
        return;

    uint32_t* mem = (uint32_t*)videoMemory;
    for (uint32_t y = 0; y < height; ++y)
    {
        uint32_t borderColorY = 0x00000000;
        borderColorY = y == 0 ? 0xFFFFFFFF : borderColorY;
        borderColorY = y == (height - 1) ? 0xFFFFFFFF : borderColorY;
        for (uint32_t x = 0; x < width; ++x)
        {
            uint32_t borderColor = borderColorY;
            borderColor |= x == 0 ? 0xFFFFFFFF : 0x00;
            borderColor |= x == (width - 1) ? 0xFFFFFFFF : 0x00;
            uint32_t red = ((y + 1) * 0xFF) / height;
            uint32_t blue = 0xFF - red;
            uint32_t green = 0xFF - (((x + 1) * 0xFF) / width);
            uint32_t color = (blue << 16) | (green << 8) | (red);
            mem[y * width + x] = borderColor == 0x0000 ? color : borderColor;
        }
    }
}

typedef void (*TestPatternFunc)(uint16_t width, uint16_t height, uint8_t* videoMemory);

struct ModeTest
{
    uint8_t mode;
    uint16_t vesaMode;
    uint16_t width;
    uint16_t height;
    uint8_t* address;
    TestPatternFunc DrawTestPattern;
};

/*
01h = T  40x25  8x8   320x200   16       8   B800 CGA,PCjr,Tandy
    = T  40x25  8x14  320x350   16       8   B800 EGA
    = T  40x25  8x16  320x400   16       8   B800 MCGA
    = T  40x25  9x16  360x400   16       8   B800 VGA
02h = T  80x25  8x8   640x200  16gray    4   B800 CGA,PCjr,Tandy
    = T  80x25  8x14  640x350  16gray    8   B800 EGA
    = T  80x25  8x16  640x400   16       8   B800 MCGA
    = T  80x25  9x16  720x400   16       8   B800 VGA
03h = T  80x25  8x8   640x200   16       4   B800 CGA,PCjr,Tandy
    = T  80x25  8x14  640x350   16/64    8   B800 EGA
    = T  80x25  8x16  640x400   16       8   B800 MCGA
    = T  80x25  9x16  720x400   16       8   B800 VGA
    = T  80x43  8x8   640x350   16       4   B800 EGA,VGA [17]
    = T  80x50  8x8   640x400   16       4   B800 VGA [17]
07h = T  80x25  9x14  720x350  mono     var  B000 MDA,Hercules,EGA
    = T  80x25  9x16  720x400  mono      .   B000 VGA

04h = G  40x25  8x8   320x200    4       .   B800 CGA,PCjr,EGA,MCGA,VGA
05h = G  40x25  8x8   320x200   4gray    .   B800 CGA,PCjr,EGA
    = G  40x25  8x8   320x200    4       .   B800 MCGA,VGA
06h = G  80x25  8x8   640x200    2       .   B800 CGA,PCjr,EGA,MCGA,VGA
    = G  80x25   .       .     mono      .   B000 HERCULES.COM on HGC [14]

0Dh = G  40x25  8x8   320x200   16       8   A000 EGA,VGA
0Eh = G  80x25  8x8   640x200   16       4   A000 EGA,VGA
0Fh = G  80x25  8x14  640x350  mono      2   A000 EGA,VGA
10h = G  80x25  8x14  640x350    4       2   A000 64k EGA
    = G    .     .    640x350   16       .   A000 256k EGA,VGA
11h = G  80x30  8x16  640x480  mono      .   A000 VGA,MCGA,ATI EGA,ATI VIP
12h = G  80x30  8x16  640x480   16/256K  .   A000 VGA,ATI VIP
    = G  80x30  8x16  640x480   16/64    .   A000 ATI EGA Wonder
    = G    .     .    640x480   16       .     .  UltraVision+256K EGA
13h = G  40x25  8x8   320x200  256/256K  .   A000 VGA,MCGA,ATI VIP
*/

ModeTest modeTests[] =
{
    //Legacy modes:
    {Hag::S3::Trio64::VideoMode::T40x25x4bppC, 0x00, 40, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::S3::Trio64::VideoMode::T80x25x4bppG, 0x00, 80, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::S3::Trio64::VideoMode::T80x25x4bppC, 0x00, 80, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::S3::Trio64::VideoMode::G320x200x2bppC, 0x00, 320, 200, (uint8_t*)0xB8000, drawTestPattern2bpp},
    {Hag::S3::Trio64::VideoMode::G320x200x2bppG, 0x00, 320, 200, (uint8_t*)0xB8000, drawTestPattern2bpp},
    {Hag::S3::Trio64::VideoMode::G640x200x1bppM, 0x00, 640, 200, (uint8_t*)0xB8000, drawTestPattern1bpp},
    {Hag::S3::Trio64::VideoMode::G320x200x4bppC, 0x00, 320, 200, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::S3::Trio64::VideoMode::G640x200x4bppC, 0x00, 640, 200, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::S3::Trio64::VideoMode::G640x350x1bppM, 0x00, 640, 350, (uint8_t*)0xA0000, drawTestPattern1bpp},
    {Hag::S3::Trio64::VideoMode::G640x350x2bppC, 0x00, 640, 350, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::S3::Trio64::VideoMode::G640x480x1bppM, 0x00, 640, 480, (uint8_t*)0xA0000, drawTestPattern1bpp},
    {Hag::S3::Trio64::VideoMode::G640x480x4bppC, 0x00, 640, 480, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::S3::Trio64::VideoMode::G320x200x8bppC, 0x00, 320, 200, (uint8_t*)0xA0000, drawTestPattern8bpp},

    //VESA modes:
    {Hag::S3::Trio64::VideoMode::G640x400x8bpp, 0x100, 640, 400, nullptr, drawTestPattern8bpp}, //VESA Mode 0x100
    {Hag::S3::Trio64::VideoMode::G640x480x8bpp, 0x101, 640, 480, nullptr, drawTestPattern8bpp}, //VESA Mode 0x101
    {Hag::S3::Trio64::VideoMode::G800x600x4bpp, 0x102, 800, 600, nullptr, drawTestPattern4bpp}, //VESA Mode 0x102
    {Hag::S3::Trio64::VideoMode::G800x600x8bpp, 0x103, 800, 600, nullptr, drawTestPattern8bpp}, //VESA Mode 0x103
    {Hag::S3::Trio64::VideoMode::G1024x768x4bpp, 0x104, 1024, 768, nullptr, drawTestPattern4bpp}, //VESA Mode 0x104
    {Hag::S3::Trio64::VideoMode::G1024x768x8bpp, 0x105, 1024, 768, nullptr, drawTestPattern8bpp}, //VESA Mode 0x105
    //{Hag::S3::Trio64::VideoMode::G1280x1024x4bpp, 0x106, 1280, 1024, nullptr, drawTestPattern4bpp}, //VESA Mode 0x106
    //{Hag::S3::Trio64::VideoMode::G1280x1024x8bpp, 0x107, 1280, 1024, nullptr, drawTestPattern8bpp}, //VESA Mode 0x107
    {Hag::S3::Trio64::VideoMode::T132x43x4bpp, 0x10A, 132, 43, (uint8_t*)0xB8000, drawTestPatternText}, //VESA Mode 0x10A
    {Hag::S3::Trio64::VideoMode::T132x25x4bpp, 0x109, 132, 25, (uint8_t*)0xB8000, drawTestPatternText}, //VESA Mode 0x109
    {Hag::S3::Trio64::VideoMode::G640x480x15bpp, 0x110, 640, 480, nullptr, drawTestPattern15bpp}, //VESA Mode 0x110
    {Hag::S3::Trio64::VideoMode::G640x480x16bpp, 0x111, 640, 480, nullptr, drawTestPattern16bpp}, //VESA Mode 0x111
    {Hag::S3::Trio64::VideoMode::G640x480x32bpp, 0x112, 640, 480, nullptr, drawTestPattern32bpp}, //VESA Mode 0x112
    {Hag::S3::Trio64::VideoMode::G800x600x15bpp, 0x113, 800, 600, nullptr, drawTestPattern15bpp}, //VESA Mode 0x113
    {Hag::S3::Trio64::VideoMode::G800x600x16bpp, 0x114, 800, 600, nullptr, drawTestPattern16bpp}, //VESA Mode 0x114
    {Hag::S3::Trio64::VideoMode::G800x600x32bpp, 0x115, 800, 600, nullptr, drawTestPattern32bpp}, //VESA Mode 0x115
    {Hag::S3::Trio64::VideoMode::G1024x768x15bpp, 0x116, 1024, 768, nullptr, drawTestPattern15bpp}, //VESA Mode 0x116
    {Hag::S3::Trio64::VideoMode::G1024x768x16bpp, 0x117, 1024, 768, nullptr, drawTestPattern16bpp}, //VESA Mode 0x117
    // Not enough memory {Hag::S3::Trio64::VideoMode::G1024x768x32bpp, 0x118, 1024, 768, nullptr, drawTestPattern32bpp}, //VESA Mode 0x118
    // Not enough memory {Hag::S3::Trio64::VideoMode::G1280x1024x15bpp, 0x119, 1280, 1024, nullptr, drawTestPattern15bpp}, //VESA Mode 0x119
    // Not enough memory {Hag::S3::Trio64::VideoMode::G1280x1024x16bpp, 0x11A, 1280, 1024, nullptr, drawTestPattern16bpp}, //VESA Mode 0x11A

    //Proprietary modes:
    //Out of range {VideoMode::P1600x1200x8bpp, 0x120, 1600, 1200, nullptr, drawTestPattern8bpp}, //VESA Mode 0x120
    {Hag::S3::Trio64::VideoMode::P640x480x8bpp, 0x201, 640, 480, nullptr, drawTestPattern8bpp}, //Proprietary VESA Mode 0x201
    {Hag::S3::Trio64::VideoMode::P800x600x4bpp, 0x202, 800, 600, nullptr, drawTestPattern4bpp}, //Proprietary VESA Mode 0x202
    {Hag::S3::Trio64::VideoMode::P800x600x8bpp, 0x203, 800, 600, nullptr, drawTestPattern8bpp}, //Proprietary VESA Mode 0x203
    {Hag::S3::Trio64::VideoMode::P1024x768x4bpp, 0x204, 1024, 768, nullptr, drawTestPattern4bpp}, //Proprietary VESA Mode 0x204
    {Hag::S3::Trio64::VideoMode::P1024x768x8bpp, 0x205, 1024, 768, nullptr, drawTestPattern8bpp}, //Proprietary VESA Mode 0x205
    //{Hag::S3::Trio64::VideoMode::P1152x864x8bpp, 0x207, 1152, 864, nullptr, drawTestPattern8bpp}, //Proprietary VESA Mode 0x207
    //{Hag::S3::Trio64::VideoMode::P1280x1024x4bpp, 0x208, 1280, 1024, nullptr, drawTestPattern4bpp}, //Proprietary VESA Mode 0x208
    {Hag::S3::Trio64::VideoMode::P640x400x32bpp, 0x213, 640, 400, nullptr, drawTestPattern32bpp}, //Proprietary VESA Mode 0x213
};

uint8_t readKey();
#pragma aux readKey = \
    "in al, 60h"    \
    value [al];

void dumpplanes(const char* start, uint8_t mode)
{
    using namespace Hag::S3::Trio64;

    char filename[50];
    uint8_t orig = GraphicsController::ReadPlaneSelect::Read();
    for (int i = 0; i < 4; ++i)
    {
        GraphicsController::ReadPlaneSelect::Write(i);
        sprintf(filename, "%sA_%02X_%02X.bin", start, mode, i);
        memdump(filename, 0xA0000);
        sprintf(filename, "%sB_%02X_%02X.bin", start, mode, i);
        memdump(filename, 0xB0000);
    }
    GraphicsController::ReadPlaneSelect::Write(orig);
}

Hag::Math::fp pX = Hag::Math::fp::Divide(5257311, 200000);
Hag::Math::fp pZ = Hag::Math::fp::Divide(8506508, 200000);
Hag::Math::fp nX = pX.Neg();
Hag::Math::fp nZ = pZ.Neg();

Hag::Math::v4 icoVecs[12] =
{
    Hag::Math::v4(nX,  0, pZ, 1), Hag::Math::v4(pX,  0, pZ, 1), Hag::Math::v4(nX,  0, nZ, 1), Hag::Math::v4(pX,  0, nZ, 1),
    Hag::Math::v4( 0, pZ, pX, 1), Hag::Math::v4( 0, pZ, nX, 1), Hag::Math::v4( 0, nZ, pX, 1), Hag::Math::v4( 0, nZ, nX, 1),
    Hag::Math::v4(pZ, pX,  0, 1), Hag::Math::v4(nZ, pX,  0, 1), Hag::Math::v4(pZ, nX,  0, 1), Hag::Math::v4(nZ, nX,  0, 1),
};

int32_t icoTri[20][3] =
{
    {0,  1,  4}, {0,  4, 9}, {9, 4,  5}, { 4, 8, 5}, {4,  1, 8},
    {8,  1, 10}, {8, 10, 3}, {5, 8,  3}, { 5, 3, 2}, {2,  3, 7},
    {7,  3, 10}, {7, 10, 6}, {7, 6, 11}, {11, 6, 0}, {0,  6, 1},
    {6, 10,  1}, {9, 11, 0}, {9, 2, 11}, { 9, 5, 2}, {7, 11, 2},
};

bool isBackFace(Hag::Math::v4& vec0, Hag::Math::v4& vec1, Hag::Math::v4& vec2)
{
    Hag::Math::fp d01x = vec1.x() - vec0.x();
    Hag::Math::fp d01y = vec1.y() - vec0.y();
    Hag::Math::fp d02x = vec2.x() - vec0.x();
    Hag::Math::fp d02y = vec2.y() - vec0.y();
    return d01x * d02y - d01y * d02x >= 0;
}

int main(void)
{
    using namespace Hag;
    using namespace Hag::Math;
    using namespace Hag::S3;
    /*
    uint16_t screenWidth = 200;
    uint16_t screenHeight = 200;

    v3 val(0);
    v3 inc(fp::Pi / 100, fp::Pi / 75, fp::Pi / 55);

    v4 icor[12];

    S3::Trio64::SetLegacyVideoMode(S3::Trio64::VideoMode::G640x480x16bpp);
    uint8_t* linearFrameBuffer = S3::Trio64::GetLinearFrameBufferAs<uint8_t>();
    
    drawTestPattern16bpp(640, 480, linearFrameBuffer);
    drawTestPattern16bpp2(640, 480, linearFrameBuffer + (640 * 480 * 2));

    m44 scale = m44::Scale(v3(3, 3, 3));
    m44 proj = m44::Projection(screenWidth, screenHeight, fp(90).ToRad(), fp::One, fp(250));
    fp scrX = 320;
    fp scrY = 240;

    v4 light = v4(v3(-1, -1, -1).Normalize(), 0);

    uint16_t startY = 0;
    do
    {
        S3::Trio64::SetScissors(0, 0, 639, 479);

        S3::Trio64::DrawRectangle(200, 120, 440, 360, 0x0000FF);

        S3::Trio64::SetScissors(210, 130, 430, 350);

        m44 rot = m44::RotateZ(val.z()) *
                    m44::RotateY(val.y()) * m44::RotateX(val.x());

        m44 icoTrans = proj * m44::Translate(v3(0, 0, 90)) * rot * scale;

        for (int32_t ij = 0; ij < 12; ++ij)
        {
            v4 t = icoTrans * icoVecs[ij];
            fp invZ = fp::One / t.z();
            icor[ij] = v4(t.x() * invZ + scrX, t.y() * invZ + scrY, t.z(), t.w());
        }

        v4 tri[3];

        for (int32_t i1 = 0; i1 < 20; ++i1)
        {
            tri[0] = icor[icoTri[i1][0]];
            tri[1] = icor[icoTri[i1][1]];
            tri[2] = icor[icoTri[i1][2]];
            if (isBackFace(tri[0], tri[1], tri[2]))
                continue;

            uint32_t bits5 = 0x1F;
            uint32_t bits6 = 0x3F;
            
            uint32_t col = 0;
            switch (i1 & 3)
            {
            case 1:
                col = bits6 << 5;
                break;
            case 2:
                col = (bits5 << 11) | bits5;
                break;
            case 3:
                col = (bits5 << 11) | (bits6 << 6);
                break;
            default:
                col = bits5;
                break;
            }

            S3::Trio64::DrawTriangle(tri[0].x().RawFloor(), tri[0].y().RawFloor(),
                                     tri[1].x().RawFloor(), tri[1].y().RawFloor(),
                                     tri[2].x().RawFloor(), tri[2].y().RawFloor(),
                                     col);
        }

        val = (val + inc) % fp::TwoPi;

        //startY = startY == 0 ? 480 : 0;
        S3::Trio64::WaitForVSync();
        //S3::Trio64::SetDisplayStart(0, startY);
    } while (readKey() != 1);

    S3::Trio64::SetLegacyVideoMode(S3::Trio64::VideoMode::T80x25x4bpp);
    */

    REGPACK r;
    memset(&r, 0, sizeof(r));
    char filename[50];
    for (uint16_t i = 0; i < sizeof(modeTests) / sizeof(ModeTest); ++i)
    {
        r.w.ax = modeTests[i].mode != 3 ? 0x0003 : 0x0002;
        intr(0x10, &r);

        if (modeTests[i].vesaMode == 0x00)
        {
            r.h.ah = 0x00;
            r.h.al = modeTests[i].mode;
            intr(0x10, &r);
        }
        else
        {
            r.w.ax = 0x4f02;
            r.w.bx = modeTests[i].vesaMode;
            intr(0x10, &r);
        }
        sprintf(filename, "BIOS%02X.txt", modeTests[i].mode);
        //dumpplanes("B", modeTests[i].mode);
        regdump(filename);

        r.w.ax = modeTests[i].mode != 3 ? 0x0003 : 0x0002;
        intr(0x10, &r);

        //SetVideoMode(modeTests[i].mode);
        //sprintf(filename, "port%02X.txt", modeTests[i].mode);
        //dumpplanes("P", modeTests[i].mode);
        //regdump(filename);

        //r.w.ax = modeTests[i].mode != 3 ? 0x0003 : 0x0002;
        //intr(0x10, &r);

        Trio64::SetLegacyVideoMode(modeTests[i].mode);
        sprintf(filename, "clean%02X.txt", modeTests[i].mode);
        //dumpplanes("C", modeTests[i].mode);
        regdump(filename);

/*
        Trio64::SetLegacyVideoMode(modeTests[i].mode);

        uint8_t* linearFrameBuffer = Trio64::GetLinearFrameBufferAs<uint8_t>();

        uint8_t* address = modeTests[i].address;
        if (modeTests[i].address == nullptr)
            address = linearFrameBuffer;

        if (address == nullptr)
            continue;

        modeTests[i].DrawTestPattern(modeTests[i].width, modeTests[i].height, address);
        getchar();
*/
    }

    r.w.ax = 0x0003;
    intr(0x10, &r);

    // r.w.ax = 0x4f02;
    // r.w.bx = Hag::Vesa::VideoMode::G800x600x32bpp;
    // intr(0x10, &r);
}
