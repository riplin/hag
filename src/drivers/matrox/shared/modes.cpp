//Copyright 2025-Present riplin

#include <hag/drivers/matrox/shared/data.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include "modintl.h"

namespace Hag::Matrox::Shared::Data
{

using namespace Hag::Matrox::Shared::Function::ModeSetting;

//Horizontal timings:

HorizontalTimings H00char40 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0 };
HorizontalTimings H01char80 = { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81 };
HorizontalTimings H02char80 = { 0x60, 0x4F, 0x50, 0x83, 0x52, 0x9E };
HorizontalTimings H03char132 = { 0x9E, 0x83, 0x84, 0x81, 0x87, 0x8D };
HorizontalTimings H04pix640 = { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80 };
HorizontalTimings H05pix320 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80 };
HorizontalTimings H06pix800 = { 0x7F, 0x63, 0x63, 0x82, 0x6B, 0x1B };
HorizontalTimings H07pix512 = { 0x5F, 0x3F, 0x40, 0x82, 0x4E, 0x9A };
HorizontalTimings H08pix320 = { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E };
HorizontalTimings H09pix400 = { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C };
HorizontalTimings H10pix512 = { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06 };
HorizontalTimings H11pix640 = { 0x67, 0x4F, 0x4F, 0x8B, 0x53, 0x9F };
HorizontalTimings H12pix640 = { 0x5F, 0x4F, 0x4F, 0x83, 0x51, 0x9D };
HorizontalTimings H13pix800 = { 0x7F, 0x63, 0x63, 0x83, 0x68, 0x18 };
HorizontalTimings H14pix1024 = { 0xA3, 0x7F, 0x7F, 0x87, 0x82, 0x93 };
HorizontalTimings H15pix1280 = { 0xCE, 0x9F, 0x9F, 0x92, 0xA5, 0x13 };
HorizontalTimings H16pix1600 = { 0x09, 0xC7, 0xC7, 0x8D, 0xCF, 0x07 };

//Vertical timings:

VerticalTimings V00char50 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x96, 0xB9 };
VerticalTimings V01char25 = { 0xBF, 0x1F, 0x00, 0x83, 0x05, 0x5D, 0x63, 0xBA };
VerticalTimings V02char60 = { 0x0B, 0x3E, 0x00, 0xEB, 0x0C, 0xDF, 0xE6, 0x06 };
VerticalTimings V03char25 = { 0xC0, 0x1F, 0x00, 0x9E, 0x06, 0x8F, 0x96, 0xBB };
VerticalTimings V04char43 = { 0x88, 0x1F, 0x00, 0x66, 0x04, 0x57, 0x5E, 0x83 };
VerticalTimings V05char60 = { 0x11, 0x3E, 0x00, 0xEB, 0x0E, 0xDF, 0xE6, 0x0C };
VerticalTimings V06line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x04 };
VerticalTimings V07line600 = { 0x72, 0xF0, 0x20, 0x58, 0x0C, 0x57, 0x57, 0x73 };
VerticalTimings V08line480 = { 0x0D, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x06 };
VerticalTimings V09line512 = { 0x23, 0xB2, 0x20, 0x0A, 0x0C, 0xFF, 0x07, 0x17 };
VerticalTimings V10line400 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x8F, 0xC0 };
VerticalTimings V11line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xDF, 0x0C };
VerticalTimings V12line600 = { 0x99, 0xF0, 0x20, 0x68, 0x0E, 0x57, 0x57, 0x9A };
VerticalTimings V13line768 = { 0x23, 0xF5, 0x20, 0x02, 0x08, 0xFF, 0xFF, 0x24 };
VerticalTimings V14line400 = { 0xC0, 0x1F, 0x00, 0x97, 0x0D, 0x8F, 0x8F, 0xC1 };
VerticalTimings V15line480 = { 0x0B, 0x3E, 0x00, 0xE9, 0x0B, 0xDF, 0xDF, 0x0C };
VerticalTimings V16line768 = { 0x24, 0xF5, 0x20, 0x02, 0x08, 0xFF, 0xFF, 0x25 };
VerticalTimings V17line1024 = { 0x28, 0x5A, 0x20, 0x00, 0x03, 0xFF, 0xFF, 0x29 };
VerticalTimings V18line1200 = { 0xE0, 0x00, 0x00, 0xB0, 0x03, 0xAF, 0xAF, 0xE1 };

//Resolution timings:

ResolutionTimings T01char40x50 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H00char40, V00char50 };
ResolutionTimings T02char80x25 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H01char80, V01char25 };
ResolutionTimings T03char80x25 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H01char80, V00char50 };
ResolutionTimings T04char40x25 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H00char40, V01char25 };
ResolutionTimings T05char40x25 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H00char40, V00char50 };
ResolutionTimings T06char80x50 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H01char80, V00char50 };
ResolutionTimings T07char80x60 = { 0x00006270, 0x00, 0x00, 0xC0, 0x00, H02char80, V02char60 };
ResolutionTimings T08char132x25 = { 0x0000A078, 0x00, 0x00, 0x40, 0x00, H03char132, V03char25 };
ResolutionTimings T09char132x43 = { 0x0000A078, 0x00, 0x00, 0x40, 0x00, H03char132, V04char43 };
ResolutionTimings T10char132x60 = { 0x0000A078, 0x00, 0x00, 0xC0, 0x00, H03char132, V05char60 };
ResolutionTimings T11pix640x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H04pix640, V00char50 };
ResolutionTimings T12pix640x350 = { 0x00000000, 0x00, 0x00, 0x80, 0x00, H04pix640, V01char25 };
ResolutionTimings T13pix640x480 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H04pix640, V06line480 };
ResolutionTimings T14pix320x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x80, H05pix320, V00char50 };
ResolutionTimings T15pix800x600 = { 0x00009C40, 0x00, 0x00, 0x00, 0x00, H06pix800, V07line600 };
ResolutionTimings T16pix640x400 = { 0x00000000, 0x00, 0x00, 0x40, 0x00, H04pix640, V00char50 };
ResolutionTimings T17pix640x480 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H04pix640, V08line480 };
ResolutionTimings T18pix512x512 = { 0x00000000, 0x00, 0x00, 0xC0, 0x00, H07pix512, V09line512 };
ResolutionTimings T19pix320x400 = { 0x00003159, 0x00, 0x00, 0x40, 0x00, H08pix320, V10line400 };
ResolutionTimings T20pix320x480 = { 0x00003159, 0x00, 0x00, 0xC0, 0x00, H08pix320, V11line480 };
ResolutionTimings T21pix400x600 = { 0x00005F41, 0x00, 0x00, 0xC0, 0x00, H09pix400, V12line600 };
ResolutionTimings T22pix512x768 = { 0x00008FA3, 0x00, 0x00, 0xC0, 0x00, H10pix512, V13line768 };
ResolutionTimings T23pix640x400 = { 0x00006A86, 0x40, 0x00, 0x40, 0x00, H11pix640, V14line400 };
ResolutionTimings T24pix640x480 = { 0x00006257, 0x40, 0x00, 0xC0, 0x00, H12pix640, V15line480 };
ResolutionTimings T25pix800x600 = { 0x00009C40, 0x00, 0x00, 0x00, 0x00, H13pix800, V07line600 };
ResolutionTimings T26pix1024x768 = { 0x0000FDE8, 0x00, 0x00, 0xC0, 0x00, H14pix1024, V16line768 };
ResolutionTimings T27pix1280x1024 = { 0x0001A5E0, 0x40, 0x21, 0x00, 0x00, H15pix1280, V17line1024 };
ResolutionTimings T28pix1600x1200 = { 0x000278D0, 0x01, 0x2D, 0x00, 0x00, H16pix1600, V18line1200 };

//Graphics:

VGA::GraphicsControllerData_t G00[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF };
VGA::GraphicsControllerData_t G01[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0A, 0x00, 0xFF };
VGA::GraphicsControllerData_t G02[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0xFF };
VGA::GraphicsControllerData_t G03[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0xFF };
VGA::GraphicsControllerData_t G04[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0xFF };
VGA::GraphicsControllerData_t G05[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0F, 0x00, 0xFF };
VGA::GraphicsControllerData_t G06[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF };
VGA::GraphicsControllerData_t G07[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF };

//Attributes:

VGA::AttributeControllerData_t A00[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
VGA::AttributeControllerData_t A01[] = { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 };
VGA::AttributeControllerData_t A02[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
VGA::AttributeControllerData_t A03[] = { 0x00, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17 };
VGA::AttributeControllerData_t A04[] = { 0x00, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00 };
VGA::AttributeControllerData_t A05[] = { 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F };
VGA::AttributeControllerData_t A06[] = { 0x00, 0x13, 0x15, 0x17, 0x02, 0x04, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 };
VGA::AttributeControllerData_t A07[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

//Configurations:

Configuration C00 = { 0x28, 0x18, 0x08, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C01 = { 0x50, 0x18, 0x0E, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x03, }, 0x00, 0x40, 0x80, 0x0D, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
Configuration C02 = { 0x50, 0x18, 0x10, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x0F, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
Configuration C03 = { 0x28, 0x18, 0x0E, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C04 = { 0x28, 0x18, 0x10, 0x0800, 0x27, { 0x08, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
Configuration C05 = { 0x50, 0x18, 0x08, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C06 = { 0x50, 0x18, 0x0E, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
Configuration C07 = { 0x50, 0x18, 0x10, 0x1000, 0x27, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
Configuration C08 = { 0x50, 0x31, 0x08, 0x2040, 0x27, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
Configuration C09 = { 0x50, 0x3B, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C10 = { 0x84, 0x18, 0x10, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C11 = { 0x84, 0x2A, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C12 = { 0x84, 0x31, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C13 = { 0x84, 0x3B, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
Configuration C14 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x01, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC2, 0xFF, G02, 0x01, 0x00, 0x01, 0x00 };
Configuration C15 = { 0x50, 0x18, 0x0E, 0x8000, 0x22, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G03, 0x0B, 0x00, 0x05, 0x00 };
Configuration C16 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G04, 0x01, 0x00, 0x0F, 0x00 };
Configuration C17 = { 0x28, 0x18, 0x08, 0x4000, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x00, 0xA2, 0xFF, G05, 0x01, 0x00, 0x03, 0x00 };
Configuration C18 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x09, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
Configuration C19 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
Configuration C20 = { 0x50, 0x18, 0x0E, 0x8000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
Configuration C21 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
Configuration C22 = { 0x64, 0x24, 0x10, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
Configuration C23 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x40, 0xA3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
Configuration C24 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
Configuration C25 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0xA0, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
Configuration C26 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0xA0, 0x40, 0xA3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
Configuration C27 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0xA0, 0x00, 0xC3, 0xFF, G07, 0x41, 0x02, 0x0F, 0x00 };
Configuration C28 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
Configuration C29 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x20, 0x00, 0xC3, 0xFF, G07, 0x41, 0x02, 0x0F, 0x00 };

//Font Configurations:

FontConfiguration F00 = { Data::Font8x8, 0xFF, 0x08, 0x00, false, 0x07, 0x06, 0x07 };
FontConfiguration F01 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x0D, 0x0B, 0x0C };
FontConfiguration F02 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x0F, 0x0D, 0x0E };
FontConfiguration F03 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x07, 0x06, 0x07 };
FontConfiguration F04 = { Data::Font8x16, 0xFF, 0x00, 0x00, true, 0x0D, 0x0B, 0x0C };
FontConfiguration F05 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x00, 0x00 };
FontConfiguration F06 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x00, 0x00 };
FontConfiguration F07 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x20, 0x00 };
FontConfiguration F08 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x20, 0x00 };

//Video Parameters:

VideoParameters P00 = { T01char40x50, C00, A00, F00 };
VideoParameters P01 = { T02char80x25, C01, A01, F01 };
VideoParameters P02 = { T03char80x25, C02, A01, F02 };
VideoParameters P03 = { T01char40x50, C00, A00, F03 };
VideoParameters P04 = { T04char40x25, C03, A02, F04 };
VideoParameters P05 = { T05char40x25, C04, A02, F02 };
VideoParameters P06 = { T06char80x50, C05, A00, F00 };
VideoParameters P07 = { T02char80x25, C06, A02, F04 };
VideoParameters P08 = { T03char80x25, C07, A02, F02 };
VideoParameters P09 = { T03char80x25, C08, A02, F00 };
VideoParameters P10 = { T07char80x60, C09, A02, F00 };
VideoParameters P11 = { T08char132x25, C10, A02, F02 };
VideoParameters P12 = { T09char132x43, C11, A02, F00 };
VideoParameters P13 = { T08char132x25, C12, A02, F00 };
VideoParameters P14 = { T10char132x60, C13, A02, F00 };
VideoParameters P15 = { T11pix640x400, C14, A03, F05 };
VideoParameters P16 = { T12pix640x350, C15, A04, F06 };
VideoParameters P17 = { T13pix640x480, C16, A05, F06 };
VideoParameters P18 = { T14pix320x400, C17, A06, F05 };
VideoParameters P19 = { T14pix320x400, C18, A00, F06 };
VideoParameters P20 = { T11pix640x400, C19, A00, F06 };
VideoParameters P21 = { T12pix640x350, C20, A02, F06 };
VideoParameters P22 = { T13pix640x480, C21, A02, F06 };
VideoParameters P23 = { T15pix800x600, C22, A02, F06 };
VideoParameters P24 = { T16pix640x400, C23, A07, F05 };
VideoParameters P25 = { T16pix640x400, C24, A07, F05 };
VideoParameters P26 = { T17pix640x480, C25, A07, F05 };
VideoParameters P27 = { T18pix512x512, C26, A07, F05 };
VideoParameters P28 = { T19pix320x400, C27, A07, F07 };
VideoParameters P29 = { T20pix320x480, C27, A07, F07 };
VideoParameters P30 = { T21pix400x600, C27, A07, F07 };
VideoParameters P31 = { T22pix512x768, C27, A07, F07 };
VideoParameters P32 = { T23pix640x400, C28, A07, F08 };
VideoParameters P33 = { T24pix640x480, C29, A07, F08 };
VideoParameters P34 = { T25pix800x600, C29, A07, F08 };
VideoParameters P35 = { T26pix1024x768, C29, A07, F08 };
VideoParameters P36 = { T27pix1280x1024, C29, A07, F08 };
VideoParameters P37 = { T28pix1600x1200, C29, A07, F08 };

//Video Parameter Arrays:

VideoParameters* PA00[] = { &P00, &P01, &P02, &P03 };
VideoParameters* PA01[] = { &P00, &P04, &P05, &P03 };
VideoParameters* PA02[] = { &P06, &P07, &P08, &P03 };
VideoParameters* PA03[] = { &P09 };
VideoParameters* PA04[] = { &P10 };
VideoParameters* PA05[] = { &P11 };
VideoParameters* PA06[] = { &P12 };
VideoParameters* PA07[] = { &P13 };
VideoParameters* PA08[] = { &P14 };
VideoParameters* PA09[] = { &P15 };
VideoParameters* PA10[] = { &P16 };
VideoParameters* PA11[] = { &P17 };
VideoParameters* PA12[] = { &P18 };
VideoParameters* PA13[] = { &P19 };
VideoParameters* PA14[] = { &P20 };
VideoParameters* PA15[] = { &P21 };
VideoParameters* PA16[] = { &P22 };
VideoParameters* PA17[] = { &P23 };
VideoParameters* PA18[] = { &P24 };
VideoParameters* PA19[] = { &P25 };
VideoParameters* PA20[] = { &P26 };
VideoParameters* PA21[] = { &P27 };
VideoParameters* PA22[] = { &P28 };
VideoParameters* PA23[] = { &P29 };
VideoParameters* PA24[] = { &P30 };
VideoParameters* PA25[] = { &P31 };
VideoParameters* PA26[] = { &P32 };
VideoParameters* PA27[] = { &P33 };
VideoParameters* PA28[] = { &P34 };
VideoParameters* PA29[] = { &P35 };
VideoParameters* PA30[] = { &P36 };
VideoParameters* PA31[] = { &P37 };

ModeDescriptor s_Descriptors[] =
{
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
        PA00
    },
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
        PA01
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
        PA02
    },
    { // Mode 3
        80,
        50,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::Vesa::VideoMode::T80x25x4bppC,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        PA03
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
        PA04
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
        PA05
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
        PA06
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
        PA07
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
        PA08
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
        PA09
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
        PA10
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
        PA11
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
        PA12
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
        PA13
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
        PA14
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
        PA15
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
        PA16
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
        PA17
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
        PA18
    },
    { // Mode X
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x14,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA19
    },
    { // Mode Q
        320,
        240,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x15,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA20
    },
    { // Mode 80x50
        256,
        256,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x16,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::S200,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA21
    },
    { // Mode 149
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x69,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 148
        320,
        240,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        0x68,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 145
        400,
        300,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        0x65,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 143
        512,
        384,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x63,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 100
        640,
        400,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x20,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA26
    },
    { // Mode 101
        640,
        480,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x21,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA27
    },
    { // Mode 103
        800,
        600,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x23,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA28
    },
    { // Mode 105
        1024,
        768,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x25,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA29
    },
    { // Mode 107
        1280,
        1024,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x27,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA30
    },
    { // Mode 11C
        1600,
        1200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3C,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA31
    },
    { // Mode 10D
        320,
        200,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x2D,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 139
        320,
        240,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        0x59,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 136
        400,
        300,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        0x56,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 134
        512,
        384,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x54,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 121
        640,
        400,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x41,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 110
        640,
        480,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x30,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 113
        800,
        600,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x33,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 116
        1024,
        768,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x36,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 119
        1280,
        1024,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x39,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11D
        1600,
        1200,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3D,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
    { // Mode 10E
        320,
        200,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x2E,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 1D9
        320,
        240,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        0xF9,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 1D6
        400,
        300,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        0xF6,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 1D4
        512,
        384,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0xF4,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 122
        640,
        400,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x42,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 111
        640,
        480,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x31,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 114
        800,
        600,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x34,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 117
        1024,
        768,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x37,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 11A
        1280,
        1024,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3A,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11E
        1600,
        1200,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3E,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
    { // Mode 10F
        320,
        200,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x2F,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 1B8
        320,
        240,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        0xD8,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 1B5
        400,
        300,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        0xD5,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 1B3
        512,
        384,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0xD3,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 124
        640,
        400,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x44,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 112
        640,
        480,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x32,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 115
        800,
        600,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x35,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 118
        1024,
        768,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x38,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 11B
        1280,
        1024,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3B,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11F
        1600,
        1200,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x3F,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
};

uint32_t s_NumDescriptors = sizeof(s_Descriptors) / sizeof(ModeDescriptor);

}