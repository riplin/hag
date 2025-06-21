//Copyright 2025-Present riplin

#include <hag/drivers/matrox/shared/data.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/vga.h>
#include <hag/drivers/matrox/shared/funcs/modeset.h>
#include "modintl.h"

namespace Hag::Matrox::Shared::Data
{

//Horizontal timings:

VGA::ModeSetting::HorizontalTimings H00char40 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0 };
VGA::ModeSetting::HorizontalTimings H01char80 = { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81 };
VGA::ModeSetting::HorizontalTimings H02char80 = { 0x60, 0x4F, 0x50, 0x83, 0x52, 0x9E };
VGA::ModeSetting::HorizontalTimings H03char132 = { 0x9E, 0x83, 0x84, 0x81, 0x87, 0x8D };
VGA::ModeSetting::HorizontalTimings H04pix640 = { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80 };
VGA::ModeSetting::HorizontalTimings H05pix320 = { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80 };
VGA::ModeSetting::HorizontalTimings H06pix800 = { 0x7F, 0x63, 0x63, 0x82, 0x6B, 0x1B };
VGA::ModeSetting::HorizontalTimings H07pix512 = { 0x5F, 0x3F, 0x40, 0x82, 0x4E, 0x9A };
VGA::ModeSetting::HorizontalTimings H08pix320 = { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E };
VGA::ModeSetting::HorizontalTimings H09pix400 = { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C };
VGA::ModeSetting::HorizontalTimings H10pix512 = { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06 };
VGA::ModeSetting::HorizontalTimings H11pix640 = { 0x67, 0x4F, 0x4F, 0x8B, 0x53, 0x9F };
VGA::ModeSetting::HorizontalTimings H12pix640 = { 0x5F, 0x4F, 0x4F, 0x83, 0x51, 0x9D };
VGA::ModeSetting::HorizontalTimings H13pix800 = { 0x7F, 0x63, 0x63, 0x83, 0x68, 0x18 };
VGA::ModeSetting::HorizontalTimings H14pix1024 = { 0xA3, 0x7F, 0x7F, 0x87, 0x82, 0x93 };
VGA::ModeSetting::HorizontalTimings H15pix1280 = { 0xCE, 0x9F, 0x9F, 0x92, 0xA5, 0x13 };
VGA::ModeSetting::HorizontalTimings H16pix1600 = { 0x09, 0xC7, 0xC7, 0x8D, 0xCF, 0x07 };

//Vertical timings:

VGA::ModeSetting::VerticalTimings V00char50 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x96, 0xB9 };
VGA::ModeSetting::VerticalTimings V01char25 = { 0xBF, 0x1F, 0x00, 0x83, 0x05, 0x5D, 0x63, 0xBA };
VGA::ModeSetting::VerticalTimings V02char60 = { 0x0B, 0x3E, 0x00, 0xEB, 0x0C, 0xDF, 0xE6, 0x06 };
VGA::ModeSetting::VerticalTimings V03char25 = { 0xC0, 0x1F, 0x00, 0x9E, 0x06, 0x8F, 0x96, 0xBB };
VGA::ModeSetting::VerticalTimings V04char43 = { 0x88, 0x1F, 0x00, 0x66, 0x04, 0x57, 0x5E, 0x83 };
VGA::ModeSetting::VerticalTimings V05char60 = { 0x11, 0x3E, 0x00, 0xEB, 0x0E, 0xDF, 0xE6, 0x0C };
VGA::ModeSetting::VerticalTimings V06line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x04 };
VGA::ModeSetting::VerticalTimings V07line600 = { 0x72, 0xF0, 0x20, 0x58, 0x0C, 0x57, 0x57, 0x73 };
VGA::ModeSetting::VerticalTimings V08line480 = { 0x0D, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x06 };
VGA::ModeSetting::VerticalTimings V09line512 = { 0x23, 0xB2, 0x20, 0x0A, 0x0C, 0xFF, 0x07, 0x17 };
VGA::ModeSetting::VerticalTimings V10line400 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x8F, 0xC0 };
VGA::ModeSetting::VerticalTimings V11line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xDF, 0x0C };
VGA::ModeSetting::VerticalTimings V12line600 = { 0x99, 0xF0, 0x20, 0x68, 0x0E, 0x57, 0x57, 0x9A };
VGA::ModeSetting::VerticalTimings V13line768 = { 0x23, 0xF5, 0x20, 0x02, 0x08, 0xFF, 0xFF, 0x24 };
VGA::ModeSetting::VerticalTimings V14line400 = { 0xC0, 0x1F, 0x00, 0x97, 0x0D, 0x8F, 0x8F, 0xC1 };
VGA::ModeSetting::VerticalTimings V15line480 = { 0x0B, 0x3E, 0x00, 0xE9, 0x0B, 0xDF, 0xDF, 0x0C };
VGA::ModeSetting::VerticalTimings V16line768 = { 0x24, 0xF5, 0x20, 0x02, 0x08, 0xFF, 0xFF, 0x25 };
VGA::ModeSetting::VerticalTimings V17line1024 = { 0x28, 0x5A, 0x20, 0x00, 0x03, 0xFF, 0xFF, 0x29 };
VGA::ModeSetting::VerticalTimings V18line1200 = { 0xE0, 0x00, 0x00, 0xB0, 0x03, 0xAF, 0xAF, 0xE1 };

//Resolution timings:

Function::ModeSetting::ResolutionTimings T01char40x50 = { H00char40, V00char50, 0x40, 0x80, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T02char80x25 = { H01char80, V01char25, 0x80, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T03char80x25 = { H01char80, V00char50, 0x40, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T04char40x25 = { H00char40, V01char25, 0x80, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T05char40x25 = { H00char40, V00char50, 0x40, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T06char80x50 = { H01char80, V00char50, 0x40, 0x80, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T07char80x60 = { H02char80, V02char60, 0xC0, 0x00, 0x00006270, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T08char132x25 = { H03char132, V03char25, 0x40, 0x00, 0x0000A078, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T09char132x43 = { H03char132, V04char43, 0x40, 0x00, 0x0000A078, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T10char132x60 = { H03char132, V05char60, 0xC0, 0x00, 0x0000A078, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T11pix640x400 = { H04pix640, V00char50, 0x40, 0x80, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T12pix640x350 = { H04pix640, V01char25, 0x80, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T13pix640x480 = { H04pix640, V06line480, 0xC0, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T14pix320x400 = { H05pix320, V00char50, 0x40, 0x80, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T15pix800x600 = { H06pix800, V07line600, 0x00, 0x00, 0x00009C40, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T16pix640x400 = { H04pix640, V00char50, 0x40, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T17pix640x480 = { H04pix640, V08line480, 0xC0, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T18pix512x512 = { H07pix512, V09line512, 0xC0, 0x00, 0x00000000, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T19pix320x400 = { H08pix320, V10line400, 0x40, 0x00, 0x00003159, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T20pix320x480 = { H08pix320, V11line480, 0xC0, 0x00, 0x00003159, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T21pix400x600 = { H09pix400, V12line600, 0xC0, 0x00, 0x00005F41, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T22pix512x768 = { H10pix512, V13line768, 0xC0, 0x00, 0x00008FA3, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T23pix640x400 = { H11pix640, V14line400, 0x40, 0x00, 0x00006A86, 0x40, 0x00 };
Function::ModeSetting::ResolutionTimings T24pix640x480 = { H12pix640, V15line480, 0xC0, 0x00, 0x00006257, 0x40, 0x00 };
Function::ModeSetting::ResolutionTimings T25pix800x600 = { H13pix800, V07line600, 0x00, 0x00, 0x00009C40, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T26pix1024x768 = { H14pix1024, V16line768, 0xC0, 0x00, 0x0000FDE8, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T27pix1280x1024 = { H15pix1280, V17line1024, 0x00, 0x00, 0x0001A5E0, 0x40, 0x21 };
Function::ModeSetting::ResolutionTimings T28pix1600x1200 = { H16pix1600, V18line1200, 0x00, 0x00, 0x000278D0, 0x01, 0x2D };

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

VGA::ModeSetting::Configuration C00 = { 0x28, 0x18, 0x08, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C01 = { 0x50, 0x18, 0x0E, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x03, }, 0x00, 0x40, 0x80, 0x0D, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
VGA::ModeSetting::Configuration C02 = { 0x50, 0x18, 0x10, 0x1000, 0x26, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x0F, 0xA3, 0xFF, G01, 0x0E, 0x00, 0x0F, 0x08 };
VGA::ModeSetting::Configuration C03 = { 0x28, 0x18, 0x0E, 0x0800, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C04 = { 0x28, 0x18, 0x10, 0x0800, 0x27, { 0x08, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
VGA::ModeSetting::Configuration C05 = { 0x50, 0x18, 0x08, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C06 = { 0x50, 0x18, 0x0E, 0x1000, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x08, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C07 = { 0x50, 0x18, 0x10, 0x1000, 0x27, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
VGA::ModeSetting::Configuration C08 = { 0x50, 0x31, 0x08, 0x2040, 0x27, { 0x00, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x08 };
VGA::ModeSetting::Configuration C09 = { 0x50, 0x3B, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C10 = { 0x84, 0x18, 0x10, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C11 = { 0x84, 0x2A, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C12 = { 0x84, 0x31, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C13 = { 0x84, 0x3B, 0x08, 0x0000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C14 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x01, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC2, 0xFF, G02, 0x01, 0x00, 0x01, 0x00 };
VGA::ModeSetting::Configuration C15 = { 0x50, 0x18, 0x0E, 0x8000, 0x22, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G03, 0x0B, 0x00, 0x05, 0x00 };
VGA::ModeSetting::Configuration C16 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G04, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C17 = { 0x28, 0x18, 0x08, 0x4000, 0x23, { 0x09, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x00, 0xA2, 0xFF, G05, 0x01, 0x00, 0x03, 0x00 };
VGA::ModeSetting::Configuration C18 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x09, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C19 = { 0x50, 0x18, 0x08, 0x4000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C20 = { 0x50, 0x18, 0x0E, 0x8000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x0F, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C21 = { 0x50, 0x1D, 0x10, 0xA000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C22 = { 0x64, 0x24, 0x10, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G06, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C23 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x40, 0xA3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C24 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x80, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C25 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0xA0, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C26 = { 0x28, 0x18, 0x08, 0x2000, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0xA0, 0x40, 0xA3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C27 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0xA0, 0x00, 0xC3, 0xFF, G07, 0x41, 0x02, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C28 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x00, 0xC3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C29 = { 0x50, 0x0B, 0x08, 0x0000, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x20, 0x00, 0xC3, 0xFF, G07, 0x41, 0x02, 0x0F, 0x00 };

//Font Configurations:

VGA::ModeSetting::FontConfiguration F00 = { Data::Font8x8, 0xFF, 0x08, 0x00, false, 0x07, 0x06, 0x07 };
VGA::ModeSetting::FontConfiguration F01 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x0D, 0x0B, 0x0C };
VGA::ModeSetting::FontConfiguration F02 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x0F, 0x0D, 0x0E };
VGA::ModeSetting::FontConfiguration F03 = { Data::Font8x16, 0xFF, 0x10, 0x00, true, 0x07, 0x06, 0x07 };
VGA::ModeSetting::FontConfiguration F04 = { Data::Font8x16, 0xFF, 0x00, 0x00, true, 0x0D, 0x0B, 0x0C };
VGA::ModeSetting::FontConfiguration F05 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x00, 0x00 };
VGA::ModeSetting::FontConfiguration F06 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x00, 0x00 };
VGA::ModeSetting::FontConfiguration F07 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x01, 0x20, 0x00 };
VGA::ModeSetting::FontConfiguration F08 = { Data::Font8x16, 0xFF, 0x00, 0x00, false, 0x00, 0x20, 0x00 };

//Video Parameters:

VGA::ModeSetting::VideoParameters P00 = { T01char40x50, C00, A00, F00 };
VGA::ModeSetting::VideoParameters P01 = { T02char80x25, C01, A01, F01 };
VGA::ModeSetting::VideoParameters P02 = { T03char80x25, C02, A01, F02 };
VGA::ModeSetting::VideoParameters P03 = { T01char40x50, C00, A00, F03 };
VGA::ModeSetting::VideoParameters P04 = { T04char40x25, C03, A02, F04 };
VGA::ModeSetting::VideoParameters P05 = { T05char40x25, C04, A02, F02 };
VGA::ModeSetting::VideoParameters P06 = { T06char80x50, C05, A00, F00 };
VGA::ModeSetting::VideoParameters P07 = { T02char80x25, C06, A02, F04 };
VGA::ModeSetting::VideoParameters P08 = { T03char80x25, C07, A02, F02 };
VGA::ModeSetting::VideoParameters P09 = { T03char80x25, C08, A02, F00 };
VGA::ModeSetting::VideoParameters P10 = { T07char80x60, C09, A02, F00 };
VGA::ModeSetting::VideoParameters P11 = { T08char132x25, C10, A02, F02 };
VGA::ModeSetting::VideoParameters P12 = { T09char132x43, C11, A02, F00 };
VGA::ModeSetting::VideoParameters P13 = { T08char132x25, C12, A02, F00 };
VGA::ModeSetting::VideoParameters P14 = { T10char132x60, C13, A02, F00 };
VGA::ModeSetting::VideoParameters P15 = { T11pix640x400, C14, A03, F05 };
VGA::ModeSetting::VideoParameters P16 = { T12pix640x350, C15, A04, F06 };
VGA::ModeSetting::VideoParameters P17 = { T13pix640x480, C16, A05, F06 };
VGA::ModeSetting::VideoParameters P18 = { T14pix320x400, C17, A06, F05 };
VGA::ModeSetting::VideoParameters P19 = { T14pix320x400, C18, A00, F06 };
VGA::ModeSetting::VideoParameters P20 = { T11pix640x400, C19, A00, F06 };
VGA::ModeSetting::VideoParameters P21 = { T12pix640x350, C20, A02, F06 };
VGA::ModeSetting::VideoParameters P22 = { T13pix640x480, C21, A02, F06 };
VGA::ModeSetting::VideoParameters P23 = { T15pix800x600, C22, A02, F06 };
VGA::ModeSetting::VideoParameters P24 = { T16pix640x400, C23, A07, F05 };
VGA::ModeSetting::VideoParameters P25 = { T16pix640x400, C24, A07, F05 };
VGA::ModeSetting::VideoParameters P26 = { T17pix640x480, C25, A07, F05 };
VGA::ModeSetting::VideoParameters P27 = { T18pix512x512, C26, A07, F05 };
VGA::ModeSetting::VideoParameters P28 = { T19pix320x400, C27, A07, F07 };
VGA::ModeSetting::VideoParameters P29 = { T20pix320x480, C27, A07, F07 };
VGA::ModeSetting::VideoParameters P30 = { T21pix400x600, C27, A07, F07 };
VGA::ModeSetting::VideoParameters P31 = { T22pix512x768, C27, A07, F07 };
VGA::ModeSetting::VideoParameters P32 = { T23pix640x400, C28, A07, F08 };
VGA::ModeSetting::VideoParameters P33 = { T24pix640x480, C29, A07, F08 };
VGA::ModeSetting::VideoParameters P34 = { T25pix800x600, C29, A07, F08 };
VGA::ModeSetting::VideoParameters P35 = { T26pix1024x768, C29, A07, F08 };
VGA::ModeSetting::VideoParameters P36 = { T27pix1280x1024, C29, A07, F08 };
VGA::ModeSetting::VideoParameters P37 = { T28pix1600x1200, C29, A07, F08 };

//Video Parameter Arrays:

const VGA::ModeSetting::VideoParameters* PA00[] = { &P00, &P01, &P02, &P03 };
const VGA::ModeSetting::VideoParameters* PA01[] = { &P00, &P04, &P05, &P03 };
const VGA::ModeSetting::VideoParameters* PA02[] = { &P06, &P07, &P08, &P03 };
const VGA::ModeSetting::VideoParameters* PA03[] = { &P09 };
const VGA::ModeSetting::VideoParameters* PA04[] = { &P10 };
const VGA::ModeSetting::VideoParameters* PA05[] = { &P11 };
const VGA::ModeSetting::VideoParameters* PA06[] = { &P12 };
const VGA::ModeSetting::VideoParameters* PA07[] = { &P13 };
const VGA::ModeSetting::VideoParameters* PA08[] = { &P14 };
const VGA::ModeSetting::VideoParameters* PA09[] = { &P15 };
const VGA::ModeSetting::VideoParameters* PA10[] = { &P16 };
const VGA::ModeSetting::VideoParameters* PA11[] = { &P17 };
const VGA::ModeSetting::VideoParameters* PA12[] = { &P18 };
const VGA::ModeSetting::VideoParameters* PA13[] = { &P19 };
const VGA::ModeSetting::VideoParameters* PA14[] = { &P20 };
const VGA::ModeSetting::VideoParameters* PA15[] = { &P21 };
const VGA::ModeSetting::VideoParameters* PA16[] = { &P22 };
const VGA::ModeSetting::VideoParameters* PA17[] = { &P23 };
const VGA::ModeSetting::VideoParameters* PA18[] = { &P24 };
const VGA::ModeSetting::VideoParameters* PA19[] = { &P25 };
const VGA::ModeSetting::VideoParameters* PA20[] = { &P26 };
const VGA::ModeSetting::VideoParameters* PA21[] = { &P27 };
const VGA::ModeSetting::VideoParameters* PA22[] = { &P28 };
const VGA::ModeSetting::VideoParameters* PA23[] = { &P29 };
const VGA::ModeSetting::VideoParameters* PA24[] = { &P30 };
const VGA::ModeSetting::VideoParameters* PA25[] = { &P31 };
const VGA::ModeSetting::VideoParameters* PA26[] = { &P32 };
const VGA::ModeSetting::VideoParameters* PA27[] = { &P33 };
const VGA::ModeSetting::VideoParameters* PA28[] = { &P34 };
const VGA::ModeSetting::VideoParameters* PA29[] = { &P35 };
const VGA::ModeSetting::VideoParameters* PA30[] = { &P36 };
const VGA::ModeSetting::VideoParameters* PA31[] = { &P37 };

VGA::ModeSetting::ModeDescriptor s_Descriptors[] =
{
    { // Mode 108
        80,
        60,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x28,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        PA04
    },
    { // Mode 109
        132,
        25,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x29,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        PA05
    },
    { // Mode 10A
        132,
        43,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x2A,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        PA06
    },
    { // Mode 10B
        132,
        50,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x2B,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        PA07
    },
    { // Mode 10C
        132,
        60,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x2C,
        0xB800,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        Data::PalettePair0,
        PA08
    },
    { // Mode 102
        800,
        600,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Planar | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCDisable | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x22,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair0,
        PA17
    },
    { // Mode 149
        320,
        200,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x69,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 148
        320,
        240,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x68,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 145
        400,
        300,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter |Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R72Hz,
        0x65,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 143
        512,
        384,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x63,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 100
        640,
        400,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x20,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA26
    },
    { // Mode 101
        640,
        480,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x21,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA27
    },
    { // Mode 103
        800,
        600,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x23,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA28
    },
    { // Mode 105
        1024,
        768,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x25,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA29
    },
    { // Mode 107
        1280,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x27,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA30
    },
    { // Mode 11C
        1600,
        1200,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3C,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        Data::PalettePair578,
        PA31
    },
    { // Mode 10D
        320,
        200,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x2D,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 139
        320,
        240,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x59,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 136
        400,
        300,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R72Hz,
        0x56,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 134
        512,
        384,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x54,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 121
        640,
        400,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x41,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 110
        640,
        480,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x30,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 113
        800,
        600,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x33,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 116
        1024,
        768,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x36,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 119
        1280,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x39,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11D
        1600,
        1200,
        VGA::ModeSetting::BitsPerPixel::Bpp15,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3D,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
    { // Mode 10E
        320,
        200,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x2E,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 1D9
        320,
        240,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0xF9,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 1D6
        400,
        300,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R72Hz,
        0xF6,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 1D4
        512,
        384,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0xF4,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 122
        640,
        400,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x42,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 111
        640,
        480,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x31,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 114
        800,
        600,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x34,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 117
        1024,
        768,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x37,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 11A
        1280,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3A,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11E
        1600,
        1200,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3E,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
    { // Mode 10F
        320,
        200,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x2F,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA22
    },
    { // Mode 1B8
        320,
        240,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0xD8,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23
    },
    { // Mode 1B5
        400,
        300,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R72Hz,
        0xD5,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24
    },
    { // Mode 1B3
        512,
        384,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0xD3,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25
    },
    { // Mode 124
        640,
        400,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x44,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26
    },
    { // Mode 112
        640,
        480,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x32,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA27
    },
    { // Mode 115
        800,
        600,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x35,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA28
    },
    { // Mode 118
        1024,
        768,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x38,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA29
    },
    { // Mode 11B
        1280,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3B,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA30
    },
    { // Mode 11F
        1600,
        1200,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics | VGA::ModeSetting::Flags::Color | VGA::ModeSetting::Flags::Sequential | VGA::ModeSetting::Flags::LinearFramebuffer | VGA::ModeSetting::Flags::SingleParameter | Function::ModeSetting::Flags::MAFCVGA | Function::ModeSetting::Flags::Matrox,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x3F,
        0xA000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA31
    },
};

uint32_t s_NumDescriptors = sizeof(s_Descriptors) / sizeof(VGA::ModeSetting::ModeDescriptor);

}