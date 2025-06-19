//Copyright 2023-Present riplin

#include <hag/system/bda.h>
#include <hag/drivers/s3/trio64/data.h>

namespace Hag::S3::Trio64::Data
{

Shared::VESAModeTranslation VideoModeTranslation[] =
{
    { VesaVideoMode::G640x400x8bpp, VideoMode::G640x400x8bpp },
    { VesaVideoMode::G640x480x8bpp, VideoMode::G640x480x8bpp },
    { VesaVideoMode::G800x600x4bpp, VideoMode::G800x600x4bpp },
    { VesaVideoMode::G800x600x8bpp, VideoMode::G800x600x8bpp },
    { VesaVideoMode::G1024x768x4bpp, VideoMode::G1024x768x4bpp },
    { VesaVideoMode::G1024x768x8bpp, VideoMode::G1024x768x8bpp },
    { VesaVideoMode::G1280x1024x4bpp, VideoMode::G1280x1024x4bpp },
    { VesaVideoMode::G1280x1024x8bpp, VideoMode::G1280x1024x8bpp },
    { VesaVideoMode::T132x25x4bpp, VideoMode::T132x25x4bpp },
    { VesaVideoMode::T132x43x4bpp, VideoMode::T132x43x4bpp },
    { VesaVideoMode::G640x480x15bpp, VideoMode::G640x480x15bpp },
    { VesaVideoMode::G640x480x16bpp, VideoMode::G640x480x16bpp },
    { VesaVideoMode::G640x480x32bpp, VideoMode::G640x480x32bpp },
    { VesaVideoMode::G800x600x15bpp, VideoMode::G800x600x15bpp },
    { VesaVideoMode::G800x600x16bpp, VideoMode::G800x600x16bpp },
    { VesaVideoMode::G800x600x32bpp, VideoMode::G800x600x32bpp },
    { VesaVideoMode::G1024x768x15bpp, VideoMode::G1024x768x15bpp },
    { VesaVideoMode::G1024x768x16bpp, VideoMode::G1024x768x16bpp },
    { VesaVideoMode::G1024x768x32bpp, VideoMode::G1024x768x32bpp },
    { VesaVideoMode::G1280x1024x15bpp, VideoMode::G1280x1024x15bpp },
    { VesaVideoMode::G1280x1024x16bpp, VideoMode::G1280x1024x16bpp },
    { VesaVideoMode::G1600x1200x8bpp, VideoMode::P1600x1200x8bpp },
    { VesaVideoMode::P640x480x8bpp, VideoMode::P640x480x8bpp },
    { VesaVideoMode::P800x600x4bpp, VideoMode::P800x600x4bpp },
    { VesaVideoMode::P800x600x8bpp, VideoMode::P800x600x8bpp },
    { VesaVideoMode::P1024x768x4bpp, VideoMode::P1024x768x4bpp },
    { VesaVideoMode::P1024x768x8bpp, VideoMode::P1024x768x8bpp },
    { VesaVideoMode::P1152x864x8bpp, VideoMode::P1152x864x8bpp },
    { VesaVideoMode::P1280x1024x4bpp, VideoMode::P1280x1024x4bpp },
    { VesaVideoMode::P640x400x32bpp, VideoMode::P640x400x32bpp }
};

int VideoModeTranslationCount = sizeof (VideoModeTranslation) / sizeof(Shared::VESAModeTranslation);

VGA::VideoParameters* LegacyVideoModesV1[] =
{
    &Shared::Data::LegacyMode00v1,
    &Shared::Data::LegacyMode00v1,
    &Shared::Data::LegacyMode02v1,
    &Shared::Data::LegacyMode02v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v1,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0Fv1,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* LegacyVideoModesV2[] =
{
    &Shared::Data::LegacyMode00v2,
    &Shared::Data::LegacyMode00v2,
    &Shared::Data::LegacyMode02v2,
    &Shared::Data::LegacyMode02v2,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v1,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0Fv1,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* LegacyVideoModesV3[] =
{
    &Shared::Data::LegacyMode00v3,
    &Shared::Data::LegacyMode00v3,
    &Shared::Data::LegacyMode02v3,
    &Shared::Data::LegacyMode02v3,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode04v1,
    &Shared::Data::LegacyMode06v1,
    &Shared::Data::LegacyMode07v3,
    &Shared::Data::LegacyMode08v1,
    &Shared::Data::LegacyMode09v1,
    &Shared::Data::LegacyMode0Av1,
    &Shared::Data::LegacyMode0Bv1,
    &Shared::Data::LegacyMode0Cv1,
    &Shared::Data::LegacyMode0Dv1,
    &Shared::Data::LegacyMode0Ev1,
    &Shared::Data::LegacyMode0Fv1,
    &Shared::Data::LegacyMode10v1,
    &Shared::Data::LegacyMode11v1,
    &Shared::Data::LegacyMode12v1,
    &Shared::Data::LegacyMode13v1
};

VGA::VideoParameters* VesaResolutions[] =       //This table is only used to lookup the applicable refresh rate in a rather clumsy manner
{
    &Shared::Data::VESAVideoParameters132x43,
    &Shared::Data::VESAVideoParameters132x25,
    &Shared::Data::VESAVideoParameters640x400,
    &Shared::Data::VESAVideoParameters640x480,
    &Shared::Data::VESAVideoParameters800x600,
    &Shared::Data::VESAVideoParameters1024x768,
    &Shared::Data::VESAVideoParameters1280x1024,
    &Shared::Data::VESAVideoParameters1152x864,
    &Shared::Data::VESAVideoParameters1600x1200
};

Shared::VESAResolutionVariant ModeData132x43x8[] =
{
    {
        Shared::Data::VESAVideoParameters132x43.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x02,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x08,
        0x08,
        0x38,
        0x38,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData132x25x8[] =
{
    {
        Shared::Data::VESAVideoParameters132x25.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x02,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x08,
        0x08,
        0x38,
        0x38,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData640x400x8[] =
{
    {
        Shared::Data::VESAVideoParameters640x400.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width640,  //040h,
        0xF8,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData640x480x8[] =
{
    {
        Shared::Data::CRTData640x480x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0B,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width640,  //040h,
        0x88,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {        
        Shared::Data::CRTData640x480x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x0B,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width640,  //040h,
        0x88,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {        
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width640,  //040h,
        0x88,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x4[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::CRTData800x600x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::CRTData800x600x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x02,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x70,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x06,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x80,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x8[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width800,  //082h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::CRTData800x600x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width800,  //082h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::CRTData800x600x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x02,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width800,  //082h,
        0x70,
        0xF8,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0x00
    },
    {        
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x06,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width800,  //080h,
        0x80,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x4[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0A,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x10,
        0xC0,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x01
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x0E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x10,
        0xC8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x01
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0D,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x30,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {        
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x60,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x8[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0A,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x10,
        0x48,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x01
    },
    {        
        Shared::Data::CRTData1024x768x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x0E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x10,
        0x48,
        0xFF,
        0x48,
        0x00,
        0x00,
        0x01
    },
    {        
        Shared::Data::CRTData1024x768x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0D,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x30,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {        
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x60,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x4[] =
{
    {
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter2 | Shared::VESAFlagsAndFilter::Terminate,
        0x2E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x38,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x8[] =
{
    {
        Shared::Data::CRTData1280x1024x6,
        Shared::VESAFlagsAndFilter::Filter6,
        0x08,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1280,  //0C2h,
        0x18,
        0x18,
        0xFF,
        0xFF,
        0x00,
        0x15,
        0x10
    },
    {        
        Shared::Data::CRTData1280x1024x5,
        Shared::VESAFlagsAndFilter::Filter5,
        0x09,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1280,  //0C0h,
        0x20,
        0x20,
        0xFF,
        0xFF,
        0x00,
        0x55,
        0x10
    },
    {        
        Shared::Data::CRTData1280x1024x4,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0C,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1280,  //0C0h,
        0x28,
        0x28,
        0xFF,
        0xFF,
        0x00,
        0x55,
        0x10
    },
    {        
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter2 | Shared::VESAFlagsAndFilter::Terminate,
        0x2E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1280,  //0C0h,
        0x50,
        0x50,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x01
    }
};

Shared::VESAResolutionVariant ModeData640x480x16[] =
{
    {
        Shared::Data::CRTData640x480x2v2,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0B,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width640,  //050h,
        0x28,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x50
    },
    {        
        Shared::Data::CRTData640x480x1v2,
        Shared::VESAFlagsAndFilter::Filter1,
        0x0B,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width640,  //050h,
        0x30,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x50
    },
    {        
        Shared::Data::CRTData640x480x0,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width640,  //050h,
        0x30,
        0x58,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x50
    }
};

Shared::VESAResolutionVariant ModeData640x480x32[] =
{
    {
        Shared::Data::CRTData640x480x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0B,
        CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width640,  //070h,
        0x30,
        0x30,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0xD0
    },
    {        
        Shared::Data::CRTData640x480x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x0B,
        CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width640,  //070h,
        0x30,
        0x30,
        0x19,
        0x19,
        0x00,
        0x00,
        0xD0
    },
    {
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width640,  //070h,
        0x38,
        0x38,
        0x1C,
        0x1C,
        0x00,
        0x00,
        0xD0
    }
};

Shared::VESAResolutionVariant ModeData800x600x16[] =
{
    {
        Shared::Data::CRTData800x600x3v2,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width800,  //092h,
        0x00,
        0x38,
        0xFF,
        0xFF,
        0x01,
        0x00,
        0x50
    },
    {
        Shared::Data::CRTData800x600x2v2,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width800,  //092h,
        0x00,
        0x38,
        0xFF,
        0xFF,
        0x01,
        0x00,
        0x50
    },
    {
        Shared::Data::CRTData800x600x1v2,
        Shared::VESAFlagsAndFilter::Filter1 | Shared::VESAFlagsAndFilter::Terminate,
        0x02,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width800,  //092h,
        0x18,
        0x48,
        0xFF,
        0xFF,
        0x01,
        0x00,
        0x50
    }
};

Shared::VESAResolutionVariant ModeData800x600x32[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width800,  //0B2h,
        0x00,
        0x00,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0xD0
    },
    {
        Shared::Data::CRTData800x600x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width800,  //0B2h,
        0x00,
        0x00,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0xD0
    },
    {
        Shared::Data::CRTData800x600x1v1,
        Shared::VESAFlagsAndFilter::Filter1 | Shared::VESAFlagsAndFilter::Terminate,
        0x02,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width800,  //0B2h,
        0x10,
        0x10,
        0xFF,
        0xFF,
        0x08,
        0x00,
        0xD0
    }
};

Shared::VESAResolutionVariant ModeData1024x768x16[] =
{
    {
        Shared::Data::CRTData1024x768x4v2,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0A,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width1024,  //012h,
        0x10,
        0x10,
        0xFF,
        0xFF,
        0x5D,
        0x00,
        0x50
    },
    {
        Shared::Data::CRTData1024x768x3v2,
        Shared::VESAFlagsAndFilter::Filter3,
        0x0E,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width1024,  //010h,
        0x10,
        0x10,
        0xFF,
        0xFF,
        0x7D,
        0x00,
        0x50
    },
    {
        Shared::Data::CRTData1024x768x2v2,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0D,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width1024,  //010h,
        0x18,
        0x18,
        0xFF,
        0xFF,
        0x7D,
        0x00,
        0x50
    },
    {
        Shared::Data::CRTData1024x768x0,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width1024,  //010h,
        0x28,
        0x28,
        0xFF,
        0xFF,
        0x75,
        0x00,
        0x50
    }
};

Shared::VESAResolutionVariant ModeData1024x768x32[] =
{
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width1024,  //030h,
        0x38,
        0x38,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0xD0
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x16[] =
{
    {
        Shared::Data::CRTData1280x1024x2,
        Shared::VESAFlagsAndFilter::Filter2 | Shared::VESAFlagsAndFilter::Terminate,
        0x2A,
        CRTController::ExtendedSystemControl1::Length16Bpp | CRTController::ExtendedSystemControl1::Width1280,  //0D0h,
        0x10,
        0x10,
        0xFF,
        0xFF,
        0x7F,
        0x00,
        0x50
    }
};

Shared::VESAResolutionVariant ModeData1600x1200x8[] =
{
    {
        Shared::Data::VESAVideoParameters1600x1200.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x29,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1600,  //081h,
        0x00,
        0x00,
        0xFF,
        0xFF,
        0x01,
        0x00,
        0x10
    }
};

Shared::VESAResolutionVariant ModeData640x480x8xOEM[] =
{
    {
        Shared::Data::CRTData640x480x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0B,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x88,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData640x480x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x0B,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x88,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::VESAVideoParameters640x480.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x88,
        0x88,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x4xOEM[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x02,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x70,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x06,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x80,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData800x600x8xOEM[] =
{
    {
        Shared::Data::CRTData800x600x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x10,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData800x600x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x04,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x48,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData800x600x1v1,
        Shared::VESAFlagsAndFilter::Filter1,
        0x02,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x70,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::VESAVideoParameters800x600.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x06,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x80,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
    };

Shared::VESAResolutionVariant ModeData1024x768x4xOEM[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0A,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x10,
        0xC0,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x0E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x10,
        0xC8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0D,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x30,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x60,
        0xC0,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1024x768x8xOEM[] =
{
    {
        Shared::Data::CRTData1024x768x4v1,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0A,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x10,
        0x48,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData1024x768x3v1,
        Shared::VESAFlagsAndFilter::Filter3,
        0x0E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x10,
        0x48,
        0xFF,
        0x48,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::CRTData1024x768x2v1,
        Shared::VESAFlagsAndFilter::Filter2,
        0x0D,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x30,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    },
    {
        Shared::Data::VESAVideoParameters1024x768.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter0 | Shared::VESAFlagsAndFilter::Terminate,
        0x27,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x60,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1152x864x8xOEM[] =
{
    {
        Shared::Data::VESAVideoParameters1152x864.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter4 | Shared::VESAFlagsAndFilter::Terminate,
        0x0A,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1152,  //001h,
        0x10,
        0x40,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x00
    }
};

Shared::VESAResolutionVariant ModeData1280x1024x4xOEM[] =
{
    {
        Shared::Data::CRTData1280x1024x6,
        Shared::VESAFlagsAndFilter::Filter6 | Shared::VESAFlagsAndFilter::Unknown,
        0x08,
        CRTController::ExtendedSystemControl1::Unknown | CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //002h,
        0x18,
        0x80,
        0xFF,
        0xFF,
        0x00,
        0x15,
        0x10
    },
    {
        Shared::Data::CRTData1280x1024x5,
        Shared::VESAFlagsAndFilter::Filter5 | Shared::VESAFlagsAndFilter::Unknown,
        0x09,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x18,
        0x80,
        0xFF,
        0xFF,
        0x00,
        0x55,
        0x10
    },
    {
        Shared::Data::CRTData1280x1024x4,
        Shared::VESAFlagsAndFilter::Filter4,
        0x0C,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x18,
        0xC0,
        0xFF,
        0xFF,
        0x00,
        0x55,
        0x10
    },
    {
        Shared::Data::VESAVideoParameters1280x1024.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter2 | Shared::VESAFlagsAndFilter::Terminate,
        0x2E,
        CRTController::ExtendedSystemControl1::Length4Or8Bpp | CRTController::ExtendedSystemControl1::Width1024,  //000h,
        0x38,
        0xF8,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0x01
    }
};

Shared::VESAResolutionVariant ModeData640x400x32xOEM[] =
{
    {
        Shared::Data::VESAVideoParameters640x400.CRTCRegisters,
        Shared::VESAFlagsAndFilter::Filter1 | Shared::VESAFlagsAndFilter::Terminate,
        0x00,
        CRTController::ExtendedSystemControl1::Length32Bpp | CRTController::ExtendedSystemControl1::Width640,  //070h,
        0x08,
        0x08,
        0xFF,
        0xFF,
        0x00,
        0x00,
        0xD0
    }
};

Shared::VESAVideoModeData VesaVideoModes[] = 
{
    {
        &Shared::Data::VESAVideoParameters132x43,
        ModeData132x43x8,
        &Shared::Data::ModeInfo_54_T_132x43x4bpp,
        VideoMode::T132x43x4bpp,
        Shared::VESAVideoModeFlags::WindowGranularity64KiB | Shared::VESAVideoModeFlags::Color, //0b00000011,
        AdvancedFunctionControl::ReservedAs1,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &Shared::Data::VESAVideoParameters132x25,
        ModeData132x25x8,
        &Shared::Data::ModeInfo_55_T_132x25x4bpp,
        VideoMode::T132x25x4bpp,
        Shared::VESAVideoModeFlags::WindowGranularity64KiB | Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Unknown3, //0b00110011,
        AdvancedFunctionControl::ReservedAs1,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &Shared::Data::VESAVideoParameters640x400,
        ModeData640x400x8,
        &Shared::Data::ModeInfo_68_G_640x400x8bpp,
        VideoMode::G640x400x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x8,
        &Shared::Data::ModeInfo_69_G_640x480x8bpp,
        VideoMode::G640x480x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x4,
        &Shared::Data::ModeInfo_6A_G_800x600x4bpp,
        VideoMode::G800x600x4bpp,
        Shared::VESAVideoModeFlags::Color, //0b00000010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x8,
        &Shared::Data::ModeInfo_6B_G_800x600x8bpp,
        VideoMode::G800x600x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x4,
        &Shared::Data::ModeInfo_6C_1024x768x4bpp,
        VideoMode::G1024x768x4bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x8,
        &Shared::Data::ModeInfo_6D_1024x768x8bpp,
        VideoMode::G1024x768x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x4,
        &Shared::Data::ModeInfo_6E_1280x1024x4bpp,
        VideoMode::G1280x1024x4bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x8,
        &Shared::Data::ModeInfo_6F_1280x1024x8bpp,
        VideoMode::G1280x1024x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x16,
        &Shared::Data::ModeInfo_70_640x480x15bpp,
        VideoMode::G640x480x15bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x16,
        &Shared::Data::ModeInfo_71_640x480x16bpp,
        VideoMode::G640x480x16bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x32,
        &Shared::Data::ModeInfo_72_640x480x32bpp,
        VideoMode::G640x480x32bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x16,
        &Shared::Data::ModeInfo_73_800x600x15bpp,
        VideoMode::G800x600x15bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x16,
        &Shared::Data::ModeInfo_74_800x600x16bpp,
        VideoMode::G800x600x16bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x32,
        &Shared::Data::ModeInfo_75_800x600x32bpp,
        VideoMode::G800x600x32bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x16,
        &Shared::Data::ModeInfo_76_1024x768x15bpp,
        VideoMode::G1024x768x15bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x16,
        &Shared::Data::ModeInfo_77_1024x768x16bpp,
        VideoMode::G1024x768x16bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x32,
        &Shared::Data::ModeInfo_78_1024x768x32bpp,
        VideoMode::G1024x768x32bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x16,
        &Shared::Data::ModeInfo_79_1280x1024x15bpp,
        VideoMode::G1280x1024x15bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x16,
        &Shared::Data::ModeInfo_7A_1280x1024x16bpp,
        VideoMode::G1280x1024x16bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1600x1200,
        ModeData1600x1200x8,
        &Shared::Data::ModeInfo_7C_1600x1200x8bpp,
        VideoMode::P1600x1200x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x480,
        ModeData640x480x8xOEM,
        &Shared::Data::ModeInfo_49_640x480x8bpp,
        VideoMode::P640x480x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x4xOEM,
        &Shared::Data::ModeInfo_4A_800x600x4bpp,
        VideoMode::P800x600x4bpp,
        Shared::VESAVideoModeFlags::Color, //0b00000010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters800x600,
        ModeData800x600x8xOEM,
        &Shared::Data::ModeInfo_4B_800x600x8bpp,
        VideoMode::P800x600x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256, //0b00000110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x4xOEM,
        &Shared::Data::ModeInfo_4C_1024x768x4bpp,
        VideoMode::P1024x768x4bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1024x768,
        ModeData1024x768x8xOEM,
        &Shared::Data::ModeInfo_4D_1024x768x8bpp,
        VideoMode::P1024x768x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1152x864,
        ModeData1152x864x8xOEM,
        &Shared::Data::ModeInfo_4E_1152x864x8bpp,
        VideoMode::P1152x864x8bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters1280x1024,
        ModeData1280x1024x4xOEM,
        &Shared::Data::ModeInfo_4F_1280x1024x4bpp,
        VideoMode::P1280x1024x4bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &Shared::Data::VESAVideoParameters640x400,
        ModeData640x400x32xOEM,
        &Shared::Data::ModeInfo_52_640x400x32bpp,
        VideoMode::P640x400x32bpp,
        Shared::VESAVideoModeFlags::Color | Shared::VESAVideoModeFlags::Palette256 | Shared::VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        Shared::VESAColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    }
};

int VesaVideoModesCount = sizeof(VesaVideoModes) / sizeof(Shared::VESAVideoModeData);

uint8_t SequenceInitData[] =
{
    0x03,
    0x08, 0x06,
    0x0B, 0x00,
    0x14, 0x00
};

uint8_t PCISystemConfig = 0xD0;
uint8_t VLBSystemConfig = 0xF0;

//;DCLK High/Low values - Trio64 data
uint8_t ClockData[] =
{
    0x28, 0x61, //00h
    0x55, 0x49, //01h
    0x2B, 0x42, //02h
    0x7C, 0x24, //03h
    0x1A, 0x40, //04h
    0x29, 0x22, //05h
    0x63, 0x48, //06h
    0x30, 0x42, //07h
    0x6F, 0x24, //08h
    0x6B, 0x24, //09h
    0x2B, 0x22, //0Ah
    0x33, 0x61, //0Bh
    0x2D, 0x21, //0Ch
    0x47, 0x42, //0Dh
    0x13, 0x20, //0Eh
    0x26, 0x21, //0Fh
    0x51, 0x44  //10h  <- Interlaced
};

//;DCLK High/Low values - Trio64 revision 03h data
uint8_t ClockDataRev3[] =
{
    0x28, 0x61, //00h
    0x55, 0x49, //01h
    0x2B, 0x42, //02h
    0x7C, 0x24, //03h
    0x1A, 0x40, //04h
    0x29, 0x22, //05h
    0x63, 0x48, //06h
    0x30, 0x42, //07h
    0x2D, 0x03, //08h  <- Different from other table
    0x50, 0x07, //09h  <- Different from other table
    0x2B, 0x22, //0Ah
    0x2A, 0x43, //0Bh  <- Different from other table
    0x2D, 0x21, //0Ch
    0x23, 0x22, //0Dh  <- Different from other table
    0x13, 0x20, //0Eh
    0x26, 0x21, //0Fh
    0x51, 0x44  //10h  <- Interlaced
};

Shared::FirmwareFlag_t FirmwareFlags = Shared::FirmwareFlag::Color | Shared::FirmwareFlag::Unknown3;

}
