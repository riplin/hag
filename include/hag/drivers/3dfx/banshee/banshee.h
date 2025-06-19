//Copyright 2025-Present riplin

//3Dfx Banshee Specification: https://theretroweb.com/chip/documentation/banshee-spec-662aff3a8597f179587013.pdf
//Additional documentation: https://theretroweb.com/chip/documentation/banshee-2d-spec-662aff3a84890000637393.pdf

#pragma once

#include <hag/drivers/imports.h>

#include <hag/drivers/3dfx/shared/regtype.h>

#include <hag/drivers/vga/featctl.h>                        //0x3BA, 0x3CA, 0x3DA
#include <hag/drivers/vga/instat1.h>                        //0x3BA, 0x3DA

#include <hag/drivers/vga/crtc/index.h>                     //CRTC Index 0x3B4, 0x3D4
#include <hag/drivers/vga/crtc/data.h>                      //CRTC Data  0x3B5, 0x3D5
#include <hag/drivers/vga/crtc/hortotal.h>                  //CR00
#include <hag/drivers/vga/crtc/hordisen.h>                  //CR01
#include <hag/drivers/vga/crtc/sthorbln.h>                  //CR02
#include <hag/drivers/vga/crtc/enhorbln.h>                  //CR03
#include <hag/drivers/vga/crtc/sthorsyn.h>                  //CR04
#include <hag/drivers/vga/crtc/enhorsyn.h>                  //CR05
#include <hag/drivers/vga/crtc/vertotal.h>                  //CR06
#include <hag/drivers/vga/crtc/crtcovfl.h>                  //CR07
#include <hag/drivers/vga/crtc/prstrwsc.h>                  //CR08
#include <hag/drivers/vga/crtc/maxscanl.h>                  //CR09
#include <hag/drivers/vga/crtc/curstrsl.h>                  //CR0A
#include <hag/drivers/vga/crtc/curendsl.h>                  //CR0B
#include <hag/drivers/vga/crtc/staddrhi.h>                  //CR0C
#include <hag/drivers/vga/crtc/staddrlo.h>                  //CR0D
#include <hag/drivers/vga/crtc/curladhi.h>                  //CR0E
#include <hag/drivers/vga/crtc/curladlo.h>                  //CR0F
#include <hag/drivers/vga/crtc/verrtcst.h>                  //CR10
#include <hag/drivers/vga/crtc/verrtcen.h>                  //CR11
#include <hag/drivers/vga/crtc/verdisen.h>                  //CR12
#include <hag/drivers/vga/crtc/scrnoffs.h>                  //CR13
#include <hag/drivers/vga/crtc/undloc.h>                    //CR14
#include <hag/drivers/vga/crtc/stverbln.h>                  //CR15
#include <hag/drivers/vga/crtc/enverbln.h>                  //CR16
#include <hag/drivers/vga/crtc/modectrl.h>                  //CR17
#include <hag/drivers/vga/crtc/linecomp.h>                  //CR18

#include <hag/drivers/vga/attribc/index.h>                  //ATTR Index 0x3C0
#include <hag/drivers/vga/attribc/data.h>                   //ATTR Data  0x3C0, 0x3C1
#include <hag/drivers/vga/attribc/palette.h>                //AR00-0F
#include <hag/drivers/vga/attribc/attrmode.h>               //AR10
#include <hag/drivers/vga/attribc/bordcol.h>                //AR11
#include <hag/drivers/vga/attribc/colplnen.h>               //AR12
#include <hag/drivers/vga/attribc/horpxpan.h>               //AR13
#include <hag/drivers/vga/attribc/pixelpad.h>               //AR14

#include <hag/drivers/vga/miscout.h>                        //0x3C2, 0x3CC
#include <hag/drivers/vga/instat0.h>                        //0x3C2
#include <hag/drivers/vga/vidsuben.h>                       //0x3C3

#include <hag/drivers/vga/sqrc/index.h>                     //SEQ Index 0x3C4
#include <hag/drivers/vga/sqrc/data.h>                      //SEQ Data  0x3C5
#include <hag/drivers/vga/sqrc/reset.h>                     //SR0
#include <hag/drivers/vga/sqrc/clkmod.h>                    //SR1
#include <hag/drivers/vga/sqrc/enwrtpl.h>                   //SR2
#include <hag/drivers/vga/sqrc/chfntsel.h>                  //SR3
#include <hag/drivers/matrox/shared/sqrc/memodctl.h>        //SR4

#include <hag/drivers/vga/dacmask.h>                        //0x3C6
#include <hag/drivers/vga/dacrdidx.h>                       //0x3C7
#include <hag/drivers/vga/dacstat.h>                        //0x3C7
#include <hag/drivers/vga/dacwridx.h>                       //0x3C8
#include <hag/drivers/vga/dacdata.h>                        //0x3C9

#include <hag/drivers/vga/gfxc/index.h>                     //GFX Index 0x3CE
#include <hag/drivers/vga/gfxc/data.h>                      //GFX Data  0x3CF
#include <hag/drivers/vga/gfxc/srdata.h>                    //GR0
#include <hag/drivers/vga/gfxc/ensrdata.h>                  //GR1
#include <hag/drivers/vga/gfxc/colcmp.h>                    //GR2
#include <hag/drivers/vga/gfxc/roprotct.h>                  //GR3
#include <hag/drivers/vga/gfxc/rdplnsel.h>                  //GR4
#include <hag/drivers/vga/gfxc/gfxcmode.h>                  //GR5
#include <hag/drivers/vga/gfxc/mmmctrl.h>                   //GR6
#include <hag/drivers/vga/gfxc/coldc.h>                     //GR7
#include <hag/drivers/vga/gfxc/bitmask.h>                   //GR8

namespace Hag::TDfx::Banshee
{
//IO Addresses from IOAddressBase

// 0x00 - 0x03 status Register
// Initialization registers
// 0x04 - 0x07 pciInit0 register
// 0x08 - 0x0b sipMonitor register
// 0x0c - 0x0f lfbMemoryConfig register
// 0x10 - 0x13 miscInit0 register
// 0x14 - 0x17 miscInit1 register
// 0x18 - 0x1b dramInit0 register
// 0x1c - 0x1f dramInit1 register
// 0x20 - 0x23 agpInit register
// 0x24 - 0x27 tmuGbeInit register
// 0x28 - 0x2b vgaInit0 register
// 0x2c - 0x2f vgaInit1 register
// 0x30 - 0x33 dramCommand register (see 2D offset 0x70)
// 0x34 - 0x37 dramData register (see 2D offset 0x064)
// 0x38 - 0x3b reserved
// PLL and Dac registers
// 0x40 - 0x43 pllCtrl0
// 0x44 - 0x47 pllCtrl1
// 0x48 - 0x4b pllCtrl2
// 0x4c - 0x4f dacMode register.
// 0x50 - 0x53 dacAddr register.
// 0x54 - 0x57 dacData register.
// Video Registers part I
// 0x58 - 0x5b rgbMaxDelta register
// 0x5c - 0x5f vidProcCfg register.
// 0x60 - 0x63 hwCurPatAddr register.
// 0x64 - 0x67 hwCurLoc register.
// 0x68 - 0x6b hwCurC0 register
// 0x6c - 0x6f hwCurC1 register.
// 0x70 - 0x73 vidInFormat register
// 0x74 - 0x77 vidInStatus register
// 0x78 - 0x7b vidSerialParallelPort register
// 0x7c - 0x7f vidInXDecimDeltas register.
// 0x80 - 0x83 vidInDecimInitErrs register.
// 0x84 - 0x87 vidInYDecimDeltas register.
// 0x88 - 0x8b vidPixelBufThold register
// 0x8c - 0x8f vidChromaMin register.
// 0x90 - 0x93 vidChromaMax register.
// 0x94 - 0x97 vidCurrentLine register.
// 0x98 - 0x9b vidScreenSize register.
// 0x9c - 0x9f vidOverlayStartCoords register.
// 0xa0 - 0xa3 vidOverlayEndScreenCoord register.
// 0xa4 - 0xa7 vidOverlayDudx register
// 0xa8 - 0xab vidOverlayDudxOffsetSrcWidth register.
// 0xac - 0xaf vidOverlayDvdy register.
// VGA Registers
// 0xb0 - 0xdf vga registers (only in I/O space, not memory mapped)
// Video Registers part II
// 0xe0 - 0xe3 vidOverlayDvdyOffset register.
// 0xe4 - 0xe7 vidDesktopStartAddr register.
// 0xe8 - 0xeb vidDesktopOverlayStride register.
// 0xec - 0xef vidInAddr0 register
// 0xf0 - 0xf3 vidInAddr1 register.
// 0xf4 - 0xf7 vidInAddr2 register.
// 0xf8 - 0xfb vidInStride register.
// 0xfc - 0xff vidCurrOverlayStartAddr register.


}