//Copyright 2023-Present riplin

//https://projectf.io/posts/video-timings-vga-720p-1080p/

#include <stdio.h>
#include <string.h>
#include <hag/system/bda.h>
#include <hag/system/pci.h>
#include <hag/system/sysasm.h>
#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/regs.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/crtc/curendsl.h>
#include <hag/drivers/vga/crtc/curstrsl.h>
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/vga/crtc/maxscanl.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/undloc.h>
#include <hag/drivers/vga/crtc/verdisen.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/gfxc/gfxcmode.h>
#include <hag/drivers/vga/gfxc/mmmctrl.h>
#include <hag/drivers/vga/gfxc/rdplnsel.h>
#include <hag/drivers/vga/sqrc/reset.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/memodctl.h>
#include <hag/drivers/s3/advfnctl.h>
#include <hag/drivers/s3/bitplnwm.h>
#include <hag/drivers/s3/curxpos.h>
#include <hag/drivers/s3/curypos.h>
#include <hag/drivers/s3/drawcmd.h>
#include <hag/drivers/s3/fgcolor.h>
#include <hag/drivers/s3/fgmix.h>
#include <hag/drivers/s3/gfxprocs.h>
#include <hag/drivers/s3/majapcnt.h>
#include <hag/drivers/s3/trio.h>
#include <hag/drivers/s3/vidmodes.h>
#include <hag/drivers/s3/wregdata.h>
#include <hag/drivers/s3/crtc/biosflag.h>
#include <hag/drivers/s3/crtc/bkwcomp1.h>
#include <hag/drivers/s3/crtc/bkwcomp3.h>
#include <hag/drivers/s3/crtc/chipidrv.h>
#include <hag/drivers/s3/crtc/conf1.h>
#include <hag/drivers/s3/crtc/devidhi.h>
#include <hag/drivers/s3/crtc/devidlo.h>
#include <hag/drivers/s3/crtc/exbiosf1.h>
#include <hag/drivers/s3/crtc/exbiosf3.h>
#include <hag/drivers/s3/crtc/exbiosf4.h>
#include <hag/drivers/s3/crtc/exhorovf.h>
#include <hag/drivers/s3/crtc/exmemct2.h>
#include <hag/drivers/s3/crtc/exmemct3.h>
#include <hag/drivers/s3/crtc/exmscct2.h>
#include <hag/drivers/s3/crtc/exsysct1.h>
#include <hag/drivers/s3/crtc/exsysct2.h>
#include <hag/drivers/s3/crtc/exsysct3.h>
#include <hag/drivers/s3/crtc/extmode.h>
#include <hag/drivers/s3/crtc/exverovf.h>
#include <hag/drivers/s3/crtc/itlrtst.h>
#include <hag/drivers/s3/crtc/linawctr.h>
#include <hag/drivers/s3/crtc/lnawposh.h>
#include <hag/drivers/s3/crtc/lnawposl.h>
#include <hag/drivers/s3/crtc/memconf.h>
#include <hag/drivers/s3/crtc/misc1.h>
#include <hag/drivers/s3/crtc/modectrl.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/revision.h>
#include <hag/drivers/s3/crtc/stdsfifo.h>
#include <hag/drivers/s3/crtc/sysconf.h>
#include <hag/drivers/s3/sqrc/clksync2.h>
#include <hag/drivers/s3/sqrc/dclkvhi.h>
#include <hag/drivers/s3/sqrc/dclkvlow.h>
#include <hag/drivers/s3/sqrc/extseq9.h>
#include <hag/drivers/s3/sqrc/extseqd.h>
#include <hag/drivers/s3/sqrc/mclkvhi.h>
#include <hag/drivers/s3/sqrc/mclkvlow.h>
#include <hag/drivers/s3/sqrc/rclksync.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>

namespace Hag { namespace S3
{

Trio64::~Trio64()
{

}

Trio64::VideoModeTranslation Trio64::m_VideoModeTranslation[] =
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

VideoParameters VESAVideoParameters132x43 =
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

VideoParameters VESAVideoParameters132x25 =
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

VideoParameters VESAVideoParameters640x400 =
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

VideoParameters VESAVideoParameters640x480 =
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

VideoParameters VESAVideoParameters800x600 =
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

VideoParameters VESAVideoParameters1024x768 =
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

VideoParameters VESAVideoParameters1280x1024 =
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

VideoParameters VESAVideoParameters1152x864 =
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

VideoParameters VESAVideoParameters1600x1200 =
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

VideoParameters Trio64::m_LegacyVideoModes[] =
{
{
    0x28,
    0x18,
    0x08,
    0x0800,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0xA0,
        0xBF,
        0x1F,
        0x00,
        0xC7,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x14,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x08,
    0x0800,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0xA0,
        0xBF,
        0x1F,
        0x00,
        0xC7,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x14,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x08,
    0x1000,
    {
        0x01,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0xC7,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x08,
    0x1000,
    {
        0x01,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0xC7,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x08,
    0x4000,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0x80,
        0xBF,
        0x1F,
        0x00,
        0xC1,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x14,
        0x00,
        0x96,
        0xB9,
        0xA2,
        0xFF
    },
    {
        0x00,
        0x13,
        0x15,
        0x17,
        0x02,
        0x04,
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
        0x01,
        0x00,
        0x03,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x30,
        0x0F,
        0x00,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x08,
    0x4000,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0x63,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0x80,
        0xBF,
        0x1F,
        0x00,
        0xC1,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x14,
        0x00,
        0x96,
        0xB9,
        0xA2,
        0xFF
    },
    {
        0x00,
        0x13,
        0x15,
        0x17,
        0x02,
        0x04,
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
        0x01,
        0x00,
        0x03,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x30,
        0x0F,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x08,
    0x4000,
    {
        0x01,
        0x01,
        0x00,
        0x06
    },
    0x63,
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
        0xC1,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
        0x00,
        0x96,
        0xB9,
        0xC2,
        0xFF
    },
    {
        0x00,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x17,
        0x01,
        0x00,
        0x01,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x0D,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x1000,
    {
        0x00,
        0x03,
        0x00,
        0x03
    },
    0xA6,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x4D,
        0x0B,
        0x0C,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x28,
        0x0D,
        0x63,
        0xBA,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x10,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x0E,
        0x00,
        0x0F,
        0x08
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x0A,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x10,
    0x7D00,
    {
        0x21,
        0x0F,
        0x00,
        0x06
    },
    0x63,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
        0x1F,
        0x96,
        0xB9,
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
},
{
    0x00,
    0x00,
    0x00,
    0x0000,
    {
        0x00,
        0x00,
        0x00,
        0x00
    },
    0x00,
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    }
},
{
    0x28,
    0x18,
    0x08,
    0x4000,
    {
        0x00,
        0x00,
        0x00,
        0x03
    },
    0x23,
    {
        0x37,
        0x27,
        0x2D,
        0x37,
        0x31,
        0x15,
        0x04,
        0x11,
        0x00,
        0x47,
        0x06,
        0x07,
        0x00,
        0x00,
        0x00,
        0x00,
        0xE1,
        0x24,
        0xC7,
        0x14,
        0x08,
        0xE0,
        0xF0,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x00,
    0x00,
    0x0000,
    {
        0x29,
        0x0F,
        0x00,
        0x06
    },
    0x62,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
        0x1F,
        0x96,
        0xB9,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x0F,
        0x00,
        0x00,
        0x08,
        0x05,
        0x0F,
        0xFF
    }
},
{
    0x50,
    0x00,
    0x00,
    0x0000,
    {
        0x29,
        0x0F,
        0x00,
        0x06
    },
    0x63,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
        0x1F,
        0x96,
        0xB9,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x3F,
        0x01,
        0x00,
        0x0F,
        0x00
    },
    {
        0x00,
        0x00,
        0x0F,
        0x00,
        0x00,
        0x08,
        0x05,
        0x0F,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x08,
    0x2000,
    {
        0x09,
        0x0F,
        0x00,
        0x06
    },
    0x63,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0x80,
        0xBF,
        0x1F,
        0x00,
        0xC0,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x14,
        0x00,
        0x96,
        0xB9,
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
},
{
    0x50,
    0x18,
    0x08,
    0x4000,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0x63,
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
        0xC0,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
        0x00,
        0x96,
        0xB9,
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
},
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    {
        0x05,
        0x0F,
        0x00,
        0x00
    },
    0xA2,
    {
        0x60,
        0x4F,
        0x56,
        0x1A,
        0x50,
        0xE0,
        0x70,
        0x1F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x5E,
        0x2E,
        0x5D,
        0x14,
        0x00,
        0x5E,
        0x6E,
        0x8B,
        0xFF
    },
    {
        0x00,
        0x08,
        0x00,
        0x00,
        0x18,
        0x18,
        0x00,
        0x00,
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x18,
        0x00,
        0x00,
        0x0B,
        0x00,
        0x05,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x07,
        0x0F,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    {
        0x05,
        0x0F,
        0x00,
        0x00
    },
    0xA7,
    {
        0x5B,
        0x4F,
        0x53,
        0x17,
        0x50,
        0xBA,
        0x6C,
        0x1F,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x5E,
        0x2B,
        0x5D,
        0x14,
        0x0F,
        0x5F,
        0x0A,
        0x8B,
        0xFF
    },
    {
        0x00,
        0x01,
        0x00,
        0x00,
        0x04,
        0x07,
        0x00,
        0x00,
        0x00,
        0x01,
        0x00,
        0x00,
        0x04,
        0x07,
        0x00,
        0x00,
        0x01,
        0x00,
        0x05,
        0x00
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x07,
        0x0F,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xA2,
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
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x28,
        0x0F,
        0x63,
        0xBA,
        0xE3,
        0xFF
    },
    {
        0x00,
        0x08,
        0x00,
        0x00,
        0x18,
        0x18,
        0x00,
        0x00,
        0x00,
        0x08,
        0x00,
        0x00,
        0x00,
        0x18,
        0x00,
        0x00,
        0x0B,
        0x00,
        0x05,
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
        0x05,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xA3,
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
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x28,
        0x0F,
        0x63,
        0xBA,
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
},
{
    0x28,
    0x18,
    0x0E,
    0x0800,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0xA3,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0xA0,
        0xBF,
        0x1F,
        0x00,
        0x4D,
        0x0B,
        0x0C,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x14,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x0E,
    0x0800,
    {
        0x09,
        0x03,
        0x00,
        0x02
    },
    0xA3,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0xA0,
        0xBF,
        0x1F,
        0x00,
        0x4D,
        0x0B,
        0x0C,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x14,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x1000,
    {
        0x01,
        0x03,
        0x00,
        0x02
    },
    0xA3,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x4D,
        0x0B,
        0x0C,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x28,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x0E,
    0x1000,
    {
        0x01,
        0x03,
        0x00,
        0x02
    },
    0xA3,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
        0xBF,
        0x1F,
        0x00,
        0x4D,
        0x0B,
        0x0C,
        0x00,
        0x00,
        0x00,
        0x00,
        0x83,
        0x85,
        0x5D,
        0x28,
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
        0x08,
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
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x28,
    0x18,
    0x10,
    0x0800,
    {
        0x08,
        0x03,
        0x00,
        0x02
    },
    0x67,
    {
        0x2D,
        0x27,
        0x28,
        0x90,
        0x2B,
        0xA0,
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
        0x14,
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
        0x08
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x10,
    0x1000,
    {
        0x00,
        0x03,
        0x00,
        0x02
    },
    0x67,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
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
        0x28,
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
        0x08
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x0E,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x18,
    0x10,
    0x1000,
    {
        0x00,
        0x03,
        0x00,
        0x02
    },
    0x66,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x55,
        0x81,
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
        0x28,
        0x0F,
        0x96,
        0xB9,
        0xA3,
        0xFF
    },
    {
        0x00,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x10,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x18,
        0x0E,
        0x00,
        0x0F,
        0x08
    },
    {
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x10,
        0x0A,
        0x00,
        0xFF
    }
},
{
    0x50,
    0x1D,
    0x10,
    0xA000,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xE3,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x54,
        0x80,
        0x0B,
        0x3E,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xEA,
        0x8C,
        0xDF,
        0x28,
        0x00,
        0xE7,
        0x04,
        0xC3,
        0xFF
    },
    {
        0x00,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
        0x3F,
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
        0x01,
        0xFF
    }
},
{
    0x50,
    0x1D,
    0x10,
    0xA000,
    {
        0x01,
        0x0F,
        0x00,
        0x06
    },
    0xE3,
    {
        0x5F,
        0x4F,
        0x50,
        0x82,
        0x54,
        0x80,
        0x0B,
        0x3E,
        0x00,
        0x40,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0xEA,
        0x8C,
        0xDF,
        0x28,
        0x00,
        0xE7,
        0x04,
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
},
{
    0x28,
    0x18,
    0x08,
    0x2000,
    {
        0x01,
        0x0F,
        0x00,
        0x0E
    },
    0x63,
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
        0x41,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x9C,
        0x8E,
        0x8F,
        0x28,
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
        0x08,
        0x09,
        0x0A,
        0x0B,
        0x0C,
        0x0D,
        0x0E,
        0x0F,
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
}
};

VideoParameters* Trio64::m_VesaResolutions[] =
{
    &VESAVideoParameters132x43,
    &VESAVideoParameters132x25,
    &VESAVideoParameters640x400,
    &VESAVideoParameters640x480,
    &VESAVideoParameters800x600,
    &VESAVideoParameters1024x768,
    &VESAVideoParameters1280x1024,
    &VESAVideoParameters1152x864,
    &VESAVideoParameters1600x1200
};

uint8_t Trio64::m_VideoModeOverrideTranslationTable1[] =
{ 0x17, 0x17, 0x18, 0x18, 0x04, 0x05, 0x06, 0x19, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x11, 0x12, 0x1A, 0x1B, 0x1C };

uint8_t Trio64::m_VideoModeOverrideTranslationTable2[] =
{ 0x13, 0x14, 0x15, 0x16, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x11, 0x12, 0x1A, 0x1B, 0x1C };

uint8_t Trio64::m_VideoModeOverrideTranslationTable3[] =
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x11, 0x12, 0x1A, 0x1B, 0x1C };

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

VESAModeInfo VESAModeInfo_68_G_640x400x256C = { 0x1B, 0x0280, 0x0280, 0x0190, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_69_G_640x480x256C = { 0x1B, 0x0280, 0x0280, 0x01E0, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_6A_G_800x600x16C = { 0x1F, 0x0064, 0x0320, 0x0258, 0x08, 0x04, 0x04, 0x01, 0x03 };
VESAModeInfo VESAModeInfo_6B_G_800x600x256C = { 0x1B, 0x0320, 0x0320, 0x0258, 0x08, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_6C_1024x768x16C = { 0x1B, 0x0080, 0x0400, 0x0300, 0x10, 0x04, 0x04, 0x01, 0x03 };
VESAModeInfo VESAModeInfo_6D_1024x768x256C = { 0x1B, 0x0400, 0x0400, 0x0300, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_6E_1280x1024x16C = { 0x1B, 0x00A0, 0x0500, 0x0400, 0x10, 0x04, 0x04, 0x01, 0x03 };
VESAModeInfo VESAModeInfo_6F_1280x1024x256C = { 0x1B, 0x0500, 0x0500, 0x0400, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_55_T_132x25 = { 0x0F, 0x0108, 0x0084, 0x0019, 0x10, 0x04, 0x04, 0x01, 0x00 };
VESAModeInfo VESAModeInfo_54_T_132x43 = { 0x0F, 0x0108, 0x0084, 0x002B, 0x08, 0x04, 0x04, 0x01, 0x00 };
VESAModeInfo VESAModeInfo_70_640x480x32K = { 0x1B, 0x0500, 0x0280, 0x01E0, 0x10, 0x01, 0x0F, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_71_640x480x64K = { 0x1B, 0x0500, 0x0280, 0x01E0, 0x10, 0x01, 0x10, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_72_640x480x16M = { 0x1B, 0x0A00, 0x0280, 0x01E0, 0x10, 0x01, 0x20, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_73_800x600x32K = { 0x1B, 0x0640, 0x0320, 0x0258, 0x08, 0x01, 0x0F, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_74_800x600x64K = { 0x1B, 0x0640, 0x0320, 0x0258, 0x08, 0x01, 0x10, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_75_800x600x16M = { 0x1B, 0x0C80, 0x0320, 0x0258, 0x08, 0x01, 0x20, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_76_1024x768x32K = { 0x1B, 0x0800, 0x0400, 0x0300, 0x10, 0x01, 0x0F, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_77_1024x768x64K = { 0x1B, 0x0800, 0x0400, 0x0300, 0x10, 0x01, 0x10, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_78_1024x768x16M = { 0x1B, 0x1000, 0x0400, 0x0300, 0x10, 0x01, 0x20, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_79_1280x1024x32K = { 0x1B, 0x0A00, 0x0500, 0x0400, 0x10, 0x01, 0x0F, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_7A_1280x1024x64K = { 0x1B, 0x0A00, 0x0500, 0x0400, 0x10, 0x01, 0x10, 0x01, 0x06 };
VESAModeInfo VESAModeInfo_7C_1600x1200x256 = { 0x1B, 0x0640, 0x0640, 0x04B0, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_49_640x480x256C = { 0x1B, 0x0400, 0x0280, 0x01E0, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4A_800x600x16C = { 0x1B, 0x0400, 0x0320, 0x0258, 0x08, 0x01, 0x04, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4B_800x600x256C = { 0x1B, 0x0400, 0x0320, 0x0258, 0x08, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4C_1024x768x16C = { 0x1B, 0x0400, 0x0400, 0x0300, 0x10, 0x01, 0x04, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4D_1024x768x256C = { 0x1B, 0x0400, 0x0400, 0x0300, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4E_1152x864x256C = { 0x1B, 0x0480, 0x0480, 0x0360, 0x10, 0x01, 0x08, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_4F_1280x1024x16C = { 0x1B, 0x0400, 0x0500, 0x0400, 0x10, 0x01, 0x04, 0x01, 0x04 };
VESAModeInfo VESAModeInfo_52_640x400x16M = { 0x1B, 0x0A00, 0x0280, 0x0190, 0x10, 0x01, 0x20, 0x01, 0x06 };

VESAVideoModeData Trio64::m_VesaVideoModes[] = 
{
    {
        &VESAVideoParameters132x43,
        ModeData132x43x8,
        &VESAModeInfo_54_T_132x43,
        VideoMode::T132x43x16C,
        VESAVideoModeFlags::WindowGranularity64KiB | VESAVideoModeFlags::Color, //0b00000011,
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &VESAVideoParameters132x25,
        ModeData132x25x8,
        &VESAModeInfo_55_T_132x25,
        VideoMode::T132x25x16C,
        VESAVideoModeFlags::WindowGranularity64KiB | VESAVideoModeFlags::Color | VESAVideoModeFlags::Unknown3, //0b00110011,
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress
    },
    {
        &VESAVideoParameters640x400,
        ModeData640x400x8,
        &VESAModeInfo_68_G_640x400x256C,
        VideoMode::G640x400x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_69_G_640x480x256C,
        VideoMode::G640x480x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_6A_G_800x600x16C,
        VideoMode::G800x600x16C,
        VESAVideoModeFlags::Color, //0b00000010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters800x600,
        ModeData800x600x8,
        &VESAModeInfo_6B_G_800x600x256C,
        VideoMode::G800x600x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256, //0b00000110,
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
        &VESAModeInfo_6C_1024x768x16C,
        VideoMode::G1024x768x16C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters1024x768,
        ModeData1024x768x8,
        &VESAModeInfo_6D_1024x768x256C,
        VideoMode::G1024x768x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_6E_1280x1024x16C,
        VideoMode::G1280x1024x16C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Unknown3, //0b00110010,
        AdvancedFunctionControl::EnhancedModePixelLength |
        AdvancedFunctionControl::ReservedAs1,
        ColorMode::C8bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode
    },
    {
        &VESAVideoParameters1280x1024,
        ModeData1280x1024x8,
        &VESAModeInfo_6F_1280x1024x256C,
        VideoMode::G1280x1024x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_70_640x480x32K,
        VideoMode::G640x480x32K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_71_640x480x64K,
        VideoMode::G640x480x64K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_72_640x480x16M,
        VideoMode::G640x480x16M,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_73_800x600x32K,
        VideoMode::G800x600x32K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256, //0b00000110,
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
        &VESAModeInfo_74_800x600x64K,
        VideoMode::G800x600x64K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256, //0b00000110,
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
        &VESAModeInfo_75_800x600x16M,
        VideoMode::G800x600x16M,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256, //0b00000110,
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
        &VESAModeInfo_76_1024x768x32K,
        VideoMode::G1024x768x32K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_77_1024x768x64K,
        VideoMode::G1024x768x64K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_78_1024x768x16M,
        VideoMode::G1024x768x16M,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_79_1280x1024x32K,
        VideoMode::G1280x1024x32K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_7A_1280x1024x64K,
        VideoMode::G1280x1024x64K,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_7C_1600x1200x256,
        VideoMode::P1600x1200x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_49_640x480x256C,
        VideoMode::P640x480x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_4A_800x600x16C,
        VideoMode::P800x600x16C,
        VESAVideoModeFlags::Color, //0b00000010,
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
        &VESAModeInfo_4B_800x600x256C,
        VideoMode::P800x600x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256, //0b00000110,
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
        &VESAModeInfo_4C_1024x768x16C,
        VideoMode::P1024x768x16C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Unknown3, //0b00110010,
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
        &VESAModeInfo_4D_1024x768x256C,
        VideoMode::P1024x768x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_4E_1152x864x256C,
        VideoMode::P1152x864x256C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
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
        &VESAModeInfo_4F_1280x1024x16C,
        VideoMode::P1280x1024x16C,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Unknown3, //0b00110010,
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
        &VESAModeInfo_52_640x400x16M,
        VideoMode::P640x400x16M,
        VESAVideoModeFlags::Color | VESAVideoModeFlags::Palette256 | VESAVideoModeFlags::Unknown3, //0b00110110,
        AdvancedFunctionControl::ReservedAs1 |
        AdvancedFunctionControl::EnableEnhancedFunctions,
        ColorMode::C24bpp1px,
        VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        VGA::Sequencer::MemoryModeControl::SequentialAddressingMode |
        VGA::Sequencer::MemoryModeControl::SelectChain4Mode
    }
};

uint8_t Trio64::m_CRTControllerInitData[] =
{
    0x1A,
    0x38, 0x48,
    0x39, 0xA5,
    0x31, 0x05,
    0x50, 0x00,
    0x51, 0x00,
    0x53, 0x00,
    0x54, 0x38,
    0x55, 0x00,
    0x58, 0x03,
    0x5D, 0x00,
    0x5E, 0x00,
    0x60, 0x07,
    0x61, 0x80,
    0x62, 0xA1,
    0x67, 0x00,
    0x69, 0x00,
    0x6A, 0x00,
    0x32, 0x40,
    0x33, 0x00,
    0x34, 0x00,
    0x35, 0x00,
    0x3A, 0x05,
    0x3B, 0x5A,
    0x3C, 0x10,
    0x43, 0x00,
    0x45, 0x00
};

uint8_t Trio64::m_SequenceInitData[] =
{
    0x03,
    0x08, 0x06,
    0x0B, 0x00,
    0x14, 0x00
};

uint8_t Trio64::m_PCISystemConfig = 0xD0;
uint8_t Trio64::m_VLBSystemConfig = 0xF0;

//;DCLK High/Low values
uint8_t Trio64::m_ClockData[] =
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

//;DCLK High/Low values - revision 03h data
uint8_t Trio64::m_ClockDataRev3[] =
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

uint8_t Trio64::m_ColorPalette[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x95,
    0x55, 0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55,
    0xFF, 0x15, 0x7F, 0x15, 0xBF
};

uint8_t Trio64::m_MonochromePalette[] =
{ 
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

uint8_t Trio64::m_SecondPalette[] =
{
    0x80, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x98,
    0x9C, 0xA0, 0xA4, 0xA8, 0xAD, 0xB2, 0xB8, 0xBF
};

uint8_t Trio64::m_Color216Palette[] =
{
    0x40, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F,
    0x2F, 0x00, 0x3F, 0xFF, 0x00, 0x3F, 0x00, 0x2F,
    0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x40,
    0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F,
    0x00, 0x7F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F,
    0x00, 0x10, 0x3F, 0x00, 0xC0, 0x3F, 0x00, 0x3F,
    0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00,
    0x7F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00,
    0x10, 0x3F, 0x5F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F,
    0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0xFF, 0x1F, 0x3F,
    0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27,
    0x3F, 0x5F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F,
    0x3F, 0x37, 0x1F, 0x7F, 0x1F, 0x37, 0x3F, 0x1F,
    0x2F, 0x3F, 0x1F, 0x27, 0x3F, 0x1F, 0xDF, 0x3F,
    0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F,
    0x37, 0x1F, 0x7F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F,
    0x3F, 0x1F, 0x27, 0x3F, 0x6D, 0x3F, 0x31, 0x2D,
    0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0xFF,
    0x2D, 0x3F, 0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F,
    0x2D, 0x31, 0x3F, 0x6D, 0x3F, 0x31, 0x2D, 0x3F,
    0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x7F, 0x2D, 0x3A,
    0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D,
    0xED, 0x3F, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36,
    0x2D, 0x3F, 0x3A, 0x2D, 0x7F, 0x2D, 0x3A, 0x3F,
    0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x40, 0x1C,
    0x07, 0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00,
    0x1C, 0xDC, 0x00, 0x1C, 0x00, 0x15, 0x1C, 0x00,
    0x0E, 0x1C, 0x00, 0x07, 0x1C, 0x40, 0x1C, 0x07,
    0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x5C,
    0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07,
    0x1C, 0x00, 0xC0, 0x1C, 0x00, 0x1C, 0x07, 0x00,
    0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x5C, 0x00,
    0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C,
    0x4E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C,
    0x18, 0x0E, 0x1C, 0xDC, 0x0E, 0x1C, 0x0E, 0x18,
    0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x4E,
    0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18,
    0x0E, 0x5C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C,
    0x0E, 0x11, 0x1C, 0x0E, 0xCE, 0x1C, 0x0E, 0x1C,
    0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E,
    0x5C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E,
    0x11, 0x1C, 0x54, 0x1C, 0x16, 0x14, 0x1C, 0x18,
    0x14, 0x1C, 0x1A, 0x14, 0x1C, 0xDC, 0x14, 0x1C,
    0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16,
    0x1C, 0x54, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14,
    0x1C, 0x1A, 0x14, 0x5C, 0x14, 0x1A, 0x1C, 0x14,
    0x18, 0x1C, 0x14, 0x16, 0x1C, 0x14, 0xD4, 0x1C,
    0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 0x1C,
    0x1A, 0x14, 0x5C, 0x14, 0x1A, 0x1C, 0x14, 0x18,
    0x1C, 0x14, 0x16, 0x1C, 0x40, 0x10, 0x04, 0x00,
    0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0xD0,
    0x00, 0x10, 0x00, 0x0C, 0x10, 0x00, 0x08, 0x10,
    0x00, 0x04, 0x10, 0x40, 0x10, 0x04, 0x00, 0x10,
    0x08, 0x00, 0x10, 0x0C, 0x00, 0x50, 0x00, 0x0C,
    0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00,
    0xC0, 0x10, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08,
    0x00, 0x10, 0x0C, 0x00, 0x50, 0x00, 0x0C, 0x10,
    0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x48, 0x10,
    0x0A, 0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08,
    0x10, 0xD0, 0x08, 0x10, 0x08, 0x0E, 0x10, 0x08,
    0x0C, 0x10, 0x08, 0x0A, 0x10, 0x48, 0x10, 0x0A,
    0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x50,
    0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A,
    0x10, 0x08, 0xC8, 0x10, 0x08, 0x10, 0x0A, 0x08,
    0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x50, 0x08,
    0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10,
    0x4B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10,
    0x0F, 0x0B, 0x10, 0xD0, 0x0B, 0x10, 0x0B, 0x0F,
    0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x4B,
    0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F,
    0x0B, 0x50, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10,
    0x0B, 0x0C, 0x10, 0x0B, 0xCB, 0x10, 0x0B, 0x10,
    0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B,
    0x50, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B,
    0x0C, 0x10
};

uint8_t Trio64::m_Monochrome64Palette[] =
{
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF
};

uint8_t Trio64::m_ColorPalette3[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x80,
    0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A, 0x40,
    0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF, 0x95, 0x55, 0x3F, 0xD5,
    0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF, 0x15, 0x7F,
    0x15, 0xBF, 0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00,
    0x6A, 0x2A, 0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00,
    0xAA, 0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A,
    0x2A, 0x40, 0xEA, 0x00, 0x2A, 0x15, 0x00, 0xAA,
    0x95, 0x55, 0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F,
    0x55, 0xFF, 0x15, 0x7F, 0x15, 0xBF, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF
};

uint8_t Trio64::m_MonochromePalette3[] =
{
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0x94, 0xA8,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

uint8_t Trio64::m_ColorPalette2[] =
{
    0x80, 0x40, 0x2A, 0xC0, 0x2A, 0x00, 0x6A, 0x2A,
    0x40, 0xEA, 0x00, 0x6A, 0x00, 0xAA, 0x40, 0x15,
    0x40, 0x3F, 0x00, 0x2A, 0x15, 0x00, 0x2A, 0x3F,
    0x2A, 0x00, 0x15, 0x2A, 0x00, 0x3F, 0x6A, 0x15,
    0x6A, 0x3F, 0xC0, 0x15, 0x00, 0x15, 0x2A, 0xC0,
    0x3F, 0x00, 0x3F, 0x2A, 0x2A, 0x15, 0x00, 0xEA,
    0x15, 0x2A, 0x3F, 0x00, 0xEA, 0x3F, 0x00, 0x55,
    0x00, 0x15, 0x3F, 0x00, 0x3F, 0x15, 0x00, 0x7F,
    0x2A, 0x55, 0x2A, 0x15, 0x3F, 0x2A, 0x3F, 0x15,
    0x2A, 0x7F, 0x15, 0x40, 0x15, 0x00, 0x2A, 0x15,
    0x2A, 0x00, 0x15, 0x6A, 0x3F, 0x40, 0x3F, 0x00,
    0x2A, 0x3F, 0x2A, 0x00, 0x3F, 0x6A, 0xD5, 0x00,
    0x15, 0x00, 0x3F, 0xD5, 0x2A, 0x15, 0x2A, 0x3F,
    0x3F, 0x00, 0x15, 0xFF, 0x00, 0x3F, 0x2A, 0x15,
    0xFF, 0x2A, 0x55, 0x00, 0x55, 0x2A, 0x15, 0x3F,
    0x00, 0x15, 0x3F, 0x2A, 0x3F, 0x15, 0x00, 0x3F,
    0x15, 0x2A, 0x7F, 0x00, 0x7F, 0x2A, 0x95, 0x55,
    0x3F, 0xD5, 0x3F, 0x15, 0x7F, 0x3F, 0x55, 0xFF,
    0x15, 0x7F, 0x15, 0xBF
};

uint8_t Trio64::m_MonochromePalette2[] =
{
    0x80, 0x85, 0x91, 0x9C, 0x88, 0x8B, 0xA5, 0xA8,
    0x82, 0x87, 0x9B, 0xA0, 0x8F, 0x94, 0xA8, 0xAC,
    0x8C, 0x91, 0xA5, 0xAA, 0x94, 0x9E, 0xB2, 0xB6,
    0x8F, 0x93, 0xA7, 0xAC, 0x9B, 0xA0, 0xB4, 0xB9,
    0x86, 0x8B, 0x9F, 0xA4, 0x93, 0x98, 0xAC, 0xB0,
    0x89, 0x8D, 0xA1, 0xA6, 0x95, 0x9A, 0xAE, 0xB3,
    0x93, 0x97, 0xAB, 0xB0, 0x9F, 0xA4, 0xB8, 0xBD,
    0x8E, 0x98, 0xAD, 0xB2, 0xA0, 0xA4, 0xB8, 0xBF
};

FirmwareFlag_t Trio64::m_FirmwareFlag = FirmwareFlag::Color | FirmwareFlag::Unknown3;

VideoMode_t Trio64::ConvertVesaModeToLegacy(Vesa::VideoMode_t mode)
{
    for (int i = 0; i < sizeof(m_VideoModeTranslation) / sizeof(VideoModeTranslation); ++i)
    {
        if (m_VideoModeTranslation[i].VesaMode == mode)
            return m_VideoModeTranslation[i].LegacyMode;
    }
    return VideoMode::Invalid;
}

VideoModeError_t Trio64::CheckValidVideoMode(VideoMode_t mode)
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

VESAVideoModeData* Trio64::FindVideoModeData(VideoMode_t mode)
{
    VESAVideoModeData* videoModeData = NULL;
    for (int i = 0; i < sizeof(m_VesaVideoModes) / sizeof(VESAVideoModeData); ++i)
    {
        if (m_VesaVideoModes[i].Mode == mode)
        {
            videoModeData = &m_VesaVideoModes[i];
            break;
        }
    }
    return videoModeData;
}

bool Trio64::GetVideoModeFlags(VideoMode_t mode, VESAVideoModeFlags_t& flags)
{
    flags = 0;
    VESAVideoModeData* videoModeData = NULL;
    if (videoModeData = FindVideoModeData(mode))
    {
        flags = videoModeData->Flags;
    }

    return videoModeData != NULL;
}

uint16_t Trio64::GetDisplayMemoryInKiB()
{
    static uint16_t memorySizeInKB = 0xFFFF;

    //Early out.
    if (memorySizeInKB != 0xFFFF)
        return memorySizeInKB;

    memorySizeInKB = CRTController::Configuration1::GetDisplayMemorySizeInKiB(CRTControllerIndex());

    return memorySizeInKB;
}

VGA::Register_t Trio64::GetCRTControllerIndexRegister()
{
    return (VGA::MiscellaneousOutput::Read() & 
            VGA::MiscellaneousOutput::IOAddressSelect) == 
            VGA::MiscellaneousOutput::IOAddressSelect ?
            VGA::Register::CRTControllerIndexD :
            VGA::Register::CRTControllerIndexB;
}

void Trio64::ModeSetBDA(VideoMode_t& mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::VGA) != VideoDisplayDataArea::VGA) ||
        (DisplayMode::Get() == mode))
        return;

    DetectedHardware_t initialVideoMode = DetectedHardware::Get() & DetectedHardware::InitialVideoModeMask;
    EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

    uint8_t flags = 0;
    if ((mode == VideoMode::T80x25x2M) || (mode == VideoMode::G640x350x2M) ||
        (GetVideoModeFlags(mode, flags) && ((flags & VESAVideoModeFlags::Color) == 0)))
    {
        if ((m_FirmwareFlag & FirmwareFlag::Color) == 0)
        {
            if ((VideoBaseIOPort::Get() != VGA::Register::CRTControllerIndexB) &&
                (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2))
            {
                if (initialVideoMode != DetectedHardware::Monochrome80x25)
                {
                    mode = VideoMode::T40x25x16G;
                }
                else
                {
                    if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25)
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                    }
                    else if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                        VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                        EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                        Hag::System::BDA::EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                        if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
                        {
                            DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                        }
                    }
                    else 
                    {
                        VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                        VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                        EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                        EGAFeatureBitSwitches::Get() |= adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2 ?
                                                                  EGAFeatureBitSwitches::CGAMono80x25_2 :
                                                                  EGAFeatureBitSwitches::MDAHiRes80x25_2;
                    }
                }
            }
        }
        else
        {
            if (VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexB)
            {
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
            else if ((adapterType <= EGAFeatureBitSwitches::CGAMono80x25) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2))
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
            else
            {
                if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
                {
                    VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                    VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                    EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                    EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                    if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
                    {
                        DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                    }
                }
                else if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2)
                {
                    VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
                    EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                    EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | EGAFeatureBitSwitches::MDAHiRes80x25_2;
                    VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

                    if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
                    {
                        DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                        DetectedHardware::Get() |= DetectedHardware::Color80x25;
                    }
                }
            }
        }
    }
    else if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
    {
        if (VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexD)
        {
            DetectedHardware::Get() &= ~DetectedHardware::InitialVideoModeMask;
            DetectedHardware::Get() |= DetectedHardware::Color80x25;
        }
        else if ((adapterType > EGAFeatureBitSwitches::CGAMono80x25) &&
            (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2))
        {
            if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() |= VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
                {
                    DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
                }
            }
            else if (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
                VideoModeOptions::Get() |= VideoModeOptions::Monochrome;
                VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;
                EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
                EGAFeatureBitSwitches::Get() |= EGAFeatureBitSwitches::CGAMono80x25_2;
                DetectedHardware::Get() |= DetectedHardware::Monochrome80x25;
            }
        }
        else
        {
            VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
            EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
            EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | EGAFeatureBitSwitches::MDAHiRes80x25_2;
            VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

            if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
            {
                DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                DetectedHardware::Get() |= DetectedHardware::Color80x25;
            }
        }
    }
    else if (VideoBaseIOPort::Get() != VGA::Register::CRTControllerIndexD)
    {
        if (initialVideoMode == DetectedHardware::Monochrome80x25)
        {
            mode = VideoMode::T80x25x2M;
        }
        else if (adapterType > EGAFeatureBitSwitches::CGAMono80x25)
        {
            EGAFeatureBitSwitches_t newAdapterType = 0;
            if (adapterType <= EGAFeatureBitSwitches::MDAHiRes80x25_2)
            {
                newAdapterType = EGAFeatureBitSwitches::CGAMono80x25_2 | EGAFeatureBitSwitches::FeatureConnector1;
            }
            else if (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2)
            {
                newAdapterType = EGAFeatureBitSwitches::CGAMono80x25_2;
            }
            else if (adapterType <= EGAFeatureBitSwitches::CGAMono80x25_2)
            {
                newAdapterType = EGAFeatureBitSwitches::MDAHiRes80x25_2 | EGAFeatureBitSwitches::FeatureConnector0;
            }

            VideoModeOptions::Get() &= ~VideoModeOptions::Monochrome;
            EGAFeatureBitSwitches::Get() &= EGAFeatureBitSwitches::FeatureConnectorMask;
            EGAFeatureBitSwitches::Get() |= ((~VideoDisplayDataArea::Get()) >> 7) | newAdapterType;
            VideoDisplayDataArea::Get() &= ~VideoDisplayDataArea::LineMode200;

            if ((m_FirmwareFlag & FirmwareFlag::Color) != 0)
            {
                DetectedHardware::Get() &= DetectedHardware::InitialVideoModeMask;
                DetectedHardware::Get() |= DetectedHardware::Color80x25;
            }
        }
    }
}

void* Trio64::GetLinearFrameBuffer()
{
    void* ret = NULL;
    using namespace Hag::VGA;
    using namespace Hag::System;
    using namespace Hag::System::BDA;
    
    VESAVideoModeData* vesaData = NULL;
    if (DisplayMode::Get() > VideoMode::MaxValid &&
        (vesaData = FindVideoModeData(DisplayMode::Get())))
    {
        Register_t crtcPort = VideoBaseIOPort::Get();
        CRTController::RegisterLock1_t rl1 = CRTController::RegisterLock1::Read(crtcPort);
        CRTController::RegisterLock2_t rl2 = CRTController::RegisterLock2::Read(crtcPort);
        CRTController::RegisterLock1::Unlock(crtcPort);
        CRTController::RegisterLock2::Unlock(crtcPort);

        uint16_t deviceId = (uint16_t(CRTController::DeviceIDHigh::Read(crtcPort)) << 8) |
                                      CRTController::DeviceIDLow::Read(crtcPort);

        uint8_t bus = 0xFF;
        uint8_t slot = 0xFF;
        uint8_t function = 0xFF;
        if (PCI::FindDevice(0x5333, deviceId, bus, slot, function))
        {
            ret = (void*)PCI::Read32(bus, slot, function, PCI::Header0::BaseAddress0);

            CRTController::SystemConfiguration::Unlock(crtcPort);

            CRTController::LinearAddressWindowControl_t linearAddressControl = 
                CRTController::LinearAddressWindowControl::Read(crtcPort);
            CRTController::LinearAddressWindowControl::Write(crtcPort, linearAddressControl |
                                                                 CRTController::LinearAddressWindowControl::EnableLinearAddressing);
        }
        CRTController::RegisterLock2::Lock(crtcPort, rl2);
        CRTController::RegisterLock1::Lock(crtcPort, rl1);
    }
    return ret;
}

bool Trio64::VerifyBDAOrDeactivate(VideoMode_t& mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    uint8_t flags = 0;
    bool ret = false;
    PointHeightOfCharacterMatrix_t newPointHeight = 0;

    if ((DetectedHardware::Get() & DetectedHardware::InitialVideoModeMask) == DetectedHardware::Monochrome80x25)
    {
        if ((VideoModeOptions::Get() & VideoModeOptions::Monochrome) != 0)
        {
            if ((mode != VideoMode::G640x350x2M) &&
                (mode != VideoMode::T80x25x2M) && 
                (!GetVideoModeFlags(mode, flags) ||
                ((flags & VESAVideoModeFlags::Color) == 0)))
            {                
                mode = VideoMode::T80x25x2M;
                VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
            }
            ret = mode != VideoMode::T40x25x16G;
        }
        else
        {
            VideoModeOptions::Get() |= VideoModeOptions::Inactive;
            newPointHeight = 14;
        }
    }
    else
    {
        if ((Hag::System::BDA::VideoModeOptions::Get() & VideoModeOptions::Monochrome) == 0)
        {
            if ((mode == VideoMode::G640x350x2M) ||
                (mode == VideoMode::T80x25x2M) ||
                (GetVideoModeFlags(mode, flags) &&
                (flags & VESAVideoModeFlags::Color) == 0))
            {
                mode = VideoMode::T40x25x16G;
                VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
            }
            ret = mode != VideoMode::T40x25x16G;
        }
        else
        {
            VideoModeOptions::Get() |= mode == VideoMode::T80x25x16C ?
                                               VideoModeOptions::Inactive | VideoModeOptions::Unknown :
                                               VideoModeOptions::Inactive;
            newPointHeight = 8;
        }
    }

    if (newPointHeight != 0)
    {
        RowsOnScreen::Get() = 24;
        PointHeightOfCharacterMatrix::Get() = newPointHeight;
    }
    return ret;
}

VideoParameters* Trio64::GetVideoModeOverrideTable(VideoMode_t mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    VideoParameters* overrideTable = NULL;

    if (mode <= VideoMode::MaxValid)
    {
        uint8_t* translationTable = m_VideoModeOverrideTranslationTable1;
        if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0)
        {
            translationTable = m_VideoModeOverrideTranslationTable2;
            EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() &
                                                  EGAFeatureBitSwitches::AdapterTypeMask;

            if ((adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2) &&
                (adapterType != EGAFeatureBitSwitches::MDAColor80x25_2))
            {
                translationTable = m_VideoModeOverrideTranslationTable3;
            }
        }
        overrideTable = &m_LegacyVideoModes[translationTable[mode]];
    }
    else
    {
        VESAVideoModeData* vesaModeData = NULL;
        if (vesaModeData = FindVideoModeData(mode))
            overrideTable = vesaModeData->OverrideTable;
    }
    return overrideTable;
}

VideoParameters* Trio64::GetCurrentVideoModeOverrideTable()
{
    return GetVideoModeOverrideTable(Hag::System::BDA::DisplayMode::Get());
}

VideoParameters* Trio64::SetTextModeBiosData(uint8_t mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    static uint8_t Mode0_7ControlRegValue[] = { 0x2C, 0x28, 0x2D, 0x29, 0x2A, 0x2E, 0x1E, 0x29 };

    for (int i = 0; i < 8; ++i)
    {
        CursorPositions::Get()[i].Column = 0;
        CursorPositions::Get()[i].Row = 0;
    }

    ActiveDisplayNumber::Get() = 0;
    VideoBufferOffset::Get() = 0;

    if (DisplayMode::Get() <= VideoMode::T80x25x2M)
    {
        CGAColorPaletteMaskSetting::Get() = DisplayMode::Get() == VideoMode::G640x200x2M ?
                                            CGAColorPaletteMaskSetting::Blue |
                                            CGAColorPaletteMaskSetting::Green |
                                            CGAColorPaletteMaskSetting::Red |
                                            CGAColorPaletteMaskSetting::BorderAndBackgroundColorIntensity |
                                            CGAColorPaletteMaskSetting::BackgroundColor |
                                            CGAColorPaletteMaskSetting::ForgroundColorSelect :
                                            CGAColorPaletteMaskSetting::BackgroundColor |
                                            CGAColorPaletteMaskSetting::ForgroundColorSelect;
        CRTModeControlRegValue::Get() = Mode0_7ControlRegValue[DisplayMode::Get()];
    }

    VideoParameters* overrideTable = GetCurrentVideoModeOverrideTable();
    NumberOfScreenColumns::Get() = overrideTable->DisplayedCharacterColumns;
    RowsOnScreen::Get() = overrideTable->DisplayedRowsMinus1;
    PointHeightOfCharacterMatrix::Get() = overrideTable->CharacterMatrixHeightPoints;
    VideoBufferSize::Get() = overrideTable->VideoBufferSize;
    CursorScanLines::Get().End = overrideTable->CRTCRegisters[11];
    CursorScanLines::Get().Start = overrideTable->CRTCRegisters[10];
    return overrideTable;
}

void Trio64::SaveDynamicParameterData(VideoParameters* overrideTable)
{
    using namespace Hag::System::BDA;

    uint8_t* savePointer;
    if (!GetVideoParameterBlockElement(1, savePointer, 0x100))
        return;

    memcpy(savePointer, overrideTable->AttributeControllerRegs, 16);
    savePointer += 16;
    *savePointer = overrideTable->AttributeControllerRegs[17];
}

void Trio64::PrepareAttributeController()
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    InputStatus1::Read(VideoBaseIOPort::Get() + (VGA::Register::InputStatus1D - VGA::Register::CRTControllerIndexD));
    AttributeControllerIndex::Write(AttributeControllerRegister::Palette0);
}

void Trio64::ApplyVideoParameters(VideoParameters* overrideTable)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    PrepareAttributeController();

    SYS_ClearInterrupts();
    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset);
    VGA::Sequencer::ClockingMode::Write(VGA::Sequencer::ClockingMode::ScreenOff);
    VGA::Sequencer::ClockingMode::Write(VGA::Sequencer::ClockingMode::ScreenOff | overrideTable->SequencerRegisters[0]);
    MiscellaneousOutput::Write(overrideTable->MiscOutputRegisterValues);
    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset | VGA::Sequencer::Reset::SynchronousReset);
    SYS_RestoreInterrupts();

    VGA::Sequencer::EnableWritePlane::Write(overrideTable->SequencerRegisters[1]);
    VGA::Sequencer::CharacterFontSelect::Write(overrideTable->SequencerRegisters[2]);
    VGA::Sequencer::MemoryModeControl::Write(overrideTable->SequencerRegisters[3]);

    Register_t crtRegister = VideoBaseIOPort::Get();

    if ((DisplayMode::Get() < VideoMode::Unknown1) ||
        (DisplayMode::Get() > VideoMode::Reserved2))
    {
        crtRegister = (overrideTable->MiscOutputRegisterValues & MiscellaneousOutput::IOAddressSelect) == 0 ? 
                        VGA::Register::CRTControllerIndexB :
                        VGA::Register::CRTControllerIndexD;
    }
    VideoBaseIOPort::Get() = crtRegister;
    VGA::CRTController::VerticalRetraceEnd::Write(crtRegister, 0);

    for (uint8_t crtIndex = 0; crtIndex < 25; ++crtIndex)
    {
        CRTControllerData::Write(crtRegister, crtIndex, overrideTable->CRTCRegisters[crtIndex]);
    }
    
    //Reset Attribute Controller port 0x3c0 to point to index register
    Register_t inputStatus1 = crtRegister + (VGA::Register::InputStatus1D - VGA::Register::CRTControllerIndexD);
    InputStatus1::Read(inputStatus1);

    if ((DisplayMode::Get() == VideoMode::Unknown2) ||
        (DisplayMode::Get() == VideoMode::Reserved1))
    {
        InputStatus1::Read(VGA::Register::InputStatus1B);
    }

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        for (uint8_t attribIndex = 0; attribIndex < 16; ++attribIndex)
        {
            AttributeControllerData::Write(attribIndex, overrideTable->AttributeControllerRegs[attribIndex]);
        }
    }

    uint8_t attribIndex = 16;
    for (uint8_t attribCount = 0; attribCount < 5; ++attribCount)
    {
        if ((attribIndex != 17) || ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0))
        {
            AttributeControllerData_t value = attribIndex == 20 ? 0 :overrideTable->AttributeControllerRegs[16 + attribCount];
            AttributeControllerData::Write(attribIndex, value);
        }
        ++attribIndex;
    }

    for (uint8_t gfxIndex = 0; gfxIndex < 9; ++gfxIndex)
    {
        GraphicsControllerData::Write(gfxIndex, overrideTable->GraphicsControllerRegs[gfxIndex]);
    }
}

uint8_t Trio64::FetchBusSpecificSystemConfig(VGA::Register_t crtcPort)
{
    return ((CRTController::Configuration1::Read(crtcPort) & 
            CRTController::Configuration1::SystemBusSelect) == 
            CRTController::Configuration1::PCIBus) ?
            m_PCISystemConfig :
            m_VLBSystemConfig;
}

void Trio64::InitializeCRTControllerAndSequencer(uint8_t* CRTCInitData, VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;

    for (uint8_t crtcCount = 0; crtcCount < CRTCInitData[0]; ++crtcCount)
    {
        CRTControllerData::Write(crtcPort, CRTCInitData[1 + (crtcCount << 1)], CRTCInitData[2 + (crtcCount << 1)]);
    }

    for (uint8_t sequenceCount = 0; sequenceCount < m_SequenceInitData[0]; ++sequenceCount)
    {
        SequencerIndex_t index = m_SequenceInitData[1 + (sequenceCount << 1)];
        SequencerData_t value = m_SequenceInitData[2 + (sequenceCount << 1)];

        //Don't write the hsync / vsync data.
        if (index == SequencerRegister::ExtendedSequencerD)
            value &= ~Sequencer::ExtendedSequencerD::SyncMask;

        SequencerData::Write(index, value);
    }

    CRTController::SystemConfiguration_t sysConf = CRTController::SystemConfiguration::Read(crtcPort);
    sysConf &= CRTController::SystemConfiguration::UpperBitsMask;
    sysConf |= FetchBusSpecificSystemConfig(crtcPort);
    CRTController::SystemConfiguration::Write(crtcPort, sysConf);

    CRTControllerIndex::Write(crtcPort, CRTControllerRegister::ModeControl);
    CRTController::ModeControl_t modeControl = CRTControllerData::Read(crtcPort + 1) & ~CRTController::ModeControl::Interlaced;
    CRTControllerData::Write(crtcPort + 1, modeControl);
}

void Trio64::WaitGraphicsEngineReady(VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;

    CRTController::SystemConfiguration_t systemConfig = CRTController::SystemConfiguration::Read(crtcPort);
    if (((systemConfig & CRTController::SystemConfiguration::EnableEnhancedRegisterAccess) != 0) &&
        GraphicsProcessorStatus::Read() != GraphicsProcessorStatus::FIFOStatusInvalid)
    {
        GraphicsProcessorStatus_t status = 0;
        do
        {
            status = GraphicsProcessorStatus::Read();
        } while ((status & GraphicsProcessorStatus::GraphicsEngineBusy) != 0);
    }
}

void Trio64::ClearMemory(VGA::Register_t crtcPort)
{
    using namespace Hag::System::BDA;

    if ((VideoModeOptions::Get() & VideoModeOptions::DontClearDisplay) == 0)
    {
        CRTController::RegisterLock1::Unlock(crtcPort);
        CRTController::RegisterLock2::Unlock(crtcPort);

        CRTController::LinearAddressWindowControl_t linearAddressControl = 
            CRTController::LinearAddressWindowControl::Read(crtcPort);
        CRTController::LinearAddressWindowControl::Write(crtcPort, linearAddressControl &
                                                             ~CRTController::LinearAddressWindowControl::EnableLinearAddressing);

        CRTController::ExtendedSystemControl1_t extendedSystemControl = 
            CRTController::ExtendedSystemControl1::Read(crtcPort) & 
            CRTController::ExtendedSystemControl1::LowerMask;
        CRTController::ExtendedSystemControl1::Write(crtcPort, extendedSystemControl |
                                                         CRTController::ExtendedSystemControl1::Length16Bpp);

        CRTController::ExtendedMode_t extendedMode = CRTController::ExtendedMode::Read(crtcPort);
        CRTController::ExtendedMode::Write(crtcPort, extendedMode &
                                               CRTController::ExtendedMode::UnknownMask);

        CRTController::SystemConfiguration_t systemConfiguration = CRTController::SystemConfiguration::Read(crtcPort);
        CRTController::SystemConfiguration::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(AdvancedFunctionControl::EnableEnhancedFunctions |
                                                AdvancedFunctionControl::ReservedAs1 |
                                                AdvancedFunctionControl::EnhancedModePixelLength);

        WriteRegisterData::WriteTopScissors(WriteRegisterData::MinValue);
        WriteRegisterData::WriteLeftScissors(WriteRegisterData::MinValue);
        WriteRegisterData::WriteBottomScissors(WriteRegisterData::MaxValue);
        WriteRegisterData::WriteRightScissors(WriteRegisterData::MaxValue);

        WriteRegisterData::WriteMultifunctionControlMisc(MultifunctionControlMiscellaneous::SelectSlowReadModifyWriteCycle);

        BitplaneWriteMask::Write16x2(BitplaneWriteMask::AllSet);

        ForegroundColor::Write16x2(0);

        ForegroundMix::Write(ForegroundMix::MixNew | ForegroundMix::SelectForegroundColor);

        WriteRegisterData::WritePixelControl(PixelControl::MixForeground);

        CurrentXPosition::Write(0);
        CurrentYPosition::Write(0);

        MajorAxisPixelCount::Write(MajorAxisPixelCount::MaxValue);

        WriteRegisterData::WriteMinorAxisPixelCount(MinorAxisPixelCount::MaxValue);

        DrawingCommand::Write(DrawingCommand::MustBe1 |
                              DrawingCommand::AcrossThePlanePixelMode |
                              DrawingCommand::DrawPixel |
                              DrawingCommand::PosYXmajPosX |
                              DrawingCommand::CommandRectangleFill);

        WaitGraphicsEngineReady(crtcPort);
    }
}

void Trio64::ConfigureDCLKAndMCLK(uint8_t idx, uint8_t* data)
{
    using namespace Hag::VGA;

    Sequencer::UnlockExtendedSequencer::Unlock();
    Sequencer::ClockSynthControl2_t clockSynthControl2;
    if (idx == 0x22)
    {
        SequencerData::Write(SequencerRegister::Unknown1B, 0x00);
        SequencerData::Write(SequencerRegister::Unknown1A, 0x46);

        Sequencer::MClockValueLow::Write(0x46);
        Sequencer::MClockValueHigh::Write(0x79);
        Sequencer::DClockValueLow::Write(0x49);
        Sequencer::DClockValueHigh::Write(0x55);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                             Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }
    else
    {
        Sequencer::DClockValueLow::Write(data[1]);
        Sequencer::DClockValueHigh::Write(data[0]);

        clockSynthControl2 = Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad;
    }

    Sequencer::RAMDACClockSynthControl::Write(0);

    Sequencer::ClockSynthControl2_t originalClockSynthControl2 = Sequencer::ClockSynthControl2::Read();

    originalClockSynthControl2 &= ~(Sequencer::ClockSynthControl2::EnableNewMClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::EnableNewDClockFrequencyLoad |
                                    Sequencer::ClockSynthControl2::DivideDClockByTwo);
    
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2);
    Sequencer::ClockSynthControl2::Write(originalClockSynthControl2 | clockSynthControl2);
}

void Trio64::SetupClocks(VGA::Register_t crtcPort, uint8_t clockConfig)
{
    uint8_t* clockDataPtr = CRTController::Revision::Read(crtcPort) != 0x03 ?
                            m_ClockData :
                            m_ClockDataRev3;

    uint32_t offset = (clockConfig & 0x1f) << 1;
    ConfigureDCLKAndMCLK(clockConfig, clockDataPtr + offset);

    CRTController::ModeControl::Write(crtcPort, clockConfig & CRTController::ModeControl::Interlaced);
}

uint8_t Trio64::GetMemorySizeInMiB(VGA::Register_t crtcPort)
{
    CRTController::RegisterLock1::Unlock(crtcPort);
    CRTController::RegisterLock2::Unlock(crtcPort);

    uint8_t size;
    switch (CRTController::Configuration1::Read(crtcPort) & CRTController::Configuration1::DisplayMemorySize)
    {
    case CRTController::Configuration1::Size05MiB:
        size = 0;      //0.5MiB == 0 for some reason.
        break;
    case CRTController::Configuration1::Size1MiB:
        size = 1;
        break;
    case CRTController::Configuration1::Size2MiB:
        size = 2;
        break;
    case CRTController::Configuration1::Size4MiB:
        size = 4;
        break;
    default:
        size = 1;
        break;
    }
    return size;
}

void Trio64::ConfigureExtraVESAModeSettings(VideoMode_t mode, VGA::Register_t crtcPort, VESAVideoModeData* overrideTable, VESAResolutionVariant* modeData)
{
    using namespace Hag::VGA;

    uint16_t logicalScreenWidth = overrideTable->ModeInfo->BytesPerScanline;

    if (overrideTable->ModeInfo->MemoryModelType == VESAModeMemoryModel::Text)
        logicalScreenWidth <<= 1;
    
    if (overrideTable->ModeInfo->MemoryModelType == VESAModeMemoryModel::EGA16ColorGraphics)
        logicalScreenWidth <<= 2;

    logicalScreenWidth >>= 3;
    uint8_t logicalScreenWidthHigh = uint8_t(logicalScreenWidth >> 8);
    VGA::CRTController::ScreenOffset::Write(crtcPort, uint8_t(logicalScreenWidth));
    CRTController::ExtendedSystemControl2::Write(crtcPort, logicalScreenWidthHigh <<
                                                     CRTController::ExtendedSystemControl2::Shift::LogicalScreenWidthHigh);

    CRTController::Miscellaneous1_t miscellaneous1 = CRTController::Miscellaneous1::RefreshCount1 |
                                                         CRTController::Miscellaneous1::EnableAlternateRefreshCount;

    CRTController::MemoryConfiguration_t memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                                                                   CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth;    

    if (overrideTable->ModeInfo->BitsPerPixel >= 8)
    {
        miscellaneous1 |= CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode;
        memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    CRTController::Miscellaneous1::Write(crtcPort, miscellaneous1);

    if ((mode == VideoMode::P800x600x16C) ||
        (mode == VideoMode::P1024x768x16C) ||
        (mode == VideoMode::P1280x1024x16C))
    {
        memoryConfiguration = CRTController::MemoryConfiguration::EnableBaseAddressOffset |
                              CRTController::MemoryConfiguration::EnableTwoPageScreenImage |
                              CRTController::MemoryConfiguration::EnableVGA16BitMemoryBusWidth |
                              CRTController::MemoryConfiguration::UseEnhancedModeMemoryMapping;
    }

    CRTController::MemoryConfiguration::Write(crtcPort, memoryConfiguration);

    CRTController::ExtendedSystemControl1::Write(crtcPort, modeData->ExtendedSystemControl1);

    if ((modeData->ExtendedSystemControl1 & CRTController::ExtendedSystemControl1::Unknown) != 0)
    {
        MiscellaneousOutput_t miscOutput = MiscellaneousOutput::Read() & 
                                           ~(MiscellaneousOutput::SelectNegativeHorizontalSyncPulse |
                                             MiscellaneousOutput::SelectNegativeVerticalSyncPulse);
        MiscellaneousOutput::Write(miscOutput);
    }

    CRTController::BackwardCompatibility3::Write(crtcPort, 
                                                     CRTController::BackwardCompatibility3::EnableStartDisplayFIFOFetch);

    if (GetMemorySizeInMiB(crtcPort) == 1)
    {
        CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2_1MiB);
        CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3_1MiB);
    }
    else
    {
        CRTController::ExtendedMemoryControl2::Write(crtcPort, modeData->ExtendedMemoryControl2);
        CRTController::ExtendedMemoryControl3::Write(crtcPort, modeData->ExtendedMemoryControl3);
    }

    CRTController::ExtendedHorizontalOverflow::Write(crtcPort, modeData->ExtendedHorizontalOverflow);

    uint16_t horizontalTotal = (uint16_t(modeData->ExtendedHorizontalOverflow &
                                  CRTController::ExtendedHorizontalOverflow::HorizontalTotalHigh) << 8) |
                                  VGA::CRTController::HorizontalTotal::Read(crtcPort);

    CRTController::InterlaceRetraceStart::Write(crtcPort, CRTController::InterlaceRetraceStart_t(horizontalTotal >> 1));
    CRTController::StartDisplayFIFO::Write(crtcPort, horizontalTotal - 7);

    CRTController::SystemConfiguration::Unlock(crtcPort);

    if ((mode != VideoMode::T132x25x16C) &&
        (mode != VideoMode::T132x43x16C) &&
        ((CRTController::Miscellaneous1::Read(crtcPort) & 
        CRTController::Miscellaneous1::Enable8bppOrGreaterColorEnhancedMode) == 0))
    {
        VGA::CRTController::VerticalRetraceEnd_t verticalRetraceEnd = VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //I somehow get the feeling that these are tweaks that could also have been applied to the data.
        VGA::CRTController::StartHorizontalSyncPosition::Write(crtcPort,
                                                          VGA::CRTController::StartHorizontalSyncPosition::Read(crtcPort) + 2);

        VGA::CRTController::EndHorizontalSyncPosition::Write(crtcPort,
                                                        VGA::CRTController::EndHorizontalSyncPosition::Read(crtcPort) + 2);
        if (mode == VideoMode::P1280x1024x16C)
        {
            VGA::CRTController::StartHorizontalBlank::Write(crtcPort,
                                                       VGA::CRTController::StartHorizontalBlank::Read(crtcPort) - 1);
        }

        VGA::CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }

    CRTController::ExtendedVerticalOverflow::Write(crtcPort, modeData->ExtendedVerticalOverflow);

    CRTController::ExtendedMiscellaneousControl2::Write(crtcPort, modeData->ExtendedMiscellaneousControl2);

    if (modeData->ExtendedMiscellaneousControl2 == CRTController::ExtendedMiscellaneousControl2::ColorMode13)
    {
        VGA::CRTController::VerticalRetraceEnd_t verticalRetraceEnd = VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        //Another tweak.
        VGA::CRTController::StartHorizontalBlank::Write(crtcPort,
                                                   VGA::CRTController::StartHorizontalBlank::Read(crtcPort) + 1);

        VGA::CRTController::VerticalRetraceEnd::Lock(crtcPort, verticalRetraceEnd);
    }
}

void Trio64::Configure256KAddressingAndAddressWindow(VideoMode_t mode, VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;

    if (mode > VideoMode::MaxValid)
        CRTController::BackwardCompatibility1::Write(crtcPort,
                                                         CRTController::BackwardCompatibility1::Read(crtcPort) &
                                                         ~CRTController::BackwardCompatibility1::StandardVGAMemoryMapping);

    //TODO: This information should come from the PCI configuration space.
    //Depending on the BIOS flags puts a dependency on the BIOS internal implementation.
    CRTController::ExtendedBIOSFlag3_t biosFlags3 = CRTController::ExtendedBIOSFlag3::Read(crtcPort);
    CRTController::LinearAddressWindowPositionH::Write(crtcPort, biosFlags3);
    CRTController::ExtendedBIOSFlag4_t biosFlags4 = CRTController::ExtendedBIOSFlag4::Read(crtcPort);
    CRTController::LinearAddressWindowPositionL::Write(crtcPort, biosFlags4 & 0x80);
}

void Trio64::SetColorMode(VideoMode_t mode, ColorMode_t colorMode, VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;

    CRTController::ExtendedMiscellaneousControl2_t cMode = 
        CRTController::ExtendedMiscellaneousControl2::ColorMode0;

    if (mode > VideoMode::MaxValid)
    {
        switch (colorMode)
        {
        case ColorMode::C8bpp2px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode8;
            break;
        case ColorMode::C24bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode13;
            break;
        case ColorMode::C24bppPacked:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode12;
            break;
        case ColorMode::C16bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode10;
            break;
        case ColorMode::C15bpp1px:
            cMode = CRTController::ExtendedMiscellaneousControl2::ColorMode9;
            break;
        }
    }

    Sequencer::UnlockExtendedSequencer::Unlock();

    if (cMode == CRTController::ExtendedMiscellaneousControl2::ColorMode8)
    {
        Sequencer::RAMDACClockSynthControl::Write(Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl |
                                                      Sequencer::RAMDACClockSynthControl::EnableClockDoubleMode);

        Sequencer::ClockSynthControl2::Write(Sequencer::ClockSynthControl2::Read() |
                                                 Sequencer::ClockSynthControl2::DivideDClockByTwo);
    }
    else
    {
        Sequencer::RAMDACClockSynthControl::Write(Sequencer::RAMDACClockSynthControl::LUTWriteCycleControl);
        
        Sequencer::ClockSynthControl2::Write(Sequencer::ClockSynthControl2::Read() & 
                                                 ~(Sequencer::ClockSynthControl2::DivideDClockByTwo |
                                                   Sequencer::ClockSynthControl2::InvertDClock));
    }

    CRTController::ExtendedMiscellaneousControl2::Write(crtcPort,
                                                            (CRTController::ExtendedMiscellaneousControl2::Read(crtcPort) &
                                                             ~CRTController::ExtendedMiscellaneousControl2::ColorMode) |
                                                             cMode);
}

void Trio64::ApplyVESAOverrideData(VideoMode_t mode, VGA::Register_t crtcPort, VESAVideoModeData* overrideTable)
{
    using namespace Hag::VGA;

    Sequencer::UnlockExtendedSequencer::Unlock();

    Sequencer::ExtendedSequencer9::Write(Sequencer::ExtendedSequencer9::Read() &
                                             ~Sequencer::ExtendedSequencer9::MemoryMappedIOOnly);

    //This is a read-only register according to the spec...
    CRTController::ChipIDRevision::Write(crtcPort, 0x55);
    if (CRTController::ChipIDRevision::Read(crtcPort) != 0x55)
    {
        CRTController::SystemConfiguration::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(AdvancedFunctionControl::ReservedAs1);

        InitializeCRTControllerAndSequencer(m_CRTControllerInitData, crtcPort);
    }

    if (overrideTable != NULL)
    {
        VESAResolutionVariant* modeData = overrideTable->VariantData;

        ClearMemory(crtcPort);

        //TODO: refresh rate limit should be passed in.
        uint16_t refreshRateCap = (uint16_t(CRTController::BIOSFlag::Read(crtcPort)) << 4) |
                                  (CRTController::ExtendedBIOSFlag1::Read(crtcPort) >> 4);

        for (int i = 0; i < 3; ++i)
        {
            if (overrideTable->OverrideTable == m_VesaResolutions[i + 3])
                break;

            refreshRateCap >>= 3;
        }
        refreshRateCap &= 7;

        VGA::Sequencer::MemoryModeControl::Write(overrideTable->MemoryModeControl);

        CRTController::SystemConfiguration::Unlock(crtcPort);
        VGA::CRTController::VerticalRetraceEnd::Unlock(crtcPort);

        AdvancedFunctionControl::WriteLower(overrideTable->AdvancedFunction);

        while (((modeData->FlagsAndFilter & VESAFlagsAndFilter::Terminate) == 0) && 
               ((modeData->FlagsAndFilter & ~VESAFlagsAndFilter::Terminate) > refreshRateCap))
        {
            ++modeData;
        }

        SetupClocks(crtcPort, modeData->ClockConfigIndex);

        for (CRTControllerIndex_t i = 0; i < 25; ++i)
        {
            CRTControllerData::Write(crtcPort, i, modeData->CRTCRegisters[i]);
        }

        ConfigureExtraVESAModeSettings(mode, crtcPort, overrideTable, modeData);
    }

    Configure256KAddressingAndAddressWindow(mode, crtcPort);

    ColorMode_t colorMode = 0;
    if (overrideTable != NULL)
        colorMode = overrideTable->ColorMode;

    SetColorMode(mode, colorMode, crtcPort);

    Sequencer::UnlockExtendedSequencer::Lock();
}

uint8_t* Trio64::DecompressPaletteColor(uint8_t* paletteData, uint8_t& red, uint8_t& green, uint8_t& blue)
{
    uint8_t redSettings = *paletteData & 0xC0;//Top 2 bits hold compression information
    red = *paletteData & 0x3F;//6 bit color data
    ++paletteData;

    if (redSettings == 0)
    {
        uint8_t greenSettings = *paletteData & 0x40;
        green = *paletteData & 0x3F;//6 bit color data
        ++paletteData;
        if (greenSettings == 0)
        {
            blue = *paletteData;
            ++paletteData;
        }
        else
        {
            blue = green;
        }
    }
    else if (redSettings == 0x40)
    {
        green = red;
        blue = *paletteData;
        ++paletteData;
    }
    else if (redSettings == 0x80)
    {
        green = blue = red;
    }
    else
    {
        blue = red;
        green = *paletteData;
        ++paletteData;
    }
    return paletteData;
}

uint16_t Trio64::SetPaletteColor(uint16_t colorIndex, uint8_t red, uint8_t green, uint8_t blue)
{
    using namespace Hag::VGA;

    SYS_ClearInterrupts();
    DACWriteIndex::Write(colorIndex);
    RAMDACData::Write(red);
    RAMDACData::Write(green);
    RAMDACData::Write(blue);
    SYS_RestoreInterrupts();

    return colorIndex + 1;
}

uint16_t Trio64::ApplyPalette(uint16_t colorIndex, uint16_t count, uint8_t* paletteData)
{
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    for (uint16_t i = 0; i < count; ++i)
    {
        paletteData = DecompressPaletteColor(paletteData, red, green, blue);
        colorIndex = SetPaletteColor(colorIndex, red, green, blue);
    }
    return colorIndex;
}

void Trio64::Set248ColorPalette()
{
    using namespace Hag::System::BDA;

    uint16_t colorIndex = 0;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
 
    colorIndex = ApplyPalette(colorIndex, 16, ((VideoDisplayDataArea::Get() &
                                               (VideoDisplayDataArea::GrayScale |
                                                VideoDisplayDataArea::MonochromeMonitor)) == 0) ?
                                                m_ColorPalette : m_MonochromePalette);

    colorIndex = ApplyPalette(colorIndex, 16, m_SecondPalette);

    if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
    {
        ApplyPalette(colorIndex, 216, m_Color216Palette);
    }
    else
    {
        uint8_t* palettePtr = m_Color216Palette;
        for (int i = 0; i < 216; ++i)
        {
            palettePtr = DecompressPaletteColor(palettePtr, red, green, blue);
            red = green = blue = ((blue * 36043 + green * 19333 + red * 9830 + 16384) >> 15) & 0xFF;
            colorIndex = SetPaletteColor(colorIndex, red, green, blue);
        }
    }
}

void Trio64::SetPalette(VideoMode_t mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        if (DACMask::Read() != 0xFF)
            DACMask::Write(0xFF);

        bool paletteNotSet = false;
        if ((mode == VideoMode::T80x25x2M) ||
            (mode == VideoMode::G640x350x2M))
        {
            ApplyPalette(0, 64, m_Monochrome64Palette);
        }
        else if (mode == VideoMode::G320x200x256C)
        {
            Set248ColorPalette();
        }
        else if (mode > VideoMode::MaxValid)
        {
            VESAVideoModeFlags_t flags = 0;
            GetVideoModeFlags(mode, flags);

            if ((flags & VESAVideoModeFlags::Color) == 0x00)
            {
                ApplyPalette(0, 64, m_Monochrome64Palette);
            }
            else if ((flags & VESAVideoModeFlags::Palette256) != 0x00)
            {
                Set248ColorPalette();
            }
            else paletteNotSet = true;
        }
        else paletteNotSet = true;

        if (paletteNotSet)
        {
            bool paletteSet3 = false;
            if (mode < VideoMode::G320x200x4C)
            {
                EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

                paletteSet3 = (((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) == 0) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced) &&
                               (adapterType != EGAFeatureBitSwitches::MDAHiResEnhanced_2));
            }
            else
            {
                paletteSet3 = ((mode != VideoMode::Unknown1) && (mode <= VideoMode::G640x200x16C));
            }

            if (paletteSet3)
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, m_ColorPalette3);
                }
                else
                {
                    ApplyPalette(0, 64, m_MonochromePalette3);
                }
            }
            else
            {
                if ((VideoDisplayDataArea::Get() & (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0)
                {
                    ApplyPalette(0, 64, m_ColorPalette2);
                }
                else
                {
                    ApplyPalette(0, 64, m_MonochromePalette2);
                }
            }
        }
    }
}

void Trio64::UploadFont(uint8_t* src, uint8_t* dst, uint16_t characters, uint8_t bytesPerCharacter, uint16_t destWidth)
{
    for (uint16_t i = 0; i < characters; ++i)
    {
        memcpy(dst, src, bytesPerCharacter);
        dst += destWidth;
        src += bytesPerCharacter;
    }
}

void Trio64::PatchFont(uint8_t flags)
{
    //0x40 = 8x14, 0x80 = 8x16, 0x00 = no patch.
    if ((flags & 0xC0) != 0x00)
    {
        bool is8x16 = (flags & 0x80) == 0x00;
        uint16_t count = is8x16 ? 16 : 14;
        uint8_t* src = is8x16 ? m_CharacterPatch8x16 : m_CharacterPatch8x14;

        FARPointer dst(0xA000, 0x0000);

        while (true)
        {
            dst.Offset = uint16_t(*src) << 5;
            ++src;

            if (dst.Offset == 0)
                break;

            uint8_t* dstPtr = dst.ToPointer<uint8_t>(count);

            SYS_ClearInterrupts();
            memcpy(dstPtr, src, count);
            SYS_RestoreInterrupts();
            src += count;
        }
    }
}

void Trio64::EnablePaletteBasedVideo()
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    Register_t inputStatus = VideoBaseIOPort::Get() + (VGA::Register::InputStatus1D - VGA::Register::CRTControllerIndexD);
    AttributeControllerIndex::ResetIndex(inputStatus);
    AttributeControllerIndex::Write(AttributeControllerIndex::EnableVideoDisplay);
}

void Trio64::SetTextFontAndAddressing(uint8_t* font, uint16_t startCharacter, uint16_t numCharacters, uint8_t charHeight, uint8_t ramBank)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    static uint16_t RamBankOffset[] =
    {
        0x0000, //(0)
        0x4000, //(1)
        0x8000, //(2)
        0xC000, //(3)
        0x2000, //(4)
        0x6000, //(5)
        0xA000, //(6)
        0xE000  //(7)
    };

    FARPointer dest(0xA000, 0x0000);

    PrepareAttributeController();
    
    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane3);
    VGA::Sequencer::MemoryModeControl::Write(VGA::Sequencer::MemoryModeControl::Unknown |
                                             VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
                                             VGA::Sequencer::MemoryModeControl::SequentialAddressingMode);

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane2);
    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::Mode0);
    GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);

    dest.Offset = RamBankOffset[ramBank & 0x07] + (startCharacter << 5);

    UploadFont(font, dest.ToPointer<uint8_t>(0x2000), numCharacters, charHeight, 32);
    PatchFont(ramBank);

    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane1 |
                                            VGA::Sequencer::EnableWritePlane::Plane2);
    VGA::Sequencer::MemoryModeControl::Write(VGA::Sequencer::MemoryModeControl::Unknown |
                                             VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress);

    GraphicsController::ReadPlaneSelect::Write(GraphicsController::ReadPlaneSelect::Plane1);

    GraphicsController::GraphicsControllerMode::Write(GraphicsController::GraphicsControllerMode::OddEvenAddressing);

    if (VideoBaseIOPort::Get() != VGA::Register::CRTControllerIndexB)
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    }
    else
    {
        GraphicsController::MemoryMapModeControl::Write(GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
                                                        GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
    }
    EnablePaletteBasedVideo();
}

void Trio64::ConfigureCursorPropertiesAndVerticalDisplayEnd(VideoMode_t mode, uint8_t characterPointHeight)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    Register_t crtcPort = VideoBaseIOPort::Get();

    PointHeightOfCharacterMatrix::Get() = characterPointHeight;

    uint16_t screenHeight = 0;

    if ((mode == VideoMode::G640x480x2M) ||
        (mode == VideoMode::G640x480x16C))
    {
        screenHeight = 400;
    }
    else if ((mode == VideoMode::G320x200x256C) ||
        (mode == VideoMode::G640x200x2M) ||
        (mode == VideoMode::G320x200x4G) ||
        ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x200x16C)))
    {
        screenHeight = 200;
    }
    else if ((mode >= VideoMode::Unknown2) &&
        (mode <= VideoMode::G640x350x4C))
    {
        screenHeight = 350;
    }
    else
    {
        EGAFeatureBitSwitches_t adapterType = EGAFeatureBitSwitches::Get() & EGAFeatureBitSwitches::AdapterTypeMask;

        if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::LineMode400) != 0)
        {
            screenHeight = 400;
        }
        else if (((VideoModeOptions::Get() & 0x02) != 0) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced) ||
                (adapterType == EGAFeatureBitSwitches::MDAHiResEnhanced_2) ||
                (mode == VideoMode::T80x25x2M))
        {
            screenHeight = 350;
        }
        else
        {
            screenHeight = 200;
        }
    }

    uint8_t rowsOnScreen = uint8_t(screenHeight / characterPointHeight);
    RowsOnScreen::Get() = rowsOnScreen - 1;

    NumberOfScreenColumns_t bytesPerLine = Hag::System::BDA::NumberOfScreenColumns::Get() << 1;

    VideoBufferSize::Get() = bytesPerLine * rowsOnScreen + 256;

    uint8_t charPointHeightMinusOne = characterPointHeight - 1;

    if (mode == VideoMode::T80x25x2M)
        VGA::CRTController::UnderlineLocation::Write(crtcPort, charPointHeightMinusOne);

    VGA::CRTController::MaximumScanLine::Write(crtcPort, (VGA::CRTController::MaximumScanLine::Read(crtcPort) &
                                                          ~VGA::CRTController::MaximumScanLine::MaximumScanLineCount) |
                                                          charPointHeightMinusOne);

    uint8_t cursorBottom = charPointHeightMinusOne;
    uint8_t cursorTop = charPointHeightMinusOne - 1;

    if (charPointHeightMinusOne > 12)
    {
        --cursorBottom;
        --cursorTop;
    }

    Hag::System::BDA::CursorScanLines::Get().End = cursorBottom;
    Hag::System::BDA::CursorScanLines::Get().Start = cursorTop;

    VGA::CRTController::CursorStartScanLine::Write(crtcPort, cursorTop);
    VGA::CRTController::CursorEndScanLine::Write(crtcPort, cursorBottom);

    uint16_t vertDisplayEnd = (rowsOnScreen - 1) * characterPointHeight;

    if (screenHeight == 200)
        vertDisplayEnd <<= 1;

    --vertDisplayEnd;
    
    VGA::CRTController::VerticalDisplayEnd::Write(crtcPort, uint8_t(vertDisplayEnd));
}

void Trio64::SetFont()
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (PointHeightOfCharacterMatrix::Get() == 14)
    {
        SetTextFontAndAddressing(m_Characters8x14, 0, 256, 14, DisplayMode::Get() == VideoMode::T80x25x2M ? 0x80 : 0);
    }
    else if (PointHeightOfCharacterMatrix::Get() == 8)
    {
        SetTextFontAndAddressing(m_Characters8x8, 0, 256, 8, 0);
    }
    else
    {
        SetTextFontAndAddressing(m_Characters8x16, 0, 256, 16, 0x40);
    }
}

void Trio64::ConfigureFontAndCursor(VideoMode_t mode, System::BDA::AlphanumericCharSet* fontDefinition)
{
    using namespace Hag::System::BDA;
    
    uint8_t* font = fontDefinition->FontData.ToPointer<uint8_t>(fontDefinition->CharacterHeight * fontDefinition->NumCharacters);

    SetTextFontAndAddressing(font,
                             fontDefinition->FirstCharacter,
                             fontDefinition->NumCharacters,
                             fontDefinition->CharacterHeight,
                             fontDefinition->RamBank & 0x3F);
    ConfigureCursorPropertiesAndVerticalDisplayEnd(mode, fontDefinition->CharacterHeight);

    if (fontDefinition->Rows != 0xFF)
    {
        RowsOnScreen::Get() = fontDefinition->Rows - 1;
    }
}

void Trio64::ConfigureFontRamBank(System::BDA::SecondaryAlphaModeAuxillaryCharacterGeneratorTable* fontDefinition)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if (fontDefinition->CharacterHeight != PointHeightOfCharacterMatrix::Get())
    {
        uint8_t* font = fontDefinition->FontData.ToPointer<uint8_t>(fontDefinition->CharacterHeight * 256);
        SetTextFontAndAddressing(font, 0, 256, fontDefinition->CharacterHeight, fontDefinition->RamBank & 0x3f);

        //This is the weirdest piece of code so far.
        VGA::Sequencer::CharacterFontSelect::Write((VGA::Sequencer::MemoryModeControl::Read() &
                                                   (VGA::Sequencer::MemoryModeControl::Unknown |
                                                    VGA::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
                                                    VGA::Sequencer::MemoryModeControl::Unknown2)) |
                                                    ((fontDefinition->RamBank & 0x03) << 2) |
                                                    ((fontDefinition->RamBank & 0x04) << 3));
    }
}

void Trio64::ClearScreen(VideoMode_t mode)
{
    using namespace Hag::VGA;

    uint32_t size =  0x2000;
    uint16_t segment = 0;
    uint32_t value = 0;

    uint8_t flags = 0;
    if ((mode > VideoMode::MaxValid) &&
        GetVideoModeFlags(mode, flags))
    {
        segment = 0xB800;
        value = 0x07200720;

        if ((flags & VESAVideoModeFlags::Color) == 0x00)
        {
            segment = 0xB000;
        }
        if ((flags & VESAVideoModeFlags::WindowGranularity64KiB) == 0x00)
            return;
    }
    else
    {
        if (mode == VideoMode::T80x25x2M)
        {
            segment = 0xB000;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::T80x25x16C)
        {
            segment = 0xB800;
            value = 0x07200720;
        }
        else if (mode <= VideoMode::G640x200x2M)
        {
            segment = 0xB800;
        }
        else
        {
            segment = 0xA000;
            size = 0x4000;
        }
    }

    uint32_t* ptr = FARPointer(segment, 0x0000).ToPointer<uint32_t>(size << 2);
    
    do
    {
        *ptr = value;
        ++ptr;
        --size;
    } while (size != 0);
}

void Trio64::SetPaletteProfile(Hag::VGA::Register_t crtcPort)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    SecondarySavePointerTable* secondarySavePointerTable = NULL;
    if (GetVideoParameterBlockElementAs<SecondarySavePointerTable>(4, secondarySavePointerTable, 32) &&
        !secondarySavePointerTable->UserPaletteProfileTable.IsNull())
    {
        PaletteProfile* paletteProfile = secondarySavePointerTable->UserPaletteProfileTable.ToPointer<PaletteProfile>(0x14 + 0x14);
        if (CheckValidInCurrentMode(paletteProfile->ApplicableModes))
        {
            if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) != 0)
            {
                InputStatus1::Read(crtcPort + (VGA::Register::InputStatus1D - VGA::Register::CRTControllerIndexD));

                uint16_t colorCount = (paletteProfile->DACRegisterCount << 1) +
                                       paletteProfile->DACRegisterCount;

                uint8_t* paletteColors = paletteProfile->DACRegisterTable.ToPointer<uint8_t>(colorCount);
                DACWriteIndex::Write(uint8_t(paletteProfile->DACRegisterStartIndex));
                while (colorCount != 0)
                {
                    RAMDACData::Write(*paletteColors);
                    ++paletteColors;
                    --colorCount;
                }

                if (paletteProfile->AttributeRegisterCount != 0)
                {
                    uint8_t* attributeRegisters = paletteProfile->AttributeRegisterTable.ToPointer<uint8_t>(paletteProfile->AttributeRegisterCount);
                    
                    for (uint16_t attrIdx = 0; attrIdx < paletteProfile->AttributeRegisterCount; ++attrIdx)
                    {
                        AttributeControllerData::Write(paletteProfile->AttributeRegisterStartIndex + attrIdx,
                                                    *attributeRegisters);
                        ++attributeRegisters;
                    }
                    AttributeControllerData::Write(paletteProfile->AttributeRegisterCount + 1, *attributeRegisters);
                }
            }

            if (paletteProfile->Underlining != 0)
            {
                VGA::CRTController::StartHorizontalSyncPosition_t horizontalSyncPos = 31;
                if (paletteProfile->Underlining > 0)
                    horizontalSyncPos = VGA::CRTController::StartHorizontalSyncPosition_t(PointHeightOfCharacterMatrix::Get() - 1);

                VGA::CRTController::StartHorizontalSyncPosition::Write(crtcPort, horizontalSyncPos);
            }
        }
    }
}

VideoModeError_t Trio64::SetLegacyVideoModeInternal(VideoMode_t mode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    VGA::Register_t crtcPort = GetCRTControllerIndexRegister();
    
    CRTController::RegisterLock1::Unlock(crtcPort);
    CRTController::RegisterLock2::Unlock(crtcPort);

    VideoMode_t dontClearDisplay = mode & VideoMode::DontClearDisplay;

    mode &= ~VideoMode::DontClearDisplay;

    VESAVideoModeData* vesaData = NULL;

    if ((mode <= VideoMode::MaxValid) ||
        (vesaData = FindVideoModeData(mode)))
    {
        VideoModeOptions::Get() &= ~VideoModeOptions::DontClearDisplay;
        VideoModeOptions::Get() |= dontClearDisplay;

        ModeSetBDA(mode);

        if ((mode <= VideoMode::MaxValid) &&
            !VerifyBDAOrDeactivate(mode))
        {
            return VideoModeError::AdapterNotActive;
        }

        DisplayMode::Get() = mode;

        VideoParameters* overrideTable = SetTextModeBiosData(mode);

        VideoModeOptions::Get() &= ~(VideoModeOptions::Unknown |
                                     VideoModeOptions::Inactive);

        SaveDynamicParameterData(overrideTable);
        ApplyVideoParameters(overrideTable);
        ApplyVESAOverrideData(mode, crtcPort, vesaData);
        SetPalette(mode);

        VESAVideoModeFlags_t flags = 0;

        if ((mode < VideoMode::G320x200x4C) ||
            (mode == VideoMode::T80x25x2M) ||
            (GetVideoModeFlags(mode, flags) &&
            ((flags & VESAVideoModeFlags::WindowGranularity64KiB) != 0)))
        {
            SetFont();

            AlphanumericCharSet* fontDefinition = NULL;
            if (GetVideoParameterBlockElementAs<AlphanumericCharSet>(2, fontDefinition, 0x0B + 0x14) &&
                CheckValidInCurrentMode(fontDefinition->ApplicableModes))
                ConfigureFontAndCursor(mode, fontDefinition);

            SecondarySavePointerTable* paramBlock = NULL;
            if (GetVideoParameterBlockElementAs<SecondarySavePointerTable>(4, paramBlock, 0x20) &&
                !paramBlock->SecondaryAlphanumericCharacterSetOverride.IsNull())
            {
                SecondaryAlphaModeAuxillaryCharacterGeneratorTable* graphicsFont =
                    paramBlock->SecondaryAlphanumericCharacterSetOverride.
                    ToPointer<SecondaryAlphaModeAuxillaryCharacterGeneratorTable>(0x0B + 0x14);

                if (CheckValidInCurrentMode(graphicsFont->ApplicableModes))
                    ConfigureFontRamBank(graphicsFont);
            }
        }
        else
        {
            CursorScanLines::Get().End = 0;
            CursorScanLines::Get().Start = 0;

            GraphicsCharacterSetOverride* graphicsCharacterFontDefinition = NULL;
            if (GetVideoParameterBlockElementAs<GraphicsCharacterSetOverride>(3, graphicsCharacterFontDefinition, 0x07 + 0x14) &&
                CheckValidInCurrentMode(graphicsCharacterFontDefinition->ApplicableVideoModes))
                SetGraphicsCharacterFont(graphicsCharacterFontDefinition);
        }

        if (((VideoModeOptions::Get() & VideoModeOptions::DontClearDisplay) == 0x00) &&
            (VideoBufferSize::Get() != 0))
            ClearScreen(mode);

        SetPaletteProfile(crtcPort);
        EnablePaletteBasedVideo();
        VGA::Sequencer::ClockingMode::TurnScreenOn();

        return VideoModeError::Success;
    }
    return VideoModeError::UnknownVideoMode;
}

VideoModeError_t Trio64::SetVesaVideoModeInternal(Vesa::VideoMode_t mode)
{
    VideoMode_t legacyMode = VideoMode::Invalid;
    if ((mode & Vesa::VideoMode::LegacyMask) == 0)
    {
        legacyMode = VideoMode_t(mode);//Just drop the top byte
    }
    else
    {
        Vesa::VideoMode_t actualMode = mode & Vesa::VideoMode_t(~Vesa::VideoMode::DontClearDisplay);
        legacyMode = ConvertVesaModeToLegacy(actualMode);
    }

    if (legacyMode == VideoMode::Invalid)
        return VideoModeError::UnknownVideoMode;
    
    if ((mode & Vesa::VideoMode::DontClearDisplay) == Vesa::VideoMode::DontClearDisplay)
        legacyMode |= VideoMode::DontClearDisplay;
    
    return SetLegacyVideoModeInternal(legacyMode);
}

}}
