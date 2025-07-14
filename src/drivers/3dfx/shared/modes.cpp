//Copyright 2025-Present riplin

#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/extmsapi.h>
#include <hag/drivers/vga/vga.h>
#include "modintl.h"

namespace Hag::TDfx::Shared::Data
{

//Horizontal timings:

VGA::ModeSetting::HorizontalTimings H01char80 = { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81 };
VGA::ModeSetting::HorizontalTimings H04pix640 = { 0x5F, 0x4F, 0x50, 0x82, 0x52, 0x9E };
VGA::ModeSetting::HorizontalTimings H05pix800 = { 0x7F, 0x63, 0x64, 0x82, 0x6B, 0x1B };
VGA::ModeSetting::HorizontalTimings H06pix800 = { 0x7F, 0x63, 0x64, 0x82, 0x69, 0x19 };
VGA::ModeSetting::HorizontalTimings H07pix1024 = { 0xA3, 0x7F, 0x80, 0x87, 0x83, 0x94 };
VGA::ModeSetting::HorizontalTimings H08pix1280 = { 0xCE, 0x9F, 0xA0, 0x91, 0xA6, 0x14 };
VGA::ModeSetting::HorizontalTimings H09char132 = { 0x9A, 0x83, 0x84, 0x9D, 0x85, 0x13 };
VGA::ModeSetting::HorizontalTimings H10char132 = { 0x9E, 0x83, 0x84, 0x81, 0x8A, 0x9E };
VGA::ModeSetting::HorizontalTimings H11pix320 = { 0x2D, 0x27, 0x28, 0x90, 0x29, 0x8F };
VGA::ModeSetting::HorizontalTimings H12pix400 = { 0x3D, 0x31, 0x32, 0x80, 0x35, 0x1D };
VGA::ModeSetting::HorizontalTimings H13pix512 = { 0x4F, 0x3F, 0x40, 0x83, 0x42, 0x0C };

//Vertical timings:

VGA::ModeSetting::VerticalTimings V00char50 = { 0xBF, 0x1F, 0x00, 0x9C, 0x0E, 0x8F, 0x96, 0xB9 };
VGA::ModeSetting::VerticalTimings V02line480 = { 0x0B, 0x3E, 0x00, 0xEA, 0x0C, 0xDF, 0xE7, 0x04 };
VGA::ModeSetting::VerticalTimings V03line600 = { 0x72, 0xF0, 0x20, 0x59, 0x0D, 0x57, 0x57, 0x73 };
VGA::ModeSetting::VerticalTimings V04line600 = { 0x72, 0xF0, 0x20, 0x59, 0x0D, 0x57, 0x58, 0x73 };
VGA::ModeSetting::VerticalTimings V05line768 = { 0x24, 0xF5, 0x20, 0x03, 0x09, 0xFF, 0xFF, 0x25 };
VGA::ModeSetting::VerticalTimings V06line1024 = { 0x28, 0x52, 0x00, 0x01, 0x04, 0xFF, 0x01, 0x28 };
VGA::ModeSetting::VerticalTimings V07char43 = { 0xBF, 0x1F, 0x00, 0x92, 0x04, 0x82, 0x89, 0xB9 };

//Resolution timings:

Function::ModeSetting::ResolutionTimings T12pix640x400 = { H01char80, V00char50, 0x40, 0x00, 0x0000, 0x01, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T13pix640x480 = { H04pix640, V02line480, 0xC0, 0x00, 0x0000, 0x01, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T14pix800x600 = { H05pix800, V03line600, 0x00, 0x00, 0xBC3E, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T15pix800x600 = { H06pix800, V04line600, 0x00, 0x00, 0xBC3E, 0x01, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T16pix1024x768 = { H07pix1024, V05line768, 0x00, 0x00, 0xE15D, 0x01, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T17pix1280x1024 = { H08pix1280, V06line1024, 0x00, 0x00, 0xB358, 0x01, 0x00, 0x51 };
Function::ModeSetting::ResolutionTimings T18char80x60 = { H01char80, V02line480, 0xC0, 0x00, 0x0000, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T19char132x25 = { H09char132, V00char50, 0x40, 0x00, 0xAEF4, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T20char132x43 = { H09char132, V07char43, 0x40, 0x00, 0xAEF4, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T21char132x50 = { H10char132, V00char50, 0x40, 0x00, 0xAEF4, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T22char132x60 = { H10char132, V02line480, 0xC0, 0x00, 0xAEF4, 0x00, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T23pix320x400 = { H11pix320, V00char50, 0x40, 0x80, 0xD1EA, 0x05, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T24pix320x480 = { H11pix320, V02line480, 0xC0, 0x80, 0xD1EA, 0x05, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T25pix400x600 = { H12pix400, V04line600, 0x00, 0x00, 0x56F4, 0x05, 0x00, 0x00 };
Function::ModeSetting::ResolutionTimings T26pix512x768 = { H13pix512, V05line768, 0x00, 0x00, 0x8DF4, 0x05, 0x00, 0x00 };

//Graphics:

VGA::GraphicsControllerData_t G00[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF };
VGA::GraphicsControllerData_t G04[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF };
VGA::GraphicsControllerData_t G07[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF };

//Attributes:

VGA::AttributeControllerData_t A01[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };
VGA::AttributeControllerData_t A07[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

//Configurations:

VGA::ModeSetting::Configuration C17 = { 0x50, 0x18, 0x10, 0xFFFF, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x1F, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C18 = { 0x50, 0x1D, 0x10, 0xFFFF, 0x23, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C19 = { 0x64, 0x24, 0x10, 0xFA00, 0x2F, { 0x01, 0x0F, 0x00, 0x06, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G04, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C20 = { 0x64, 0x24, 0x10, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C21 = { 0x80, 0x2F, 0x10, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C22 = { 0xA0, 0x3F, 0x10, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C23 = { 0x50, 0x3B, 0x08, 0x2600, 0x23, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C24 = { 0x84, 0x18, 0x10, 0x2000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C25 = { 0x84, 0x2A, 0x09, 0x4000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C26 = { 0x84, 0x31, 0x08, 0x4000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C27 = { 0x84, 0x3B, 0x08, 0x4000, 0x2F, { 0x01, 0x03, 0x00, 0x02, }, 0x00, 0x40, 0x80, 0x1F, 0xA3, 0xFF, G00, 0x0C, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C28 = { 0x28, 0x18, 0x08, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x80, 0x1F, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C29 = { 0x28, 0x1D, 0x08, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x1F, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C30 = { 0x32, 0x24, 0x08, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x01, 0x00, 0x0F, 0x00 };
VGA::ModeSetting::Configuration C31 = { 0x40, 0x1C, 0x0E, 0xFFFF, 0x2F, { 0x01, 0x0F, 0x00, 0x0E, }, 0x00, 0x40, 0x00, 0x00, 0xE3, 0xFF, G07, 0x41, 0x00, 0x0F, 0x00 };

//Video Parameters:

VGA::ModeSetting::VideoParameters P17 = { T12pix640x400, C17, A07, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P18 = { T13pix640x480, C18, A07, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P19 = { T14pix800x600, C19, A01, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P20 = { T15pix800x600, C20, A07, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P21 = { T16pix1024x768, C21, A07, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P22 = { T17pix1280x1024, C22, A07, VGA::Data::FontConfig12 };
VGA::ModeSetting::VideoParameters P23 = { T18char80x60, C23, A01, VGA::Data::FontConfig00 };
VGA::ModeSetting::VideoParameters P24 = { T19char132x25, C24, A01, VGA::Data::FontConfig02 };
VGA::ModeSetting::VideoParameters P25 = { T20char132x43, C25, A01, VGA::Data::FontConfig13 };
VGA::ModeSetting::VideoParameters P26 = { T21char132x50, C26, A01, VGA::Data::FontConfig00 };
VGA::ModeSetting::VideoParameters P27 = { T22char132x60, C27, A01, VGA::Data::FontConfig00 };
VGA::ModeSetting::VideoParameters P28 = { T23pix320x400, C28, A07, VGA::Data::FontConfig10 };
VGA::ModeSetting::VideoParameters P29 = { T24pix320x480, C29, A07, VGA::Data::FontConfig10 };
VGA::ModeSetting::VideoParameters P30 = { T25pix400x600, C30, A07, VGA::Data::FontConfig10 };
VGA::ModeSetting::VideoParameters P31 = { T26pix512x768, C31, A07, VGA::Data::FontConfig11 };

//Video Parameter Arrays:

const VGA::ModeSetting::VideoParameters* PA12[] = { &P17 };
const VGA::ModeSetting::VideoParameters* PA13[] = { &P18 };
const VGA::ModeSetting::VideoParameters* PA14[] = { &P19 };
const VGA::ModeSetting::VideoParameters* PA15[] = { &P20 };
const VGA::ModeSetting::VideoParameters* PA16[] = { &P21 };
const VGA::ModeSetting::VideoParameters* PA17[] = { &P22 };
const VGA::ModeSetting::VideoParameters* PA18[] = { &P23 };
const VGA::ModeSetting::VideoParameters* PA19[] = { &P24 };
const VGA::ModeSetting::VideoParameters* PA20[] = { &P25 };
const VGA::ModeSetting::VideoParameters* PA21[] = { &P26 };
const VGA::ModeSetting::VideoParameters* PA22[] = { &P27 };
const VGA::ModeSetting::VideoParameters* PA23[] = { &P28 };
const VGA::ModeSetting::VideoParameters* PA24[] = { &P29 };
const VGA::ModeSetting::VideoParameters* PA25[] = { &P30 };
const VGA::ModeSetting::VideoParameters* PA26[] = { &P31 };

Function::ModeSetting::ModeDescriptor s_Descriptors[] =
{
    { // Mode 100 parameter: 0x08
        640,
        400,
        640,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x5B,//Hag::Vesa::VideoMode::G640x400x8bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA12,
        0x50
    },
    { // Mode 101 parameter: 0x09
        640,
        480,
        640,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x5F,//Hag::Vesa::VideoMode::G640x480x8bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA13,
        0x50
    },
    { // Mode 102 parameter: 0x0A
        800,
        600,
        100,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Planar |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x6A,//Hag::Vesa::VideoMode::G800x600x4bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA14,
        0x32
    },
    { // Mode 103 parameter: 0x0B
        800,
        600,
        800,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x5C,//Hag::Vesa::VideoMode::G800x600x8bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA15,
        0x64
    },
    { // Mode 105 parameter: 0x0F
        1024,
        768,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x5E,//Hag::Vesa::VideoMode::G1024x768x8bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA16,
        0x80
    },
    { // Mode 107 parameter: 0x0C
        1280,
        1024,
        1280,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x6B,//Hag::Vesa::VideoMode::G1280x1024x8bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA17,
        0xA0
    },
    { // Mode 108 parameter: 0x10
        80,
        60,
        160,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x68,//Hag::Vesa::VideoMode::T80x60x4bpp
        0xB800,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA18,
        0x28
    },
    { // Mode 109 parameter: 0x1D
        132,
        25,
        264,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x55,//Hag::Vesa::VideoMode::T132x25x4bpp
        0xB800,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA19,
        0x42
    },
    { // Mode 10A parameter: 0x1E
        132,
        43,
        264,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x54,//Hag::Vesa::VideoMode::T132x43x4bpp
        0xB800,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA20,
        0x42
    },
    { // Mode 10B parameter: 0x1F
        132,
        50,
        264,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x65,//Hag::Vesa::VideoMode::T132x50x4bpp
        0xB800,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA21,
        0x42
    },
    { // Mode 10C parameter: 0x20
        132,
        60,
        264,
        VGA::ModeSetting::BitsPerPixel::Bpp4,
        VGA::ModeSetting::Flags::Text |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x64,//Hag::Vesa::VideoMode::T132x60x4bpp
        0xB800,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::EGAPair,
        PA22,
        0x42
    },
    { // Mode 180 parameter: 0x21
        320,
        200,
        320,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x78,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA23,
        0x28
    },
    { // Mode 181 parameter: 0x22
        320,
        240,
        320,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x20,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA24,
        0x28
    },
    { // Mode 184 parameter: 0x23
        400,
        300,
        400,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x23,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA25,
        0x64
    },
    { // Mode 187 parameter: 0x24
        512,
        384,
        512,
        VGA::ModeSetting::BitsPerPixel::Bpp8,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x26,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        VGA::Data::MCGAPairs,
        PA26,
        0x80
    },
    { // Mode 10E parameter: 0x21
        320,
        200,
        640,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x79,//Hag::Vesa::VideoMode::G320x200x16bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23,
        0x28
    },
    { // Mode 10F parameter: 0x21
        320,
        200,
        960,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x7A,//Hag::Vesa::VideoMode::G320x200x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23,
        0x28
    },
    { // Mode 10F parameter: 0x21
        320,
        200,
        1280,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x7A,//Hag::Vesa::VideoMode::G320x200x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA23,
        0x28
    },
    { // Mode 182 parameter: 0x22
        320,
        240,
        640,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x21,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24,
        0x28
    },
    { // Mode 183 parameter: 0x22
        320,
        240,
        960,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x22,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24,
        0x28
    },
    { // Mode 183 parameter: 0x22
        320,
        240,
        1280,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x22,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA24,
        0x28
    },
    { // Mode 185 parameter: 0x23
        400,
        300,
        800,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x24,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25,
        0x64
    },
    { // Mode 186 parameter: 0x23
        400,
        300,
        1200,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x25,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25,
        0x64
    },
    { // Mode 186 parameter: 0x23
        400,
        300,
        1600,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x25,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA25,
        0x64
    },
    { // Mode 188 parameter: 0x24
        512,
        384,
        1024,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x27,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26,
        0x80
    },
    { // Mode 189 parameter: 0x24
        512,
        384,
        1536,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x28,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26,
        0x80
    },
    { // Mode 189 parameter: 0x24
        512,
        384,
        2048,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x28,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA26,
        0x80
    },
    { // Mode 18A parameter: 0x08
        640,
        400,
        640,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x29,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA12,
        0x50
    },
    { // Mode 18B parameter: 0x08
        640,
        400,
        1920,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x2A,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA12,
        0x50
    },
    { // Mode 18B parameter: 0x08
        640,
        400,
        2560,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R70Hz,
        0x2A,
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA12,
        0x50
    },
    { // Mode 111 parameter: 0x09
        640,
        480,
        1280,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x6E,//Hag::Vesa::VideoMode::G640x480x16bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA13,
        0x50
    },
    { // Mode 112 parameter: 0x09
        640,
        480,
        1920,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x69,//Hag::Vesa::VideoMode::G640x480x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA13,
        0x50
    },
    { // Mode 112 parameter: 0x09
        640,
        480,
        2560,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x69,//Hag::Vesa::VideoMode::G640x480x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA13,
        0x50
    },
    { // Mode 114 parameter: 0x0B
        800,
        600,
        1600,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x70,//Hag::Vesa::VideoMode::G800x600x16bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA15,
        0x64
    },
    { // Mode 115 parameter: 0x0B
        800,
        600,
        2400,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x71,//Hag::Vesa::VideoMode::G800x600x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA15,
        0x64
    },
    { // Mode 115 parameter: 0x0B
        800,
        600,
        3200,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x71,//Hag::Vesa::VideoMode::G800x600x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA15,
        0x64
    },
    { // Mode 117 parameter: 0x0F
        1024,
        768,
        2048,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x72,//Hag::Vesa::VideoMode::G1024x768x16bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA16,
        0x80
    },
    { // Mode 118 parameter: 0x0F
        1024,
        768,
        3072,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x73,//Hag::Vesa::VideoMode::G1024x768x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA16,
        0x80
    },
    { // Mode 118 parameter: 0x0F
        1024,
        768,
        4096,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x73,//Hag::Vesa::VideoMode::G1024x768x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA16,
        0x80
    },
    { // Mode 11A parameter: 0x0C
        1280,
        1024,
        2560,
        VGA::ModeSetting::BitsPerPixel::Bpp16,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        VGA::ModeSetting::Flags::Accelerate3D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x74,//Hag::Vesa::VideoMode::G1280x1024x16bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA17,
        0xA0
    },
    { // Mode 11B parameter: 0x0C
        1280,
        1024,
        3840,
        VGA::ModeSetting::BitsPerPixel::Bpp24,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x75,//Hag::Vesa::VideoMode::G1280x1024x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA17,
        0xA0
    },
    { // Mode 11B parameter: 0x0C
        1280,
        1024,
        5120,
        VGA::ModeSetting::BitsPerPixel::Bpp32,
        VGA::ModeSetting::Flags::Graphics |
        VGA::ModeSetting::Flags::Color |
        VGA::ModeSetting::Flags::Sequential |
        VGA::ModeSetting::Flags::SingleParameter |
        VGA::ModeSetting::Flags::LinearFramebuffer |
        VGA::ModeSetting::Flags::Accelerate2D |
        Function::ModeSetting::Flags::TDfx,
        VGA::ModeSetting::RefreshRate::R60Hz,
        0x75,//Hag::Vesa::VideoMode::G1280x1024x32bpp
        0xA000,
        VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        VGA::ModeSetting::Scanlines::Invalid,
        0xFF,
        0xFF,
        nullptr,
        PA17,
        0xA0
    }
};

uint32_t s_NumDescriptors = sizeof(s_Descriptors) / sizeof(Function::ModeSetting::ModeDescriptor);

}