//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/vga/vidmodes.h>

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

#include <hag/drivers/matrox/shared/miscout.h>              //0x3C2, 0x3CC
#include <hag/drivers/vga/instat0.h>                        //0x3C2
#include <hag/drivers/vga/vidsuben.h>                       //0x3C3

#include <hag/drivers/vga/sqrc/index.h>                     //SEQ Index 0x3C4
#include <hag/drivers/vga/sqrc/data.h>                      //SEQ Data  0x3C5
#include <hag/drivers/vga/sqrc/reset.h>                     //SR0
#include <hag/drivers/vga/sqrc/clkmod.h>                    //SR1
#include <hag/drivers/vga/sqrc/enwrtpl.h>                   //SR2
#include <hag/drivers/vga/sqrc/chfntsel.h>                  //SR3
#include <hag/drivers/vga/sqrc/memodctl.h>                  //SR4

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
