//Copyright 2025-Present riplin

#include <hag/drivers/matrox/shared/data.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include "modintl.h"

namespace Hag { namespace Matrox { namespace Shared { namespace Data
{

using namespace Hag::Matrox::Shared::Function::ModeSetting;

//Horizontal timings:

HorizontalTimings H00char40 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0 };
HorizontalTimings H01char80 = { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81 };
HorizontalTimings H02pix640 = { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80 };
HorizontalTimings H03pix320 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80 };
HorizontalTimings H04pix640 = { 0x67, 0x4F, 0x4F, 0x8B, 0x53, 0x9F };
HorizontalTimings H05pix640 = { 0x5F, 0x4F, 0x4F, 0x83, 0x51, 0x9D };
HorizontalTimings H06pix800 = { 0x7F, 0x63, 0x63, 0x82, 0x6B, 0x1B };
HorizontalTimings H07pix800 = { 0x7F, 0x63, 0x63, 0x83, 0x68, 0x18 };
HorizontalTimings H08pix1024 = { 0xA3, 0x7F, 0x7F, 0x87, 0x82, 0x93 };
HorizontalTimings H09pix1280 = { 0xCE, 0x9F, 0x9F, 0x92, 0xA5, 0x13 };
HorizontalTimings H10pix1600 = { 0x09, 0xC7, 0xC7, 0x8D, 0xCF, 0x07 };
HorizontalTimings H11char80 = { 0x60, 0x4F, 0x50, 0x83, 0x52, 0x9E };
HorizontalTimings H12char132 = { 0x9E, 0x83, 0x84, 0x81, 0x87, 0x8D };
HorizontalTimings H13pix512 = { 0x5F, 0x3F, 0x40, 0x82, 0x4E, 0x9A };

//Vertical timings:

VerticalTimings V00char50 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x96, 0xB9 };
VerticalTimings V01char25 = { 0xBF, 0x1F, 0x00, 0x83, 0x05, 0x5D, 0x63, 0xBA };
VerticalTimings V02line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x04 };
VerticalTimings V03line400 = { 0xC0, 0x1F, 0x00, 0x97, 0x0D, 0x8F, 0x8F, 0xC1 };
VerticalTimings V04line480 = { 0x0B, 0x3E, 0x00, 0xE9, 0x0B, 0xDF, 0xDF, 0x0C };
VerticalTimings V05line600 = { 0x72, 0xF0, 0x20, 0x58, 0x0C, 0x57, 0x57, 0x73 };
VerticalTimings V06line768 = { 0x24, 0xF5, 0x20, 0x02, 0x08, 0xFF, 0xFF, 0x25 };
VerticalTimings V07line1024 = { 0x28, 0x5A, 0x20, 0x00, 0x03, 0xFF, 0xFF, 0x29 };
VerticalTimings V08line1200 = { 0xE0, 0x00, 0x00, 0xB0, 0x03, 0xAF, 0xAF, 0xE1 };
VerticalTimings V09char60 = { 0x0B, 0x3E, 0x00, 0xEB, 0x0C, 0xDF, 0xE6, 0x06 };
VerticalTimings V10char25 = { 0xC0, 0x1F, 0x00, 0x9E, 0x06, 0x8F, 0x96, 0xBB };
VerticalTimings V11char43 = { 0x88, 0x1F, 0x00, 0x66, 0x04, 0x57, 0x5E, 0x83 };
VerticalTimings V12char60 = { 0x11, 0x3E, 0x00, 0xEB, 0x0E, 0xDF, 0xE6, 0x0C };
VerticalTimings V13line480 = { 0x0D, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x06 };
VerticalTimings V14line512 = { 0x23, 0xB2, 0x20, 0x0A, 0x0C, 0xFF, 0x07, 0x17 };

//Resolution timings:

ResolutionTimings T01char40x50 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H00char40, V00char50 };
ResolutionTimings T02char40x25 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H00char40, V01char25 };
ResolutionTimings T03char40x25 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H00char40, V00char50 };
ResolutionTimings T04char80x50 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H01char80, V00char50 };
ResolutionTimings T05char80x25 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H01char80, V01char25 };
ResolutionTimings T06char80x25 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H01char80, V00char50 };
ResolutionTimings T07pix640x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H02pix640, V00char50 };
ResolutionTimings T08pix640x350 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H02pix640, V01char25 };
ResolutionTimings T09pix640x480 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H02pix640, V02line480 };
ResolutionTimings T10pix320x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H03pix320, V00char50 };
ResolutionTimings T11pix640x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H02pix640, V00char50 };
ResolutionTimings T12pix640x400 = { 0x00006A86, 0x40, 0x00, 0x40, 0x00, H04pix640, V03line400 };
ResolutionTimings T13pix640x480 = { 0x00006257, 0x40, 0x00, 0xC0, 0x00, H05pix640, V04line480 };
ResolutionTimings T14pix800x600 = { 0x00009C40, 0x00, 0x00, 0x00, 0x00, H06pix800, V05line600 };
ResolutionTimings T15pix800x600 = { 0x00009C40, 0x00, 0x00, 0x00, 0x00, H07pix800, V05line600 };
ResolutionTimings T16pix1024x768 = { 0x0000FDE8, 0x00, 0x00, 0xC0, 0x00, H08pix1024, V06line768 };
ResolutionTimings T17pix1280x1024 = { 0x0001A5E0, 0x40, 0x21, 0x00, 0x00, H09pix1280, V07line1024 };
ResolutionTimings T18pix1600x1200 = { 0x000278D0, 0x01, 0x2D, 0x00, 0x00, H10pix1600, V08line1200 };
ResolutionTimings T19char80x60 = { 0x00006270, 0x00, 0x00, 0xC0, 0x00, H11char80, V09char60 };
ResolutionTimings T20char132x25 = { 0x0000A078, 0x00, 0x00, 0x40, 0x00, H12char132, V10char25 };
ResolutionTimings T21char132x43 = { 0x0000A078, 0x00, 0x00, 0x40, 0x00, H12char132, V11char43 };
ResolutionTimings T22char132x60 = { 0x0000A078, 0x00, 0x00, 0xC0, 0x00, H12char132, V12char60 };
ResolutionTimings T23pix640x480 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H02pix640, V13line480 };
ResolutionTimings T24pix512x512 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H13pix512, V14line512 };

//Graphics:

VGA::GraphicsControllerData_t G00[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF };
VGA::GraphicsControllerData_t G01[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0A, 0x00, 0xFF };
VGA::GraphicsControllerData_t G02[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0xFF };
VGA::GraphicsControllerData_t G03[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0xFF };
VGA::GraphicsControllerData_t G04[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0xFF };
VGA::GraphicsControllerData_t G05[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0F, 0x00, 0xFF };
VGA::GraphicsControllerData_t G06[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF };
VGA::GraphicsControllerData_t G07[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF };

//Attributes:

VGA::AttributeControllerData_t A00[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
VGA::AttributeControllerData_t A01[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
VGA::AttributeControllerData_t A02[] = { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 };
VGA::AttributeControllerData_t A03[] = { 0x00, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17 };
VGA::AttributeControllerData_t A04[] = { 0x00, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00 };
VGA::AttributeControllerData_t A05[] = { 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F };
VGA::AttributeControllerData_t A06[] = { 0x00, 0x13, 0x15, 0x17, 0x02, 0x04, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
VGA::AttributeControllerData_t A07[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

//Configurations:

Configuration C00 = { 0x28, 0x18, 0x08, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C01 = { 0x28, 0x18, 0x0E, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C02 = { 0x28, 0x18, 0x10, 0x0800, 0x27, { 0x08, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
Configuration C03 = { 0x50, 0x18, 0x08, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C04 = { 0x50, 0x18, 0x0E, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C05 = { 0x50, 0x18, 0x10, 0x1000, 0x27, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
Configuration C06 = { 0x50, 0x18, 0x0E, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x03, }, 0x00, 0x40, 0x80, 0x0D, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
Configuration C07 = { 0x50, 0x18, 0x10, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x0F, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
Configuration C08 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x01, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC2, 0xFF, G02, 0x01, 0x00, 0x01, 0x00 };
Configuration C09 = { 0x50, 0x18, 0x0E, 0x8000, 0x22, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G03, 0x0B, 0x00, 0x05, 0x00 };
Configuration C10 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G04, 0x01, 0x00, 0x0F, 0x00 };
Configuration C11 = { 0x28, 0x18, 0x08, 0x4000, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x00, 0xA2, 0xFF, G05, 0x01, 0x00, 0x03, 0x00 };
Configuration C12 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x40, 0xA3, 0xFF, G06, 0x41, 0x00, 0x0F, 0x00 };
Configuration C13 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x09, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
Configuration C14 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
Configuration C15 = { 0x50, 0x18, 0x0E, 0x8000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
Configuration C16 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
Configuration C17 = { 0x50, 0x0B, 0x08, 0x2000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G06, 0x41, 0x00, 0x0F, 0x00 };
Configuration C18 = { 0x50, 0x0B, 0x08, 0x2000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x20, 0x00, 0xC3, 0xFF, G06, 0x41, 0x02, 0x0F, 0x00 };
Configuration C19 = { 0x64, 0x24, 0x10, 0xF000, 0x2F, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
Configuration C20 = { 0x50, 0x3B, 0x08, 0x2580, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C21 = { 0x84, 0x18, 0x10, 0x19D0, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C22 = { 0x84, 0x2A, 0x08, 0x2C58, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C23 = { 0x84, 0x31, 0x08, 0x3400, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C24 = { 0x84, 0x3B, 0x08, 0x3E00, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C25 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0xA0, 0x00, 0xE3, 0xFF, G06, 0x41, 0x00, 0x0F, 0x00 };
Configuration C26 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0xA0, 0x40, 0xA3, 0xFF, G06, 0x41, 0x00, 0x0F, 0x00 };

//Font Configurations:

FontConfiguration F00 = { Data::Font8x8, 0xFF, 0x08, 0x00, false, 0x07, 0x06, 0x07 };
FontConfiguration F01 = { Data::Font8x16, 0xFF, 0x00, 0x00, true, 0x0D, 0x0B, 0x0C };
FontConfiguration F02 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x0F, 0x0D, 0x0E };
FontConfiguration F03 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x07, 0x06, 0x07 };
FontConfiguration F04 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x0D, 0x0B, 0x0C };
FontConfiguration F05 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x00, 0x00 };
FontConfiguration F06 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x00, 0x00 };
FontConfiguration F07 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x20, 0x00 };

//Video Parameters:

VideoParameters P00 = { T01char40x50, C00, A00, F00 };
VideoParameters P01 = { T02char40x25, C01, A01, F01 };
VideoParameters P02 = { T03char40x25, C02, A01, F02 };
VideoParameters P03 = { T01char40x50, C00, A00, F03 };
VideoParameters P04 = { T04char80x50, C03, A00, F00 };
VideoParameters P05 = { T05char80x25, C04, A01, F01 };
VideoParameters P06 = { T06char80x25, C05, A01, F02 };
VideoParameters P07 = { T05char80x25, C06, A02, F04 };
VideoParameters P08 = { T06char80x25, C07, A02, F02 };
VideoParameters P09 = { T07pix640x400, C08, A03, F05 };
VideoParameters P10 = { T08pix640x350, C09, A04, F06 };
VideoParameters P11 = { T09pix640x480, C10, A05, F06 };
VideoParameters P12 = { T10pix320x400, C11, A06, F05 };
VideoParameters P13 = { T11pix640x400, C12, A07, F05 };
VideoParameters P14 = { T10pix320x400, C13, A00, F06 };
VideoParameters P15 = { T07pix640x400, C14, A00, F06 };
VideoParameters P16 = { T08pix640x350, C15, A01, F06 };
VideoParameters P17 = { T09pix640x480, C16, A01, F06 };
VideoParameters P18 = { T12pix640x400, C17, A07, F07 };
VideoParameters P19 = { T13pix640x480, C18, A07, F07 };
VideoParameters P20 = { T14pix800x600, C19, A01, F06 };
VideoParameters P21 = { T15pix800x600, C18, A07, F07 };
VideoParameters P22 = { T16pix1024x768, C18, A07, F07 };
VideoParameters P23 = { T17pix1280x1024, C18, A07, F07 };
VideoParameters P24 = { T18pix1600x1200, C18, A07, F07 };
VideoParameters P25 = { T19char80x60, C20, A01, F00 };
VideoParameters P26 = { T20char132x25, C21, A01, F02 };
VideoParameters P27 = { T21char132x43, C22, A01, F00 };
VideoParameters P28 = { T20char132x25, C23, A01, F00 };
VideoParameters P29 = { T22char132x60, C24, A01, F00 };
VideoParameters P30 = { T23pix640x480, C25, A07, F05 };
VideoParameters P31 = { T24pix512x512, C26, A07, F05 };

//Video Parameter Arrays:

VideoParameters* Mode1Parameters[] = { &P00, &P01, &P02, &P03 };
VideoParameters* Mode3Parameters[] = { &P04, &P05, &P06, &P03 };
VideoParameters* Mode7Parameters[] = { &P00, &P07, &P08, &P03 };
VideoParameters* Mode6Parameters[] = { &P09 };
VideoParameters* ModeFParameters[] = { &P10 };
VideoParameters* Mode11Parameters[] = { &P11 };
VideoParameters* Mode4Parameters[] = { &P12 };
VideoParameters* Mode13Parameters[] = { &P13 };
VideoParameters* ModeDParameters[] = { &P14 };
VideoParameters* ModeEParameters[] = { &P15 };
VideoParameters* Mode10Parameters[] = { &P16 };
VideoParameters* Mode12Parameters[] = { &P17 };
VideoParameters* Mode100Parameters[] = { &P18 };
VideoParameters* Mode101Parameters[] = { &P19 };
VideoParameters* Mode110Parameters[] = { &P19 };
VideoParameters* Mode111Parameters[] = { &P19 };
VideoParameters* Mode112Parameters[] = { &P19 };
VideoParameters* Mode102Parameters[] = { &P20 };
VideoParameters* Mode103Parameters[] = { &P21 };
VideoParameters* Mode113Parameters[] = { &P21 };
VideoParameters* Mode114Parameters[] = { &P21 };
VideoParameters* Mode115Parameters[] = { &P21 };
VideoParameters* Mode105Parameters[] = { &P22 };
VideoParameters* Mode116Parameters[] = { &P22 };
VideoParameters* Mode117Parameters[] = { &P22 };
VideoParameters* Mode118Parameters[] = { &P22 };
VideoParameters* Mode107Parameters[] = { &P23 };
VideoParameters* Mode119Parameters[] = { &P23 };
VideoParameters* Mode11AParameters[] = { &P23 };
VideoParameters* Mode11BParameters[] = { &P23 };
VideoParameters* Mode11CParameters[] = { &P24 };
VideoParameters* Mode11DParameters[] = { &P24 };
VideoParameters* Mode11EParameters[] = { &P24 };
VideoParameters* Mode11FParameters[] = { &P24 };
VideoParameters* Mode108Parameters[] = { &P25 };
VideoParameters* Mode109Parameters[] = { &P26 };
VideoParameters* Mode10AParameters[] = { &P27 };
VideoParameters* Mode10BParameters[] = { &P28 };
VideoParameters* Mode10CParameters[] = { &P29 };
VideoParameters* ModeXParameters[] = { &P30 };
VideoParameters* ModeQParameters[] = { &P31 };

ModeDescriptor s_Descriptors[] =
{
    { // Mode 1
        40,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::T40x25x4bppC,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode1Parameters
    },
    { // Mode 3
        80,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::T80x25x4bppC,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode3Parameters
    },
    { // Mode 7
        80,
        25,
        BitsPerPixel::Bpp1,
        Flags::Text | Flags::Monochrome | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::T80x25x1bppM,
        0xB000,
        Hag::VGA::Register::CRTControllerIndexB,
        Hag::System::BDA::DetectedHardware::Monochrome80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair4,
        Mode7Parameters
    },
    { // Mode 6
        640,
        200,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G640x200x1bppM,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics | Hag::System::BDA::CRTModeControlRegValue::Monochrome | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation,
        0x3F,
        Data::PalettePair2,
        Mode6Parameters
    },
    { // Mode F
        640,
        350,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G640x350x1bppM,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexB,
        Hag::System::BDA::DetectedHardware::Monochrome80x25,
        Scanlines::S350,
        0xFF,
        0xFF,
        Data::PalettePair4,
        ModeFParameters
    },
    { // Mode 11
        640,
        480,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        Hag::Vesa::VideoMode::G640x480x1bppM,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S480,
        0xFF,
        0xFF,
        Data::PalettePair0,
        Mode11Parameters
    },
    { // Mode 4
        320,
        200,
        BitsPerPixel::Bpp2,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G320x200x2bppC,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair2,
        Mode4Parameters
    },
    { // Mode 13
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G320x200x8bppC,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode13Parameters
    },
    { // Mode D
        320,
        200,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G320x200x4bppC,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair2,
        ModeDParameters
    },
    { // Mode E
        640,
        200,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G640x200x4bppC,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair2,
        ModeEParameters
    },
    { // Mode 10
        640,
        350,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::G640x350x4bppC,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S350,
        0xFF,
        0xFF,
        Data::PalettePair0,
        Mode10Parameters
    },
    { // Mode 12
        640,
        480,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        Hag::Vesa::VideoMode::G640x480x4bppC,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S480,
        0xFF,
        0xFF,
        Data::PalettePair0,
        Mode12Parameters
    },
    { // Mode 100
        640,
        400,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x20,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode100Parameters
    },
    { // Mode 101
        640,
        480,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x21,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode101Parameters
    },
    { // Mode 110
        640,
        480,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x30,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode110Parameters
    },
    { // Mode 111
        640,
        480,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x31,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode111Parameters
    },
    { // Mode 112
        640,
        480,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x32,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode112Parameters
    },
    { // Mode 102
        800,
        600,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x22,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair0,
        Mode102Parameters
    },
    { // Mode 103
        800,
        600,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x23,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode103Parameters
    },
    { // Mode 113
        800,
        600,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x33,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode113Parameters
    },
    { // Mode 114
        800,
        600,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x34,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode114Parameters
    },
    { // Mode 115
        800,
        600,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x35,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode115Parameters
    },
    { // Mode 105
        1024,
        768,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x25,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode105Parameters
    },
    { // Mode 116
        1024,
        768,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x36,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode116Parameters
    },
    { // Mode 117
        1024,
        768,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x37,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode117Parameters
    },
    { // Mode 118
        1024,
        768,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x38,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode118Parameters
    },
    { // Mode 107
        1280,
        1024,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x27,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode107Parameters
    },
    { // Mode 119
        1280,
        1024,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x39,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode119Parameters
    },
    { // Mode 11A
        1280,
        1024,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3A,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode11AParameters
    },
    { // Mode 11B
        1280,
        1024,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3B,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode11BParameters
    },
    { // Mode 11C
        1600,
        1200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3C,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        Mode11CParameters
    },
    { // Mode 11D
        1600,
        1200,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3D,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode11DParameters
    },
    { // Mode 11E
        1600,
        1200,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3E,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode11EParameters
    },
    { // Mode 11F
        1600,
        1200,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3F,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        NULL,
        Mode11FParameters
    },
    { // Mode 108
        80,
        60,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x28,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode108Parameters
    },
    { // Mode 109
        132,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x29,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode109Parameters
    },
    { // Mode 10A
        132,
        43,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x2A,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode10AParameters
    },
    { // Mode 10B
        132,
        50,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x2B,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode10BParameters
    },
    { // Mode 10C
        132,
        60,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        0x2C,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        Mode10CParameters
    },
    { // Mode X
        320,
        240,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x40,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        ModeXParameters
    },
    { // Mode Q
        256,
        256,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x41,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        ModeQParameters
    },
};

uint32_t s_NumDescriptors = sizeof(s_Descriptors) / sizeof(ModeDescriptor);

}}}}