//Copyright 2023-Present riplin

#pragma once

#include <hag/driver.h>
#include <hag/factory.h>
#include <hag/ialloc.h>
#include <hag/types.h>

#include <hag/vesa/ddc.h>
#include <hag/vesa/pm.h>
#include <hag/vesa/vidmodes.h>

#include <hag/testing/mock.h>

#include <hag/system/bda.h>
#include <hag/system/interrup.h>
#include <hag/system/sysasm.h>

#include <hag/support/allocatr.h>

#include <hag/drivers/vga/attribc/attrmode.h>
#include <hag/drivers/vga/attribc/bordcol.h>
#include <hag/drivers/vga/attribc/colplnen.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/attribc/horpxpan.h>
#include <hag/drivers/vga/attribc/index.h>
#include <hag/drivers/vga/attribc/palette.h>
#include <hag/drivers/vga/attribc/pixelpad.h>
#include <hag/drivers/vga/attribc/regs.h>

#include <hag/drivers/vga/crtc/crtcovfl.h>
#include <hag/drivers/vga/crtc/curendsl.h>
#include <hag/drivers/vga/crtc/curladhi.h>
#include <hag/drivers/vga/crtc/curladlo.h>
#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/vga/crtc/enhorbln.h>
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/enverbln.h>
#include <hag/drivers/vga/crtc/hordisen.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/vga/crtc/index.h>
#include <hag/drivers/vga/crtc/linecomp.h>
#include <hag/drivers/vga/crtc/maxscanl.h>
#include <hag/drivers/vga/crtc/modectrl.h>
#include <hag/drivers/vga/crtc/prstrwsc.h>
#include <hag/drivers/vga/crtc/regs.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/staddrhi.h>
#include <hag/drivers/vga/crtc/staddrlo.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/stverbln.h>
#include <hag/drivers/vga/crtc/undloc.h>
#include <hag/drivers/vga/crtc/verdisen.h>
#include <hag/drivers/vga/crtc/verrtcen.h>
#include <hag/drivers/vga/crtc/verrtcst.h>
#include <hag/drivers/vga/crtc/vertotal.h>

#include <hag/drivers/vga/gfxc/bitmask.h>
#include <hag/drivers/vga/gfxc/colcmp.h>
#include <hag/drivers/vga/gfxc/coldc.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/gfxc/ensrdata.h>
#include <hag/drivers/vga/gfxc/gfxcmode.h>
#include <hag/drivers/vga/gfxc/index.h>
#include <hag/drivers/vga/gfxc/mmmctrl.h>
#include <hag/drivers/vga/gfxc/rdplnsel.h>
#include <hag/drivers/vga/gfxc/regs.h>
#include <hag/drivers/vga/gfxc/roprotct.h>
#include <hag/drivers/vga/gfxc/srdata.h>

#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/vga/sqrc/index.h>
#include <hag/drivers/vga/sqrc/memodctl.h>
#include <hag/drivers/vga/sqrc/regs.h>
#include <hag/drivers/vga/sqrc/reset.h>

#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacrdidx.h>
#include <hag/drivers/vga/dacstat.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/vga/featctl.h>
#include <hag/drivers/vga/instat0.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/regs.h>
#include <hag/drivers/vga/vidmodes.h>

#include <hag/drivers/s3/attribc/colplnen.h>

#include <hag/drivers/s3/crtc/bkwcomp2.h>
#include <hag/drivers/s3/crtc/chipidrv.h>
#include <hag/drivers/s3/crtc/conf1.h>
#include <hag/drivers/s3/crtc/conf1v2.h>
#include <hag/drivers/s3/crtc/conf2v2.h>
#include <hag/drivers/s3/crtc/crtreglk.h>
#include <hag/drivers/s3/crtc/devidhi.h>
#include <hag/drivers/s3/crtc/devidlo.h>
#include <hag/drivers/s3/crtc/exhorovf.h>
#include <hag/drivers/s3/crtc/exmscct2.h>
#include <hag/drivers/s3/crtc/exsysct1.h>
#include <hag/drivers/s3/crtc/exsysct2.h>
#include <hag/drivers/s3/crtc/exsysct3.h>
#include <hag/drivers/s3/crtc/extmode.h>
#include <hag/drivers/s3/crtc/extrdctl.h>
#include <hag/drivers/s3/crtc/exverovf.h>
#include <hag/drivers/s3/crtc/genout.h>
#include <hag/drivers/s3/crtc/reglock1.h>
#include <hag/drivers/s3/crtc/reglock2.h>
#include <hag/drivers/s3/crtc/regs.h>
#include <hag/drivers/s3/crtc/revision.h>
#include <hag/drivers/s3/crtc/sysconf.h>

#include <hag/drivers/s3/readreg/regs.h>

#include <hag/drivers/s3/sqrc/clksync1.h>
#include <hag/drivers/s3/sqrc/clksync2.h>
#include <hag/drivers/s3/sqrc/dclkvhi.h>
#include <hag/drivers/s3/sqrc/dclkvlow.h>
#include <hag/drivers/s3/sqrc/extbusrc.h>
#include <hag/drivers/s3/sqrc/extseq9.h>
#include <hag/drivers/s3/sqrc/extseqd.h>
#include <hag/drivers/s3/sqrc/mclkvhi.h>
#include <hag/drivers/s3/sqrc/mclkvlow.h>
#include <hag/drivers/s3/sqrc/mscextsq.h>
#include <hag/drivers/s3/sqrc/rclksync.h>
#include <hag/drivers/s3/sqrc/regs.h>
#include <hag/drivers/s3/sqrc/unlexseq.h>

#include <hag/drivers/s3/advfnctl.h>
#include <hag/drivers/s3/chipid.h>
#include <hag/drivers/s3/ddchw.h>
#include <hag/drivers/s3/factory.h>
#include <hag/drivers/s3/gfxprocs.h>
#include <hag/drivers/s3/hardware.h>
#include <hag/drivers/s3/instat0.h>
#include <hag/drivers/s3/pmhw.h>
#include <hag/drivers/s3/regs.h>
#include <hag/drivers/s3/trio.h>
#include <hag/drivers/s3/vidmodes.h>
#include <hag/drivers/s3/virge.h>
