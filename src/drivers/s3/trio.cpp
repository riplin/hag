//Copyright 2023-Present riplin

//https://projectf.io/posts/video-timings-vga-720p-1080p/

#include <hag/drivers/s3/trio.h>

namespace Hag { namespace S3
{

TrioBase::~TrioBase()
{

}

TrioBase::VideoModeTranslation TrioBase::m_VideoModeTranslation[] =
{
    { VesaVideoMode::G640x400x256C, VideoMode::G640x400x256C },
    { VesaVideoMode::G640x480x256C, VideoMode::G640x480x256C },
    { VesaVideoMode::G800x600x16C, VideoMode::G800x600x16C },
    { VesaVideoMode::G800x600x256C, VideoMode::G800x600x256C },
    { VesaVideoMode::G1024x768x16C, VideoMode::G1024x768x16C },
    { VesaVideoMode::G1024x768x256C, VideoMode::G1024x768x256C },
    { VesaVideoMode::G1280x1024x16C, VideoMode::G1280x1024x16C },
    { VesaVideoMode::G1280x1024x256C, VideoMode::G1280x1024x256C },
    { VesaVideoMode::T132x25x16C, VideoMode::T132x25x16C },
    { VesaVideoMode::T132x43x16C, VideoMode::T132x43x16C },
    { VesaVideoMode::G640x480x32K, VideoMode::G640x480x32K },
    { VesaVideoMode::G640x480x64K, VideoMode::G640x480x64K },
    { VesaVideoMode::G640x480x16M, VideoMode::G640x480x16M },
    { VesaVideoMode::G800x600x32K, VideoMode::G800x600x32K },
    { VesaVideoMode::G800x600x64K, VideoMode::G800x600x64K },
    { VesaVideoMode::G800x600x16M, VideoMode::G800x600x16M },
    { VesaVideoMode::G1024x768x32K, VideoMode::G1024x768x32K },
    { VesaVideoMode::G1024x768x64K, VideoMode::G1024x768x64K },
    { VesaVideoMode::G1024x768x16M, VideoMode::G1024x768x16M },
    { VesaVideoMode::G1280x1024x32K, VideoMode::G1280x1024x32K },
    { VesaVideoMode::G1280x1024x64K, VideoMode::G1280x1024x64K },
    { VesaVideoMode::G1600x1200x256C, VideoMode::P1600x1200x256C },
    { VesaVideoMode::P640x480x256C, VideoMode::P640x480x256C },
    { VesaVideoMode::P800x600x16C, VideoMode::P800x600x16C },
    { VesaVideoMode::P800x600x256C, VideoMode::P800x600x256C },
    { VesaVideoMode::P1024x768x16C, VideoMode::P1024x768x16C },
    { VesaVideoMode::P1024x768x256C, VideoMode::P1024x768x256C },
    { VesaVideoMode::P1152x864x256C, VideoMode::P1152x864x256C },
    { VesaVideoMode::P1280x1024x16C, VideoMode::P1280x1024x16C },
    { VesaVideoMode::P640x400x16M, VideoMode::P640x400x16M }
};

VESAVideoParametersTable VESAVideoParameters132x43 =
{
    0x84,
    0x2A,
    0x08,
    0x4000,
    { 
        0x01,
        0x03,
        0x00,
        0x02
    },
    0x6F,
    //VESAVideoParameters132x43.CRTCRegisters
    {
        0x9A,
        0x83,
        0x84,
        0x9C,
        0x88,
        0x1A,
        0xBF,
        0x1F,
        0x00,
        0x47,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x58,
        0x42,
        0x1F,
        0x63,
        0xBA,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x0C,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x90,
        0x0E,
        0x00,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters132x25 =
{
    0x84,
    0x18,
    0x10,
    0x2000,
    {
        0x01,
        0x03,
        0x00,
        0x02
    },
    0x6F,
    //VESAVideoParameters132x25.CRTCRegisters
    {
        0x9A,
        0x83,
        0x84,
        0x9C,
        0x88,
        0x1A,
        0xBF,
        0x1F,
        0x00,
        0x4F,
        0x0D,
        0x0E,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x42,
        0x1F,
        0x96,
        0xB9,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x0C,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x90,
        0x0E,
        0x00,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters640x400 =
{
    0x50,
    0x18,
    0x10,
    0x1000,
    {
        0x01,
        0x0F,
        0x00,
        0x0E
    },
    0xEF,
    //VESAVideoParameters640x400.CRTCRegisters
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x54,
        0x80,
        0xBF,
        0x1F,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x50,
        0x40,
        0x96,
        0xB9,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x10,
        0x11,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16,
        0x17,
        0x41,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x40,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters640x480 =
{
    0x50,
    0x1D,
    0x10,
    0x1000,
    {
        0x01,
        0x0F,
        0x00,
        0x0E
    },
    0xEF,
    //VESAVideoParameters640x480.CRTCRegisters
    {
        0x5F,
        0x4F,
        0x50,
        0x02,
        0x53,
        0x9F,
        0x0B,
        0x3E,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0xEA,
        0x8C,
        0xDF,
        0x80,
        0x60,
        0xE7,
        0x04,
        0xAB,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x10,
        0x11,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16,
        0x17,
        0x41,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x40,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters800x600 =
{
    0x64,
    0x4A,
    0x08,
    0xFA00,
    {
        0x01,
        0x0F,
        0x00,
        0x0E
    },
    0xEF,
    //VESAVideoParameters800x600.CRTCRegisters
    {
        0x7B,
        0x63,
        0x63,
        0x80,
        0x67,
        0x10,
        0x6F,
        0xF0,
        0x00,
        0x60,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x58,
        0x8A,
        0x57,
        0x80,
        0x00,
        0x57,
        0x00,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters1024x768 =
{
    0x80,
    0x2F,
    0x10,
    0xFFFF,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xEF,
    //VESAVideoParameters1024x768.CRTCRegisters
    {
        0x9A,
        0x7F,
        0x7F,
        0x9D,
        0x81,
        0x17,
        0x97,
        0x1F,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x80,
        0x84,
        0x7F,
        0x80,
        0x00,
        0x80,
        0x00,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters1280x1024 =
{
    0xA0,
    0x3F,
    0x10,
    0x0100,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xEF,
    //VESAVideoParameters1280x1024.CRTCRegisters
    {
        0xC0,
        0x9F,
        0xA0,
        0x83,
        0xA4,
        0x19,
        0x18,
        0xB2,
        0x00,
        0x60,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x01,
        0x85,
        0xFF,
        0x80,
        0x00,
        0x00,
        0x18,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters1152x864 =
{
    0x90,
    0x36,
    0x10,
    0xFFFF,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xEF,
    //VESAVideoParameters1152x864.CRTCRegisters
    {
        0xB1,
        0x8F,
        0x90,
        0x14,
        0x93,
        0x9F,
        0x9A,
        0xFF,
        0x00,
        0x60,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x70,
        0x83,
        0x5F,
        0x90,
        0x60,
        0x6F,
        0x8D,
        0xEB,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x05,
        0x0F,
        0xFF
    }
};

VESAVideoParametersTable VESAVideoParameters1600x1200 =
{
    0xC8,
    0x4B,
    0x10,
    0xFFFF,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0x2F,
    //VESAVideoParameters1600x1200.CRTCRegisters
    {
        0x01,
        0xC7,
        0xC8,
        0x81,
        0xD3,
        0x13,
        0x74,
        0xE0,
        0x00,
        0x60,
        0x00,
        0x00,
        0x00,
        0x00,
        0xFF,
        0x00,
        0x58,
        0x00,
        0x57,
        0x64,
        0x00,
        0x58,
        0x00,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x14,
        0x07,
        0x38,
        0x39,
        0x3A,
        0x3B,
        0x3C,
        0x3D,
        0x3E,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x05,
        0x0F,
        0xFF
    }
};

//Data0BD0
static VGA::CRTControllerData_t CRTData640x480x1v1[] =
{
    0x63,//CR0
    0x4F,//CR1
    0x4F,//CR2
    0x00,//CR3
    0x53,//CR4
    0x18,//CR5
    0x06,//CR6
    0x3E,//CR7
    0x00,//CR8
    0x40,//CR9
    0x00,//CRA
    0x00,//CRB
    0x00,//CRC
    0x00,//CRD
    0xFF,//CRE
    0x00,//CRF
    0xE8,//CR10
    0x8B,//CR11
    0xDF,//CR12
    0x80,//CR13
    0x60,//CR14
    0xDF,//CR15
    0x06,//CR16
    0xAB,//CR17
    0xFF //CR18
};

//Data0BE9
static VGA::CRTControllerData_t CRTData640x480x2v1[] =
{
    0x64,
    0x4F,
    0x4F,
    0x00,
    0x52,
    0x1A,
    0xF2,
    0x1F,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0xE0,
    0x83,
    0xDF,
    0x80,
    0x60,
    0xDF,
    0x00,
    0xAB,
    0xFF
};

//Data0C02
static VGA::CRTControllerData_t CRTData800x600x1v1[] =
{
    0x7F,
    0x63,
    0x63,
    0x80,
    0x69,
    0x19,
    0x72,
    0xF0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x58,
    0x8C,
    0x57,
    0x80,
    0x00,
    0x57,
    0x00,
    0xE3,
    0xFF
};

//Data0C1B
static VGA::CRTControllerData_t CRTData800x600x2v1[] =
{
    0x7D,
    0x63,
    0x63,
    0x80,
    0x6B,
    0x1A,
    0x99,
    0xF0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x7C,
    0xA2,
    0x57,
    0x80,
    0x00,
    0x57,
    0x99,
    0xE3,
    0xFF
};

//Data0C34
static VGA::CRTControllerData_t CRTData800x600x3v1[] =
{
    0x7F,
    0x63,
    0x63,
    0x00,
    0x66,
    0x10,
    0x6F,
    0xE0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x58,
    0x8B,
    0x57,
    0x80,
    0x00,
    0x57,
    0x00,
    0xE3,
    0xFF
};

//Data0C4D
static VGA::CRTControllerData_t CRTData1024x768x2v1[] =
{
    0xA4,
    0x7F,
    0x7F,
    0x07,
    0x83,
    0x95,
    0x25,
    0xF5,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x02,
    0x88,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0x25,
    0xE3,
    0xFF
};

//Data0C66
static VGA::CRTControllerData_t CRTData1024x768x3v1[] =
{
    0xA2,
    0x7F,
    0x7F,
    0x85,
    0x83,
    0x94,
    0x25,
    0xF5,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x02,
    0x88,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0x25,
    0xE3,
    0xFF
};

//Data0C7F
static VGA::CRTControllerData_t CRTData1024x768x4v1[] =
{
    0xA2,
    0x7F,
    0x7F,
    0x83,
    0x82,
    0x8E,
    0x1F,
    0xFD,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x01,
    0x84,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0xEB,
    0xFF
};

//Data0C98
static VGA::CRTControllerData_t CRTData1280x1024x4[] =
{
    0xD7,
    0x9F,
    0xA0,
    0x1A,
    0xA5,
    0x1D,
    0x1E,
    0x52,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x03,
    0x86,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0xE3,
    0xFF
};

//Data0CB1
static VGA::CRTControllerData_t CRTData1280x1024x5[] =
{
    0xCC,
    0x9F,
    0xA0,
    0x8E,
    0xA5,
    0x19,
    0x33,
    0x52,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x05,
    0x8C,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x31,
    0xE3,
    0xFF
};

//Data0CCA
static VGA::CRTControllerData_t CRTData1280x1024x6[] =
{
    0xCE,
    0x9F,
    0xA0,
    0x11,
    0xA3,
    0x15,
    0x29,
    0x52,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x01,
    0x84,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0xE3,
    0xFF
};

//Data0CE3
static VGA::CRTControllerData_t CRTData640x480x0[] =
{
    0xC2,
    0x9F,
    0xA0,
    0x84,
    0xA3,
    0x1B,
    0x0C,
    0x3E,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0xE9,
    0x8B,
    0xDF,
    0x00,
    0x60,
    0xE7,
    0x04,
    0xAB,
    0xFF
};

//Data0CFC
static VGA::CRTControllerData_t CRTData640x480x1v2[] =
{
    0xCC,
    0x9F,
    0xA0,
    0x0F,
    0xA7,
    0x11,
    0x06,
    0x3E,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0xE8,
    0x8B,
    0xDF,
    0x80,
    0x60,
    0xDF,
    0x06,
    0xAB,
    0xFF
};

//Data0D15
static VGA::CRTControllerData_t CRTData640x480x2v2[] =
{
    0xCF,
    0x9F,
    0xA0,
    0x12,
    0xA5,
    0x15,
    0xF1,
    0x1F,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0xE0,
    0x83,
    0xDF,
    0x80,
    0x60,
    0xDF,
    0x00,
    0xAB,
    0xFF
};

//Data0D2E
static VGA::CRTControllerData_t CRTData800x600x1v2[] =
{
    0x03,
    0xC7,
    0xC8,
    0x06,
    0xD3,
    0x13,
    0x72,
    0xF0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x58,
    0x8C,
    0x57,
    0x64,
    0x00,
    0x57,
    0x00,
    0xA3,
    0xFF
};

//Data0D47
static VGA::CRTControllerData_t CRTData800x600x2v2[] =
{
    0x00,
    0xC7,
    0xC8,
    0x03,
    0xD7,
    0x15,
    0x99,
    0xF0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x7C,
    0x82,
    0x57,
    0x64,
    0x00,
    0x57,
    0x99,
    0xA3,
    0xFF
};

//Data0D60
static VGA::CRTControllerData_t CRTData800x600x3v2[] =
{
    0x04,
    0xC7,
    0xC8,
    0x07,
    0xCD,
    0x01,
    0x6F,
    0xF0,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x58,
    0x8B,
    0x57,
    0x80,
    0x00,
    0x57,
    0x00,
    0xE3,
    0xFF
};

//Data0D79
static VGA::CRTControllerData_t CRTData1024x768x0[] =
{
    0x38,
    0xFF,
    0x00,
    0x00,
    0x03,
    0x0F,
    0x97,
    0x1F,
    0x00,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x80,
    0x84,
    0x7F,
    0x00,
    0x00,
    0x80,
    0x00,
    0xA3,
    0xFF
};

//Data0D92
static VGA::CRTControllerData_t CRTData1024x768x2v2[] =
{
    0x4D,
    0xFF,
    0x00,
    0x0D,
    0x07,
    0x09,
    0x25,
    0xF5,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x02,
    0x88,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0x25,
    0xE3,
    0xFF
};

//Data0DAB
static VGA::CRTControllerData_t CRTData1024x768x3v2[] =
{
    0x49,
    0xFF,
    0x00,
    0x09,
    0x07,
    0x09,
    0x1F,
    0xF5,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x02,
    0x88,
    0xFF,
    0x80,
    0x00,
    0xFF,
    0x1F,
    0xE3,
    0xFF
};

//Data0DC4
static VGA::CRTControllerData_t CRTData1024x768x4v2[] =
{
    0x48,
    0xFF,
    0x00,
    0x08,
    0x05,
    0x1D,
    0x1F,
    0xFD,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x01,
    0x84,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0xEB,
    0xFF
};

//Data0DDD
static VGA::CRTControllerData_t CRTData1280x1024x2[] =
{
    0xA1,
    0x3F,
    0x40,
    0x0D,
    0x49,
    0x03,
    0x18,
    0xB2,
    0x00,
    0x60,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x01,
    0x85,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x18,
    0xE3,
    0xFF
};


static VESAResolutionVariant ModeData132x43x8[] =
{
    {
        VESAVideoParameters132x43.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData132x25x8[] =
{
    {
        VESAVideoParameters132x25.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x400x8[] =
{
    {
        VESAVideoParameters640x400.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x480x8[] =
{
    {
        CRTData640x480x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData640x480x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters640x480.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x4[] =
{
    {
        CRTData800x600x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters800x600.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x8[] =
{
    {
        CRTData800x600x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters800x600.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x4[] =
{
    {
        CRTData1024x768x4v1,
        VESAFlagsAndFilter::Filter4,
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
        CRTData1024x768x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData1024x768x2v1,
        VESAFlagsAndFilter::Filter2,
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
        VESAVideoParameters1024x768.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x8[] =
{
    {
        CRTData1024x768x4v1,
        VESAFlagsAndFilter::Filter4,
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
        CRTData1024x768x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData1024x768x2v1,
        VESAFlagsAndFilter::Filter2,
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
        VESAVideoParameters1024x768.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1280x1024x4[] =
{
    {
        VESAVideoParameters1280x1024.CRTCRegisters,
        VESAFlagsAndFilter::Filter2 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1280x1024x8[] =
{
    {
        CRTData1280x1024x6,
        VESAFlagsAndFilter::Filter6,
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
        CRTData1280x1024x5,
        VESAFlagsAndFilter::Filter5,
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
        CRTData1280x1024x4,
        VESAFlagsAndFilter::Filter4,
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
        VESAVideoParameters1280x1024.CRTCRegisters,
        VESAFlagsAndFilter::Filter2 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x480x16[] =
{
    {
        CRTData640x480x2v2,
        VESAFlagsAndFilter::Filter2,
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
        CRTData640x480x1v2,
        VESAFlagsAndFilter::Filter1,
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
        CRTData640x480x0,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x480x32[] =
{
    {
        CRTData640x480x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData640x480x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters640x480.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x16[] =
{
    {
        CRTData800x600x3v2,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v2,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v2,
        VESAFlagsAndFilter::Filter1 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x32[] =
{
    {
        CRTData800x600x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v1,
        VESAFlagsAndFilter::Filter1 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x16[] =
{
    {
        CRTData1024x768x4v2,
        VESAFlagsAndFilter::Filter4,
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
        CRTData1024x768x3v2,
        VESAFlagsAndFilter::Filter3,
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
        CRTData1024x768x2v2,
        VESAFlagsAndFilter::Filter2,
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
        CRTData1024x768x0,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x32[] =
{
    {
        VESAVideoParameters1024x768.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1280x1024x16[] =
{
    {
        CRTData1280x1024x2,
        VESAFlagsAndFilter::Filter2 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1600x1200x8[] =
{
    {
        VESAVideoParameters1600x1200.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x480x8xOEM[] =
{
    {
        CRTData640x480x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData640x480x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters640x480.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x4xOEM[] =
{
    {
        CRTData800x600x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters800x600.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData800x600x8xOEM[] =
{
    {
        CRTData800x600x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData800x600x2v1,
        VESAFlagsAndFilter::Filter2,
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
        CRTData800x600x1v1,
        VESAFlagsAndFilter::Filter1,
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
        VESAVideoParameters800x600.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x4xOEM[] =
{
    {
        CRTData1024x768x4v1,
        VESAFlagsAndFilter::Filter4,
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
        CRTData1024x768x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData1024x768x2v1,
        VESAFlagsAndFilter::Filter2,
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
        VESAVideoParameters1024x768.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1024x768x8xOEM[] =
{
    {
        CRTData1024x768x4v1,
        VESAFlagsAndFilter::Filter4,
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
        CRTData1024x768x3v1,
        VESAFlagsAndFilter::Filter3,
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
        CRTData1024x768x2v1,
        VESAFlagsAndFilter::Filter2,
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
        VESAVideoParameters1024x768.CRTCRegisters,
        VESAFlagsAndFilter::Filter0 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1152x864x8xOEM[] =
{
    {
        VESAVideoParameters1152x864.CRTCRegisters,
        VESAFlagsAndFilter::Filter4 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData1280x1024x4xOEM[] =
{
    {
        CRTData1280x1024x6,
        VESAFlagsAndFilter::Filter6 | VESAFlagsAndFilter::Unknown,
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
        CRTData1280x1024x5,
        VESAFlagsAndFilter::Filter5 | VESAFlagsAndFilter::Unknown,
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
        CRTData1280x1024x4,
        VESAFlagsAndFilter::Filter4,
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
        VESAVideoParameters1280x1024.CRTCRegisters,
        VESAFlagsAndFilter::Filter2 | VESAFlagsAndFilter::Terminate,
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

static VESAResolutionVariant ModeData640x400x32xOEM[] =
{
    {
        VESAVideoParameters640x400.CRTCRegisters,
        VESAFlagsAndFilter::Filter1 | VESAFlagsAndFilter::Terminate,
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


VESAVideoModeData TrioBase::m_VideoModeData[] = 
{
    {
        &VESAVideoParameters132x43,
        ModeData132x43x8,
        VideoMode::T132x43x16C,
        VESAVideoModeFlags::WindowGranularity64KiB | VESAVideoModeFlags::Unknown1, //0b00000011,
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &VESAVideoParameters132x25,
        ModeData132x25x8,
        VideoMode::T132x25x16C,
        VESAVideoModeFlags::WindowGranularity64KiB | VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown3, //0b00110011,
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &VESAVideoParameters640x400, ModeData640x400x8,
        VideoMode::G640x400x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x480,
        ModeData640x480x8,
        VideoMode::G640x480x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x4,
        VideoMode::G800x600x16C,
        VESAVideoModeFlags::Unknown1, //0b00000010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x8,
        VideoMode::G800x600x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x4,
        VideoMode::G1024x768x16C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x8,
        VideoMode::G1024x768x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x4,
        VideoMode::G1280x1024x16C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x8,
        VideoMode::G1280x1024x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x480,
        ModeData640x480x16,
        VideoMode::G640x480x32K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x480,
        ModeData640x480x16,
        VideoMode::G640x480x64K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x480,
        ModeData640x480x32,
        VideoMode::G640x480x16M,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x16,
        VideoMode::G800x600x32K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x16,
        VideoMode::G800x600x64K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x32,
        VideoMode::G800x600x16M,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2, //0b00000110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x16,
        VideoMode::G1024x768x32K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x16,
        VideoMode::G1024x768x64K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x32,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        VideoMode::G1024x768x16M,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x16,
        VideoMode::G1280x1024x32K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C15bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x16,
        VideoMode::G1280x1024x64K,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C16bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1600x1200,
        ModeData1600x1200x8,
        VideoMode::P1600x1200x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x480,
        ModeData640x480x8xOEM,
        VideoMode::P640x480x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x4xOEM,
        VideoMode::P800x600x16C,
        VESAVideoModeFlags::Unknown1, //0b00000010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x8xOEM,
        VideoMode::P800x600x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2, //0b00000110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x4xOEM,
        VideoMode::P1024x768x16C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x8xOEM,
        VideoMode::P1024x768x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1152x864,
        ModeData1152x864x8xOEM,
        VideoMode::P1152x864x256C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x4xOEM,
        VideoMode::P1280x1024x16C,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C8bpp2px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    },
    {
        &VESAVideoParameters640x400,
        ModeData640x400x32xOEM,
        VideoMode::P640x400x16M,
        VESAVideoModeFlags::Unknown1 | VESAVideoModeFlags::Unknown2 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    }
};

FirmwareFlag_t TrioBase::m_FirmwareFlag = FirmwareFlag:: Unknown0 |FirmwareFlag::Unknown3;

VideoMode_t TrioBase::ConvertVesaModeToLegacy(Vesa::VideoMode_t mode) const
{
    for (int i = 0; i < sizeof(m_VideoModeTranslation) / sizeof(VideoModeTranslation); ++i)
    {
        if (m_VideoModeTranslation[i].VesaMode == mode)
            return m_VideoModeTranslation[i].LegacyMode;
    }
    return VideoMode::Invalid;
}

VideoModeError_t TrioBase::CheckValidVideoMode(VideoMode_t mode) const
{
    //Check legacy
    if (mode <= VGA::VideoMode::MaxValid)
        return VideoModeError::Success;

    //Check VESA and OEM
    for (int i = 0; i < sizeof(m_VideoModeTranslation) / sizeof(VideoModeTranslation); ++i)
    {
        if (m_VideoModeTranslation[i].LegacyMode == mode)
            return VideoModeError::Success;
    }
    return VideoModeError::UnknownVideoMode;
}

bool TrioBase::FindVideoModeData(VideoMode_t mode, VESAVideoModeData*& videoModeData) const
{
    bool ret = false;
    for (int i = 0; i < sizeof(m_VideoModeData) / sizeof(VESAVideoModeData); ++i)
    {
        if (m_VideoModeData[i].Mode == mode)
        {
            videoModeData = &m_VideoModeData[i];
            ret = true;
        }
    }

    return ret;
}

bool TrioBase::GetVideoModeFlags(VideoMode_t mode, VESAVideoModeFlags_t& flags) const
{
    bool ret = false;
    flags = 0;
    VESAVideoModeData* videoModeData = NULL;

    if (ret = FindVideoModeData(mode, videoModeData))
    {
        flags = videoModeData->Flags;
    }

    return ret;
}

void TrioBase::SetupBDAColor(System::BDA::EGAFeatureBitSwitches_t featureBitSwitches) const
{
    //Switch us to color.
    System::BDA::VideoModeOptions::Get() &= System::BDA::VideoModeOptions_t(~System::BDA::VideoModeOptions::Monochrome);

    //Throw out the EGA mode.
    System::BDA::EGAFeatureBitSwitches::Get() &= System::BDA::EGAFeatureBitSwitches::FeatureConnectorMask;
    
    //This is confusing. It's using the top bit to increase MDAHiRes80x25_2 to MDAHiResEnhanced_2 if
    //the line mode is not 200.
    //The other mode, CGAMono80x25_2 is uneven so that bit is already set.
    System::BDA::VideoDisplayDataArea_t bumpHiResEnhanced = (~System::BDA::VideoDisplayDataArea::Get()) >> 7;
    System::BDA::EGAFeatureBitSwitches::Get() |= featureBitSwitches | bumpHiResEnhanced;

    //Drop the 200 line
    System::BDA::VideoDisplayDataArea::Get() &= System::BDA::VideoDisplayDataArea_t(~System::BDA::VideoDisplayDataArea::LineMode200);
    if ((m_FirmwareFlag & FirmwareFlag::Unknown0) != 0x00)
    {    
        //Clear out and set us to color.
        System::BDA::DetectedHardware::Get() &= System::BDA::DetectedHardware_t(~System::BDA::DetectedHardware::InitialVideoModeMask);
        System::BDA::DetectedHardware::Get() |= System::BDA::DetectedHardware::Color80x25;
    }
}

void TrioBase::SetBDAVideoMode(VideoMode_t mode) const
{
    System::BDA::EGAFeatureBitSwitches_t adapterType = System::BDA::EGAFeatureBitSwitches::Get() &
                                                       System::BDA::EGAFeatureBitSwitches::AdapterTypeMask;

    System::BDA::DetectedHardware_t detectedHardware = System::BDA::DetectedHardware::Get() & System::BDA::DetectedHardware::InitialVideoModeMask;

    VESAVideoModeFlags_t videoModeFlags = 0;
    bool videoModeFlagsFound = GetVideoModeFlags(mode, videoModeFlags);

    if (!videoModeFlagsFound)
    {
        if (m_FirmwareFlag & FirmwareFlag::Unknown0 == 0x00)
        {
            if (System::BDA::VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexD)
                return;
            
            if (detectedHardware == System::BDA::DetectedHardware::Monochrome80x25)
            {
                //al = 7
                return;
            }

            if (adapterType <= System::BDA::EGAFeatureBitSwitches::CGAMono80x25)
                return;
            
            System::BDA::EGAFeatureBitSwitches_t newSwitches = 0;
            if (adapterType <= System::BDA::EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                newSwitches = System::BDA::EGAFeatureBitSwitches::CGAMono80x25_2 | 
                              System::BDA::EGAFeatureBitSwitches::FeatureConnector1;
            }
            else
            if (adapterType == System::BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
                newSwitches = System::BDA::EGAFeatureBitSwitches::CGAMono80x25_2;
            }
            else
            if (adapterType <= System::BDA::EGAFeatureBitSwitches::CGAMono80x25_2)
            {
                newSwitches = System::BDA::EGAFeatureBitSwitches::MDAHiRes80x25_2 | 
                              System::BDA::EGAFeatureBitSwitches::FeatureConnector0;
            }
            else
                return;
            
            SetupBDAColor(newSwitches);
            return;
        } else
        {
            if (System::BDA::VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexD)
            {
                //Clear out and set us to color.
                System::BDA::DetectedHardware::Get() &= System::BDA::DetectedHardware_t(~System::BDA::DetectedHardware::InitialVideoModeMask);
                System::BDA::DetectedHardware::Get() |= System::BDA::DetectedHardware::Color80x25;
                return;
            }

            if (adapterType <= System::BDA::EGAFeatureBitSwitches::CGAMono80x25)
            {
//    mov  bl, 08h
  //  jmp  Label0x1950                    ;Offset 0x1950
            }
            else if (adapterType <= System::BDA::EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                //...
            }
            else if (adapterType == System::BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
//Label0x1a34:                            ;Offset 0x1a34
//    or   byte ptr ds:[BDA_VideoModeOptions], BDA_VMO_Monochrome;Offset 0x487, 0x2
//    and  byte ptr ds:[BDA_VideoDisplayDataArea], NOT BDA_VDDA_LineMode200;Offset 0x489, 0x7f
//    and  byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_FeatureConnectorMask;Offset 0x488, 0xf0
//    or   byte ptr ds:[BDA_EGAFeatureBitSwitches], BDA_EFBS_CGAMono80x25_2;Offset 0x488, 0xb
//Label0x1a48:                            ;Offset 0x1a48
//    or   word ptr ds:[BDA_DetectedHardware], BDA_DH_80x25Monochrome;Offset 0x410, 0x30
//    ret  
            }
            else if (adapterType > System::BDA::EGAFeatureBitSwitches::CGAMono80x25_2)
            {
                return;
            }
//    cmp  ah, BDA_EFBS_MDAHiRes80x25_2   ;0x8
//    jbe  Label0x19e3                    ;Offset 0x19e3
//    cmp  ah, BDA_EFBS_MDAHiResEnhanced_2;0x9
//    je   Label0x1a34                    ;Offset 0x1a34
//    cmp  ah, BDA_EFBS_CGAMono80x25_2    ;0bh
//    ja   Label0x1a16                    ;Offset 0x1a16

        }
    }
    else
    if (mode == VGA::VideoMode::T80x25x2M ||
        mode == VGA::VideoMode::G640x350x2M ||
        (videoModeFlags & VESAVideoModeFlags::Unknown1) == 0x00)
    {
        //GetVideoModeFlags()
    }

}


}}
