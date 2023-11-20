//Copyright 2023-Present riplin

#include <stdio.h>
#include <hag/types.h>
#include <hag/farptr.h>
#include <i86.h>
#include <string.h>
#include <hag/system/pci.h>
#include <hag/system/sysasm.h>
#include <hag/support/allocatr.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/s3/regs.h>
#include <hag/drivers/s3/vidmodes.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/sysconf.h>
#include <hag/drivers/s3/crtc/bkwcomp2.h>
#include <hag/drivers/s3/crtc/crtreglk.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>

Hag::VGA::Register_t Registers[] =
{
    Hag::VGA::Register::InputStatus0,
    Hag::VGA::Register::FeatureControlR,
    Hag::VGA::Register::MiscellaneousR,
    Hag::VGA::Register::InputStatus1B,
    Hag::VGA::Register::InputStatus1D,
    Hag::VGA::Register::VideoSubsystemEnable,
    Hag::VGA::Register::DACMask,
    Hag::VGA::Register::DACStatus,
    Hag::S3::Register::SetupOptionSelect,
    Hag::S3::Register::VideoSubsystemEnableS3,
    Hag::S3::Register::SubsystemStatus,
    Hag::S3::Register::AdvancedFunctionControl,
    Hag::S3::Register::CurrentYPosition,
    Hag::S3::Register::CurrentYPosition2,
    Hag::S3::Register::CurrentXPosition,
    Hag::S3::Register::CurrentXPosition2,
    Hag::S3::Register::DestinationYPositionAxialStepConstant,
    Hag::S3::Register::YCoordinate2AxialStepConstant2,
    Hag::S3::Register::DestinationXPositionDiagonalStepConstant,
    Hag::S3::Register::XCoordinate2,
    Hag::S3::Register::LineErrorTerm,
    Hag::S3::Register::LineErrorTerm2,
    Hag::S3::Register::MajorAxisPixelCount,
    Hag::S3::Register::MajorAxisPixelCount2,
    Hag::S3::Register::GraphicsProcessorStatus,
    Hag::S3::Register::DrawingCommand,
    Hag::S3::Register::DrawingCommand2,
    Hag::S3::Register::ShortStrokeVectorTransfer,
    Hag::S3::Register::BackgroundColor,
    Hag::S3::Register::ForegroundColor,
    Hag::S3::Register::BitplaneWriteMask,
    Hag::S3::Register::BitplaneReadMask,
    Hag::S3::Register::ColorCompareRegister,
    Hag::S3::Register::BackgroundMix,
    Hag::S3::Register::ForegroundMix,
    Hag::S3::Register::ReadRegisterData,
    Hag::S3::Register::WriteRegisterData,
    Hag::S3::Register::PixelDataTransfer,
    Hag::S3::Register::PixelDataTransferExtension,
    Hag::S3::Register::PatternY,
    Hag::S3::Register::PatternX
};

namespace Clean
{
    bool SetVideoMode(uint8_t mode);
}

bool SetVideoMode(uint8_t mode);

void regdump(const char* filename)
{
    //Write code to CR38 to provide access to the S3 VGA registers (CR30-CR3F)
    Hag::S3::CRTController::RegisterLock1::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> rl1;
    
    //Write code to CR39 to provide access to the System Control and System Extension registers (CR40-CRFF)
    Hag::S3::CRTController::RegisterLock2::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> rl2;

    //Set bit 0 in CR40 to enable access to the Enhanced Commands registers.
    Hag::S3::CRTController::SystemConfiguration::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> sc;

    //Enable write access to bits 1 and 6 of CR7
    //Enable access to RAMDAC register
    //Enable access to Palette/Overscan registers
    Hag::S3::CRTController::BackwardCompatibility2::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> bc2;

    //Enable write access to CR0-CR6, CR7 (bits 7,5,3,2,0), CR9 (bit5), CR10, CR11 (bits 3-0), CR15-CR16, CR17 (bit 2)
    Hag::S3::CRTController::CRTRegisterLock::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> crl;

    //Enable write access to CR0-CR7
    Hag::VGA::CRTController::VerticalRetraceEnd::SoftUnlock<Hag::VGA::Register::CRTControllerIndexD> vre;

    //write code to SR8 to provide access to SR9-SR18.
    Hag::S3::Sequencer::UnlockExtendedSequencer::SoftUnlock ues;

    FILE* fp = fopen(filename, "w");

    fprintf(fp, "VGA and S3 registers:\n");
    for (uint32_t i = 0; i < sizeof(Registers) / sizeof(Hag::VGA::Register_t); ++i)
    {
        Hag::VGA::Register_t reg = Registers[i];
        uint8_t value = SYS_ReadPortByte(reg);
        fprintf(fp, "{ 0x%04X, 0x%02X },\n", reg, value);
    }

    fprintf(fp, "\nAttribute Controller registers:\n");
    SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
    uint8_t orgAttribIdx = SYS_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
    for (uint8_t i = 0; i < 32; ++i)
    {
        uint8_t idx = (orgAttribIdx & 0xE0) | i;
        SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
        SYS_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, idx);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::AttributeControllerDataR);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }
    SYS_ReadPortByte(Hag::VGA::Register::InputStatus1D);//Reset attribute controller to index register.
    SYS_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, orgAttribIdx);

    fprintf(fp, "\nRAMDAC registers:\n");
    SYS_WritePortByte(Hag::VGA::Register::DACReadIndex, 0x00);
    for (uint16_t i = 0; i < 256; ++i)
    {
        uint8_t red = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        uint8_t green = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        uint8_t blue = SYS_ReadPortByte(Hag::VGA::Register::RAMDACData);
        fprintf(fp, "{ 0x%02X, 0x%02X, 0x%02X }, //%i\n", red, green, blue, i);
    }

    fprintf(fp, "\nSequencer Controller registers:\n");
    for (uint8_t i = 0; i < 32; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::SequencerIndex, i);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::SequencerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nGraphics Controller registers:\n");
    for (uint8_t i = 0; i < 16; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::GraphicsControllerIndex, i);
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::GraphicsControllerData);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", i, value);
    }

    fprintf(fp, "\nCRT Controller registers:\n");
    for (uint16_t i = 0; i < 256; ++i)
    {
        SYS_WritePortByte(Hag::VGA::Register::CRTControllerIndexD, uint8_t(i));
        uint8_t value = SYS_ReadPortByte(Hag::VGA::Register::CRTControllerDataD);
        fprintf(fp, "{ 0x%02X, 0x%02X },\n", uint8_t(i), value);
    }

    fclose(fp);
}

void memdump(const char* filename)
{
    uint8_t* ptr = NULL;
    FILE* fp = fopen(filename, "wb");

    fwrite(ptr, 1, 0x10000, fp);

    fclose(fp);
}

void drawTestPattern1bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
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
            
            Hag::VGA::Sequencer::EnableWritePlane::Write(0x01);
            videoMemory[y * pixelWidth + x] = plane0;

            Hag::VGA::Sequencer::EnableWritePlane::Write(0x02);
            videoMemory[y * pixelWidth + x] = plane1;

            Hag::VGA::Sequencer::EnableWritePlane::Write(0x04);
            videoMemory[y * pixelWidth + x] = plane2;

            Hag::VGA::Sequencer::EnableWritePlane::Write(0x08);
            videoMemory[y * pixelWidth + x] = plane3;
        }
    }
}

void drawTestPattern8bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
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
}

struct CharAndAttr
{
    uint8_t Char;
    uint8_t Attr;
};

void drawTestPatternText(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
    CharAndAttr* screen = (CharAndAttr*)videoMemory;

    //│ 0xB3
    //─ 0xC4
    //└ 0xC0
    //┘ 0xD9
    //┌ 0xDA
    //┐ 0xBF
    //┴ 0xC1
    //┬ 0xC2
    //├ 0xC3
    //┼ 0xC5
    //┤ 0xB4
    //╙ 0xD3
    //╜ 0xBD
    //╓ 0xD6
    //╖ 0xB7
    //╨ 0xD0
    //╥ 0xD2
    //╟ 0xC7
    //╫ 0xD7
    //╢ 0xB6
    //╘ 0xD4
    //╛ 0xBE
    //╒ 0xD5
    //╕ 0xB8
    //╧ 0xCF
    //╤ 0xD1
    //╞ 0xC6
    //╪ 0xD8
    //╡ 0xB5
    //║ 0xBA
    //═ 0xCD
    //╚ 0xC8
    //╝ 0xBC
    //╔ 0xC9
    //╗ 0xBB
    //╩ 0xCA
    //╦ 0xCB
    //╠ 0xCC
    //╬ 0xCE
    //╣ 0xB9
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

void drawTestPattern32bpp(uint16_t width, uint16_t height, uint8_t* videoMemory)
{
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
    {Hag::VGA::VideoMode::T40x25x16C, 0x00, 40, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::VGA::VideoMode::T80x25x16G, 0x00, 80, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::VGA::VideoMode::T80x25x16C, 0x00, 80, 25, (uint8_t*)0xB8000, drawTestPatternText},
    {Hag::VGA::VideoMode::T80x25x2M, 0x00, 80, 25, (uint8_t*)0xB0000, drawTestPatternText},
    {Hag::VGA::VideoMode::G320x200x4C, 0x00, 320, 200, (uint8_t*)0xB8000, drawTestPattern2bpp},
    {Hag::VGA::VideoMode::G320x200x4G, 0x00, 320, 200, (uint8_t*)0xB8000, drawTestPattern2bpp},
    {Hag::VGA::VideoMode::G640x200x2M, 0x00, 640, 200, (uint8_t*)0xB8000, drawTestPattern1bpp},
    {Hag::VGA::VideoMode::G320x200x16C, 0x00, 320, 200, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::VGA::VideoMode::G640x200x16C, 0x00, 640, 200, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::VGA::VideoMode::G640x350x2M, 0x00, 640, 350, (uint8_t*)0xA0000, drawTestPattern1bpp},
    {Hag::VGA::VideoMode::G640x350x4C, 0x00, 640, 350, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::VGA::VideoMode::G640x480x2M, 0x00, 640, 480, (uint8_t*)0xA0000, drawTestPattern1bpp},
    {Hag::VGA::VideoMode::G640x480x16C, 0x00, 640, 480, (uint8_t*)0xA0000, drawTestPattern4bpp},
    {Hag::VGA::VideoMode::G320x200x256C, 0x00, 320, 200, (uint8_t*)0xA0000, drawTestPattern8bpp},

    //VESA modes:
    {Hag::S3::VideoMode::G640x400x256C, 0x100, 640, 400, NULL, drawTestPattern8bpp}, //VESA Mode 0x100
    {Hag::S3::VideoMode::G640x480x256C, 0x101, 640, 480, NULL, drawTestPattern8bpp}, //VESA Mode 0x101
    {Hag::S3::VideoMode::G800x600x16C, 0x102, 800, 600, NULL, drawTestPattern4bpp}, //VESA Mode 0x102
    {Hag::S3::VideoMode::G800x600x256C, 0x103, 800, 600, NULL, drawTestPattern8bpp}, //VESA Mode 0x103
    {Hag::S3::VideoMode::G1024x768x16C, 0x104, 1024, 768, NULL, drawTestPattern4bpp}, //VESA Mode 0x104
    {Hag::S3::VideoMode::G1024x768x256C, 0x105, 1024, 768, NULL, drawTestPattern8bpp}, //VESA Mode 0x105
    //{Hag::S3::VideoMode::G1280x1024x16C, 0x106, 1280, 1024, NULL, drawTestPattern4bpp}, //VESA Mode 0x106
    //{Hag::S3::VideoMode::G1280x1024x256C, 0x107, 1280, 1024, NULL, drawTestPattern8bpp}, //VESA Mode 0x107
    {Hag::S3::VideoMode::T132x43x16C, 0x10A, 132, 43, (uint8_t*)0xB8000, drawTestPatternText}, //VESA Mode 0x10A
    {Hag::S3::VideoMode::T132x25x16C, 0x109, 132, 25, (uint8_t*)0xB8000, drawTestPatternText}, //VESA Mode 0x109
    {Hag::S3::VideoMode::G640x480x32K, 0x110, 640, 480, NULL, drawTestPattern15bpp}, //VESA Mode 0x110
    {Hag::S3::VideoMode::G640x480x64K, 0x111, 640, 480, NULL, drawTestPattern16bpp}, //VESA Mode 0x111
    {Hag::S3::VideoMode::G640x480x16M, 0x112, 640, 480, NULL, drawTestPattern32bpp}, //VESA Mode 0x112
    {Hag::S3::VideoMode::G800x600x32K, 0x113, 800, 600, NULL, drawTestPattern15bpp}, //VESA Mode 0x113
    {Hag::S3::VideoMode::G800x600x64K, 0x114, 800, 600, NULL, drawTestPattern16bpp}, //VESA Mode 0x114
    {Hag::S3::VideoMode::G800x600x16M, 0x115, 800, 600, NULL, drawTestPattern32bpp}, //VESA Mode 0x115
    {Hag::S3::VideoMode::G1024x768x32K, 0x116, 1024, 768, NULL, drawTestPattern15bpp}, //VESA Mode 0x116
    {Hag::S3::VideoMode::G1024x768x64K, 0x117, 1024, 768, NULL, drawTestPattern16bpp}, //VESA Mode 0x117
    // Not enough memory {Hag::S3::VideoMode::G1024x768x16M, 0x118, 1024, 768, NULL, drawTestPattern32bpp}, //VESA Mode 0x118
    // Not enough memory {Hag::S3::VideoMode::G1280x1024x32K, 0x119, 1280, 1024, NULL, drawTestPattern15bpp}, //VESA Mode 0x119
    // Not enough memory {Hag::S3::VideoMode::G1280x1024x64K, 0x11A, 1280, 1024, NULL, drawTestPattern16bpp}, //VESA Mode 0x11A

    //Proprietary modes:
    //Out of range {Hag::S3::VideoMode::P1600x1200x256C, 0x120, 1600, 1200, NULL, drawTestPattern8bpp}, //VESA Mode 0x120
    {Hag::S3::VideoMode::P640x480x256C, 0x201, 640, 480, NULL, drawTestPattern8bpp}, //Proprietary VESA Mode 0x201
    {Hag::S3::VideoMode::P800x600x16C, 0x202, 800, 600, NULL, drawTestPattern4bpp}, //Proprietary VESA Mode 0x202
    {Hag::S3::VideoMode::P800x600x256C, 0x203, 800, 600, NULL, drawTestPattern8bpp}, //Proprietary VESA Mode 0x203
    {Hag::S3::VideoMode::P1024x768x16C, 0x204, 1024, 768, NULL, drawTestPattern4bpp}, //Proprietary VESA Mode 0x204
    {Hag::S3::VideoMode::P1024x768x256C, 0x205, 1024, 768, NULL, drawTestPattern8bpp}, //Proprietary VESA Mode 0x205
    //{Hag::S3::VideoMode::P1152x864x256C, 0x207, 1152, 864, NULL, drawTestPattern8bpp}, //Proprietary VESA Mode 0x207
    //{Hag::S3::VideoMode::P1280x1024x16C, 0x208, 1280, 1024, NULL, drawTestPattern4bpp}, //Proprietary VESA Mode 0x208
    {Hag::S3::VideoMode::P640x400x16M, 0x213, 640, 400, NULL, drawTestPattern32bpp}, //Proprietary VESA Mode 0x213
};

uint8_t* GetLinearFrameBuffer(uint16_t vendorId, uint16_t deviceId)
{
    uint8_t* linearFrameBuffer = 0x00000000;
    uint8_t bus = 0xFF;
    uint8_t slot = 0xFF;
    uint8_t function = 0xFF;
    if (Hag::System::PCI::FindDevice(vendorId, deviceId, bus, slot, function))
        linearFrameBuffer = (uint8_t*)Hag::System::PCI::Read32(bus, slot, function, Hag::System::PCI::Header0::BaseAddress0);

    return linearFrameBuffer;
}

int main(void)
{
    uint8_t* linearFrameBuffer = GetLinearFrameBuffer(0x5333, 0x8811);
    if (linearFrameBuffer == NULL)
        linearFrameBuffer = GetLinearFrameBuffer(0x5333, 0x8810);

    REGPACK r;
    memset(&r, 0, sizeof(r));
    char filename[50];
    for (uint16_t i = 0; i < sizeof(modeTests) / sizeof(ModeTest); ++i)
    {
        r.w.ax = 0x0003;
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
        sprintf(filename, "bios%02X.txt", modeTests[i].mode);
        regdump(filename);

        r.w.ax = 0x0003;
        intr(0x10, &r);

        SetVideoMode(modeTests[i].mode);
        sprintf(filename, "port%02X.txt", modeTests[i].mode);
        regdump(filename);

        r.w.ax = 0x0003;
        intr(0x10, &r);

        Clean::SetVideoMode(modeTests[i].mode);
        sprintf(filename, "clean%02X.txt", modeTests[i].mode);
        regdump(filename);

        /*
        uint8_t* address = modeTests[i].address;
        if (modeTests[i].address == NULL)
            address = linearFrameBuffer;

        if (address == NULL)
            continue;

        if (modeTests[i].vesaMode == 0x00)
            continue;

        r.w.ax = 0x4f02;
        r.w.bx = modeTests[i].vesaMode;
        intr(0x10, &r);

        //Clean::SetVideoMode(modeTests[i].mode);
        modeTests[i].DrawTestPattern(modeTests[i].width, modeTests[i].height, address);
        getchar();
        */
    }

    r.w.ax = 0x0003;
    intr(0x10, &r);
}
