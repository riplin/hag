//Copyright 2025-Present riplin

//Matrox Mystique Specification: https://bitsavers.trailing-edge.com/components/matrox/_dataSheets/MGA-1064sg_199702.pdf

#pragma once

#include <hag/drivers/imports.h>

#include <hag/drivers/matrox/shared/regtype.h>

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
#include <hag/drivers/vga/crtc/cpulatch.h>                  //CR22
#include <hag/drivers/vga/crtc/attridxf.h>                  //CR24
#include <hag/drivers/vga/crtc/attridxi.h>                  //CR26

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
//#include <hag/drivers/vga/vidsuben.h>                       //0x3C3

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

#include <hag/drivers/matrox/shared/crtcext/index.h>        //CRTCE Index 0x3DE
#include <hag/drivers/matrox/shared/crtcext/data.h>         //CRTCE Data  0x3DF
#include <hag/drivers/matrox/shared/crtcext/adgenext.h>     //CER0
#include <hag/drivers/matrox/shared/crtcext/hrcntext.h>     //CER1
#include <hag/drivers/matrox/shared/crtcext/vrcntext.h>     //CER2
#include <hag/drivers/matrox/shared/crtcext/misc.h>         //CER3
#include <hag/drivers/matrox/shared/crtcext/mempg.h>        //CER4
#include <hag/drivers/matrox/shared/crtcext/hrhlfcnt.h>     //CER5

#include <hag/drivers/matrox/shared/pci/devid.h>            //PCI00
#include <hag/drivers/matrox/shared/pci/devctl.h>           //PCI04
#include <hag/drivers/matrox/shared/pci/clsscod.h>          //PCI08
#include <hag/drivers/matrox/shared/pci/hdr.h>              //PCI0C
#include <hag/drivers/matrox/shared/pci/ctrlap.h>           //PCI10
#include <hag/drivers/matrox/shared/pci/fbap.h>             //PCI14
#include <hag/drivers/matrox/shared/pci/iloadap.h>          //PCI18
#include <hag/drivers/matrox/shared/pci/subsysid.h>         //PCI2C, PCI4C
#include <hag/drivers/matrox/shared/pci/rombase.h>          //PCI30
#include <hag/drivers/matrox/shared/pci/intctrl.h>          //PCI3C
#include <hag/drivers/matrox/shared/pci/opt.h>              //PCI40

#include <hag/drivers/matrox/shared/idx/curbadl.h>          //IDX04
#include <hag/drivers/matrox/shared/idx/curbadh.h>          //IDX05
#include <hag/drivers/matrox/shared/idx/curctrl.h>          //IDX06
#include <hag/drivers/matrox/shared/idx/curcols.h>          //IDX08, IDX09, IDX0A, IDX0C, IDX0D, IDX0E, IDX10, IDX11, IDX12
#include <hag/drivers/matrox/shared/idx/vrefctrl.h>         //IDX18
#include <hag/drivers/matrox/shared/idx/muxctrl.h>          //IDX19
#include <hag/drivers/matrox/shared/idx/pclkctrl.h>         //IDX1A
#include <hag/drivers/matrox/shared/idx/genctrl.h>          //IDX1D
#include <hag/drivers/matrox/shared/idx/miscctrl.h>         //IDX1E
#include <hag/drivers/matrox/shared/idx/gpioctrl.h>         //IDX2A
#include <hag/drivers/matrox/shared/idx/gpiodata.h>         //IDX2B
#include <hag/drivers/matrox/shared/idx/syspllm.h>          //IDX2C
#include <hag/drivers/matrox/shared/idx/sysplln.h>          //IDX2D
#include <hag/drivers/matrox/shared/idx/syspllp.h>          //IDX2E
#include <hag/drivers/matrox/shared/idx/syspllst.h>         //IDX2F
#include <hag/drivers/matrox/shared/idx/zoomctrl.h>         //IDX38
#include <hag/drivers/matrox/shared/idx/sensetst.h>         //IDX3A
#include <hag/drivers/matrox/shared/idx/crcreml.h>          //IDX3C
#include <hag/drivers/matrox/shared/idx/crcremh.h>          //IDX3D
#include <hag/drivers/matrox/shared/idx/crcbitsl.h>         //IDX3E
#include <hag/drivers/matrox/shared/idx/colkeyml.h>         //IDX40
#include <hag/drivers/matrox/shared/idx/colkeymh.h>         //IDX41
#include <hag/drivers/matrox/shared/idx/colkeyl.h>          //IDX42
#include <hag/drivers/matrox/shared/idx/colkeyh.h>          //IDX43
#include <hag/drivers/matrox/shared/idx/pixpllm.h>          //IDX44, IDX48, IDX4C
#include <hag/drivers/matrox/shared/idx/pixplln.h>          //IDX45, IDX49, IDX4D
#include <hag/drivers/matrox/shared/idx/pixpllp.h>          //IDX46, IDX4A, IDX4E
#include <hag/drivers/matrox/shared/idx/pixpllst.h>         //IDX4F


#include <hag/drivers/matrox/shared/pci/ind/indidx.h>       //PCI44
#include <hag/drivers/matrox/shared/pci/ind/inddat.h>       //PCI48

#include <hag/drivers/matrox/shared/pci/idx/curbadl.h>      //IDX04
#include <hag/drivers/matrox/shared/pci/idx/curbadh.h>      //IDX05
#include <hag/drivers/matrox/shared/pci/idx/curctrl.h>      //IDX06
#include <hag/drivers/matrox/shared/pci/idx/curcols.h>      //IDX08, IDX09, IDX0A, IDX0C, IDX0D, IDX0E, IDX10, IDX11, IDX12
#include <hag/drivers/matrox/shared/pci/idx/vrefctrl.h>     //IDX18
#include <hag/drivers/matrox/shared/pci/idx/muxctrl.h>      //IDX19
#include <hag/drivers/matrox/shared/pci/idx/pclkctrl.h>     //IDX1A
#include <hag/drivers/matrox/shared/pci/idx/genctrl.h>      //IDX1D
#include <hag/drivers/matrox/shared/pci/idx/miscctrl.h>     //IDX1E
#include <hag/drivers/matrox/shared/pci/idx/gpioctrl.h>     //IDX2A
#include <hag/drivers/matrox/shared/pci/idx/gpiodata.h>     //IDX2B
#include <hag/drivers/matrox/shared/pci/idx/syspllm.h>      //IDX2C
#include <hag/drivers/matrox/shared/pci/idx/sysplln.h>      //IDX2D
#include <hag/drivers/matrox/shared/pci/idx/syspllp.h>      //IDX2E
#include <hag/drivers/matrox/shared/pci/idx/syspllst.h>     //IDX2F
#include <hag/drivers/matrox/shared/pci/idx/zoomctrl.h>     //IDX38
#include <hag/drivers/matrox/shared/pci/idx/sensetst.h>     //IDX3A
#include <hag/drivers/matrox/shared/pci/idx/crcreml.h>      //IDX3C
#include <hag/drivers/matrox/shared/pci/idx/crcremh.h>      //IDX3D
#include <hag/drivers/matrox/shared/pci/idx/crcbitsl.h>     //IDX3E
#include <hag/drivers/matrox/shared/pci/idx/colkeyml.h>     //IDX40
#include <hag/drivers/matrox/shared/pci/idx/colkeymh.h>     //IDX41
#include <hag/drivers/matrox/shared/pci/idx/colkeyl.h>      //IDX42
#include <hag/drivers/matrox/shared/pci/idx/colkeyh.h>      //IDX43
#include <hag/drivers/matrox/shared/pci/idx/pixpllm.h>      //IDX44, IDX48, IDX4C
#include <hag/drivers/matrox/shared/pci/idx/pixplln.h>      //IDX45, IDX49, IDX4D
#include <hag/drivers/matrox/shared/pci/idx/pixpllp.h>      //IDX46, IDX4A, IDX4E
#include <hag/drivers/matrox/shared/pci/idx/pixpllst.h>     //IDX4F

#include <hag/drivers/matrox/shared/regs/dwgctrl.h>         //0x1C00
#include <hag/drivers/matrox/shared/regs/memacc.h>          //0x1C04
#include <hag/drivers/matrox/shared/regs/memctlws.h>        //0x1C08
#include <hag/drivers/matrox/shared/regs/zdorg.h>           //0x1C0C
#include <hag/drivers/matrox/shared/regs/ptrn.h>            //0x1C10, 0x1C14
#include <hag/drivers/matrox/shared/regs/plnwrmsk.h>        //0x1C1C
#include <hag/drivers/matrox/shared/regs/bgcol.h>           //0x1C20
#include <hag/drivers/matrox/shared/regs/blcolmsk.h>        //0x1C20
#include <hag/drivers/matrox/shared/regs/fgcol.h>           //0x1C24
#include <hag/drivers/matrox/shared/regs/blcolkey.h>        //0x1C24
#include <hag/drivers/matrox/shared/regs/src.h>             //0x1C30, 0x1C34, 0x1C38, 0x1C3C
#include <hag/drivers/matrox/shared/regs/xyaddr.h>          //0x1C40, 0x1C44, 0x1C84, 0x1CA8, 0x1CAC, 0x1CB0, 0x1C88, 0x1C90
#include <hag/drivers/matrox/shared/regs/shift.h>           //0x1C50
#include <hag/drivers/matrox/shared/regs/dmapad.h>          //0x1C54
#include <hag/drivers/matrox/shared/regs/sign.h>            //0x1C58
#include <hag/drivers/matrox/shared/regs/length.h>          //0x1C5C
#include <hag/drivers/matrox/shared/regs/mpaddr0.h>         //0x1C60
#include <hag/drivers/matrox/shared/regs/mpaddr1.h>         //0x1C64
#include <hag/drivers/matrox/shared/regs/mpaddr2.h>         //0x1C68
#include <hag/drivers/matrox/shared/regs/mpaddr3.h>         //0x1C6C
#include <hag/drivers/matrox/shared/regs/mpaddr4.h>         //0x1C70
#include <hag/drivers/matrox/shared/regs/mpaddr5.h>         //0x1C74
#include <hag/drivers/matrox/shared/regs/mpaddr6.h>         //0x1C78
#include <hag/drivers/matrox/shared/regs/cxbound.h>         //0x1C80
#include <hag/drivers/matrox/shared/regs/cytop.h>           //0x1C98
#include <hag/drivers/matrox/shared/regs/cybot.h>           //0x1C9C
#include <hag/drivers/matrox/shared/regs/cxlft.h>           //0x1CA0
#include <hag/drivers/matrox/shared/regs/cxrgt.h>           //0x1CA4
#include <hag/drivers/matrox/shared/regs/memptch.h>         //0x1C8C
#include <hag/drivers/matrox/shared/regs/memorg.h>          //0x1C94
#include <hag/drivers/matrox/shared/regs/status.h>          //0x1E14

#include <hag/drivers/matrox/shared/pci/ind/dwgctrl.h>      //0x1C00
#include <hag/drivers/matrox/shared/pci/ind/memacc.h>       //0x1C04
#include <hag/drivers/matrox/shared/pci/ind/memctlws.h>     //0x1C08
#include <hag/drivers/matrox/shared/pci/ind/zdorg.h>        //0x1C0C
#include <hag/drivers/matrox/shared/pci/ind/ptrn.h>         //0x1C10, 0x1C14
#include <hag/drivers/matrox/shared/pci/ind/plnwrmsk.h>     //0x1C1C
#include <hag/drivers/matrox/shared/pci/ind/bgcol.h>        //0x1C20
#include <hag/drivers/matrox/shared/pci/ind/blcolmsk.h>     //0x1C20
#include <hag/drivers/matrox/shared/pci/ind/fgcol.h>        //0x1C24
#include <hag/drivers/matrox/shared/pci/ind/blcolkey.h>     //0x1C24
#include <hag/drivers/matrox/shared/pci/ind/src.h>          //0x1C30, 0x1C34, 0x1C38, 0x1C3C
#include <hag/drivers/matrox/shared/pci/ind/xyaddr.h>       //0x1C40, 0x1C44, 0x1C84, 0x1CA8, 0x1CAC, 0x1CB0, 0x1C88, 0x1C90
#include <hag/drivers/matrox/shared/pci/ind/shift.h>        //0x1C50
#include <hag/drivers/matrox/shared/pci/ind/dmapad.h>       //0x1C54
#include <hag/drivers/matrox/shared/pci/ind/sign.h>         //0x1C58
#include <hag/drivers/matrox/shared/pci/ind/length.h>       //0x1C5C
#include <hag/drivers/matrox/shared/pci/ind/mpaddr.h>       //0x1C60, 0x1C64, 0x1C68, 0x1C6C, 0x1C70, 0x1C74, 0x1C78
#include <hag/drivers/matrox/shared/pci/ind/clipper.h>      //0x1C80, 0x1C98, 0x1CA0, 0x1CA4
#include <hag/drivers/matrox/shared/pci/ind/memptch.h>      //0x1C8C
#include <hag/drivers/matrox/shared/pci/ind/memorg.h>       //0x1C94
#include <hag/drivers/matrox/shared/pci/ind/status.h>       //0x1E14

#include <hag/drivers/matrox/shared/mmio/dwgctrl.h>         //0x1C00
#include <hag/drivers/matrox/shared/mmio/memacc.h>          //0x1C04
#include <hag/drivers/matrox/shared/mmio/memctlws.h>        //0x1C08
#include <hag/drivers/matrox/shared/mmio/zdorg.h>           //0x1C0C
#include <hag/drivers/matrox/shared/mmio/ptrn.h>            //0x1C10, 0x1C14
#include <hag/drivers/matrox/shared/mmio/plnwrmsk.h>        //0x1C1C
#include <hag/drivers/matrox/shared/mmio/bgcol.h>           //0x1C20
#include <hag/drivers/matrox/shared/mmio/blcolmsk.h>        //0x1C20
#include <hag/drivers/matrox/shared/mmio/fgcol.h>           //0x1C24
#include <hag/drivers/matrox/shared/mmio/blcolkey.h>        //0x1C24
#include <hag/drivers/matrox/shared/mmio/src.h>             //0x1C30, 0x1C34, 0x1C38, 0x1C3C
#include <hag/drivers/matrox/shared/mmio/xyaddr.h>          //0x1C40, 0x1C44, 0x1C84, 0x1CA8, 0x1CAC, 0x1CB0, 0x1C88, 0x1C90
#include <hag/drivers/matrox/shared/mmio/shift.h>           //0x1C50
#include <hag/drivers/matrox/shared/mmio/dmapad.h>          //0x1C54
#include <hag/drivers/matrox/shared/mmio/sign.h>            //0x1C58
#include <hag/drivers/matrox/shared/mmio/length.h>          //0x1C5C
#include <hag/drivers/matrox/shared/mmio/mpaddr.h>          //0x1C60, 0x1C64, 0x1C68, 0x1C6C, 0x1C70, 0x1C74, 0x1C78
#include <hag/drivers/matrox/shared/mmio/clipper.h>         //0x1C80, 0x1C98, 0x1CA0, 0x1CA4
#include <hag/drivers/matrox/shared/mmio/memptch.h>         //0x1C8C
#include <hag/drivers/matrox/shared/mmio/memorg.h>          //0x1C94
#include <hag/drivers/matrox/shared/mmio/status.h>          //0x1E14

//Functions
#include <hag/drivers/vga/modeset.h>
#include <hag/drivers/matrox/shared/funcs/system.h>

namespace Hag::Matrox::Mystique
{
    //HAG_IMPORT_SYSTEM;//Import initialization and shutdown.
    HAG_IMPORT_MODESETTING;//Import SetVideoMode.

    typedef Shared::Register_t Register_t;

    namespace Register
    {
        using namespace VGA::Register;
        using namespace Shared::Register;
    }
    
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, FeatureControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, InputStatus1);

    IMPORTNAMESPACE(VGA, CRTControllerIndex);
    IMPORTNAMESPACEANDTYPE(VGA, CRTControllerData);
    
    namespace CRTController
    {
        IMPORTNAMESPACE(VGA::CRTController, Register);
    
        IMPORTNAMESPACEANDTYPE(VGA::CRTController, HorizontalTotal);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, HorizontalDisplayEnd);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, StartHorizontalBlank);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, EndHorizontalBlank);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, StartHorizontalSyncPosition);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, EndHorizontalSyncPosition);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, VerticalTotal);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CRTCOverflow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, PresetRowScan);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, MaximumScanLine);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CursorStartScanLine);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CursorEndScanLine);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, StartAddressHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, StartAddressLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CursorLocationAddressHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CursorLocationAddressLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, VerticalRetraceStart);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, VerticalRetraceEnd);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, VerticalDisplayEnd);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, ScreenOffset);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, UnderlineLocation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, StartVerticalBlank);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, EndVerticalBlank);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, CRTCModeControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, LineCompare);
        IMPORTNAMESPACEANDTYPE(VGA::CRTController, CPULatchData);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, AttributeIndexI);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::CRTController, AttributeIndexF);
    }

    IMPORTNAMESPACE(VGA, AttributeControllerIndex);
    IMPORTNAMESPACEANDTYPE(VGA, AttributeControllerData);

    namespace AttributeController
    {
        IMPORTNAMESPACE(VGA::AttributeController, Register);
    
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, AttributeMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, BorderColor);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, ColorPlane);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, HorizontalPixelPanning);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, Palette);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, PixelPadding);
    }

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MiscellaneousOutput);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, InputStatus0);
    //IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, VideoSubsystemEnable); //Docs don't specify this register

    IMPORTNAMESPACE(VGA, SequencerIndex);
    IMPORTNAMESPACEANDTYPE(VGA, SequencerData);
    
    namespace Sequencer
    {
        IMPORTNAMESPACE(VGA::Sequencer, Register);
    
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, Reset);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, ClockingMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, EnableWritePlane);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, CharacterFontSelect);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, MemoryModeControl);
    }

    IMPORTNAMESPACEANDTYPE(VGA, DACMask);
    IMPORTNAMESPACEANDTYPE(VGA, DACReadIndex);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, DACStatus);
    IMPORTNAMESPACEANDTYPE(VGA, DACWriteIndex);
    IMPORTNAMESPACEANDTYPE(VGA, RAMDACData);    

    IMPORTNAMESPACE(VGA, GraphicsControllerIndex);
    IMPORTNAMESPACEANDTYPE(VGA, GraphicsControllerData);

    namespace GraphicsController
    {
        IMPORTNAMESPACEANDTYPE(VGA::GraphicsController, Register);
    
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, SetResetData);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, EnableSetResetData);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, ColorCompare);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, RasterOperationRotateCount);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, ReadPlaneSelect);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, GraphicsControllerMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, MemoryMapModeControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, ColorDontCare);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::GraphicsController, BitMask);
    }
    
    IMPORTNAMESPACE(Shared, CRTCExtensionIndex);
    IMPORTNAMESPACEANDTYPE(Shared, CRTCExtensionData);

    namespace CRTCExtension
    {
        IMPORTNAMESPACEANDTYPE(Shared::CRTCExtension, Register);

        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, AddressGeneratorExtensions);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, HorizontalCounterExtensions);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, VerticalCounterExtensions);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, Miscellaneous);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, MemoryPage);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTCExtension, HorizontalHalfCount);
    }

    namespace Indexed
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CursorBaseAddressLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CursorBaseAddressHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CursorControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CursorColor);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, VoltageReferenceControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, MultiplexControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, PixelClockControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, GeneralControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, MiscellaneousControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, GeneralPurposeIOControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, GeneralPurposeIOData);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, SystemPLLM);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, SystemPLLN);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, SystemPLLP);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, SystemPLLStatus);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, ZoomControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, SenseTest);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CRCRemainderLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CRCRemainderHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, CRCBitSelect);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, ColorKeyMaskLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, ColorKeyMaskHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, ColorKeyLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, ColorKeyHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, PixelPLLM);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, PixelPLLN);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, PixelPLLP);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Indexed, PixelPLLStatus);
    }

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, DrawingControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MemoryAccess);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MemoryControlWaitState);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ZDepthOrigin);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Pattern);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, PlaneWriteMask);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, BackgroundColor);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, BlitColorMask);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ForegroundColor);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, BlitColorKey);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Source);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, XYAddress);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, XLeftRight);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, XLeft);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, XRight);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, XDestination);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, YDestinationAndLength);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, YDestination);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, FunnelShifterControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, DMAPadding);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Sign);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Length);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress0);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress4);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress5);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MultiPurposeAddress6);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ClipperXBoundary);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ClipperYTopBoundary);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ClipperYBottomBoundary);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ClipperXLeftBoundary);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, ClipperXRightBoundary);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MemoryPitch);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MemoryOrigin);

    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Status);

    namespace PCI
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, DeviceIdentification);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, DeviceControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, ClassCode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, Header);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, ControlAperture);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, FrameBufferAperture);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, ILOADAperture);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, SubsystemID);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, RomBaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, InterruptControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::PCI, Option);

        IMPORTNAMESPACE(Shared::PCI, IndirectIndex);
        IMPORTNAMESPACEANDTYPE(Shared::PCI, IndirectData);

        namespace Indexed
        {
            IMPORTNAMESPACE(Shared::PCI::Indexed, CursorBaseAddressLow);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CursorBaseAddressHigh);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CursorControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CursorColor);
            IMPORTNAMESPACE(Shared::PCI::Indexed, VoltageReferenceControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, MultiplexControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, PixelClockControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, GeneralControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, MiscellaneousControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, GeneralPurposeIOControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, GeneralPurposeIOData);
            IMPORTNAMESPACE(Shared::PCI::Indexed, SystemPLLM);
            IMPORTNAMESPACE(Shared::PCI::Indexed, SystemPLLN);
            IMPORTNAMESPACE(Shared::PCI::Indexed, SystemPLLP);
            IMPORTNAMESPACE(Shared::PCI::Indexed, SystemPLLStatus);
            IMPORTNAMESPACE(Shared::PCI::Indexed, ZoomControl);
            IMPORTNAMESPACE(Shared::PCI::Indexed, SenseTest);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CRCRemainderLow);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CRCRemainderHigh);
            IMPORTNAMESPACE(Shared::PCI::Indexed, CRCBitSelect);
            IMPORTNAMESPACE(Shared::PCI::Indexed, ColorKeyMaskLow);
            IMPORTNAMESPACE(Shared::PCI::Indexed, ColorKeyMaskHigh);
            IMPORTNAMESPACE(Shared::PCI::Indexed, ColorKeyLow);
            IMPORTNAMESPACE(Shared::PCI::Indexed, ColorKeyHigh);
            IMPORTNAMESPACE(Shared::PCI::Indexed, PixelPLLM);
            IMPORTNAMESPACE(Shared::PCI::Indexed, PixelPLLN);
            IMPORTNAMESPACE(Shared::PCI::Indexed, PixelPLLP);
            IMPORTNAMESPACE(Shared::PCI::Indexed, PixelPLLStatus);
        }
        
        namespace Indirect
        {
            IMPORTNAMESPACE(Shared::PCI::Indirect, DrawingControl);
            IMPORTNAMESPACE(Shared::PCI::Indirect, MemoryAccess);
            IMPORTNAMESPACE(Shared::PCI::Indirect, MemoryControlWaitState);
            IMPORTNAMESPACE(Shared::PCI::Indirect, ZDepthOrigin);
            IMPORTNAMESPACE(Shared::PCI::Indirect, Pattern);
            IMPORTNAMESPACE(Shared::PCI::Indirect, PlaneWriteMask);
            IMPORTNAMESPACE(Shared::PCI::Indirect, BackgroundColor);
            IMPORTNAMESPACE(Shared::PCI::Indirect, BlitColorMask);
            IMPORTNAMESPACE(Shared::PCI::Indirect, ForegroundColor);
            IMPORTNAMESPACE(Shared::PCI::Indirect, BlitColorKey);
            IMPORTNAMESPACE(Shared::PCI::Indirect, Source);
            IMPORTNAMESPACE(Shared::PCI::Indirect, XYAddress);
            IMPORTNAMESPACE(Shared::PCI::Indirect, FunnelShifterControl);
            IMPORTNAMESPACE(Shared::PCI::Indirect, DMAPadding);
            IMPORTNAMESPACE(Shared::PCI::Indirect, Sign);
            IMPORTNAMESPACE(Shared::PCI::Indirect, Length);
            IMPORTNAMESPACE(Shared::PCI::Indirect, MultiPurposeAddress);
            IMPORTNAMESPACE(Shared::PCI::Indirect, Clipper);
            IMPORTNAMESPACE(Shared::PCI::Indirect, MemoryPitch);
            IMPORTNAMESPACE(Shared::PCI::Indirect, MemoryOrigin);

            IMPORTNAMESPACE(Shared::PCI::Indirect, Status);
        }
    }

    namespace MMIO
    {
        IMPORTNAMESPACE(Shared::MMIO, DrawingControl);                          // DrawingControl (3)           WO 0x1C00 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, MemoryAccess);                            // MemoryAccess (3)             WO 0x1C04 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, MemoryControlWaitState);                  // MemoryControlWaitState       WO 0x1C08 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, ZDepthOrigin);                            // ZDepthOrigin                 WO 0x1C0C (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, Pattern);                                 // Pattern0                     WO 0x1C10 (4)   FIFO
                                                                                // Pattern1                     WO 0x1C14 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, PlaneWriteMask);                          // PlaneWriteMask (3)           WO 0x1C1C (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, BackgroundColor);                         // BackgroundColor              WO 0x1C20 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, BlitColorMask);                           // BlitColorMask                WO 0x1C20 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, ForegroundColor);                         // ForegroundColor              WO 0x1C24 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, BlitColorKey);                            // BlitColorKey                 WO 0x1C24 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, Source);                                  // Source0                      WO 0x1C30 (4)   FIFO
                                                                                // Source1                      WO 0x1C34 (4)   FIFO
                                                                                // Source2                      WO 0x1C38 (4)   FIFO
                                                                                // Source3                      WO 0x1C3C (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, XYAddress);                               // XYStartAddress (5)           WO 0x1C40 (4)   FIFO
                                                                                // XYEndAddress (5)             WO 0x1C44 (4)   FIFO
                                                                                // XAddressBoundary (5)         WO 0x1C84 (4)   FIFO
                                                                                // XAddressLeft (5)             WO 0x1CA8 (4)   FIFO
                                                                                // XAddressRight (5)            WO 0x1CAC (4)   FIFO
                                                                                // XDestinationAddress (5)      WO 0x1CB0 (4)   FIFO
                                                                                // YDestinationAndLength (5)    WO 0x1C88 (4)   FIFO
                                                                                // YAddress (5)                 WO 0x1C90 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, FunnelShifterControl);                    // FunnelShifterControl (5)     WO 0x1C50 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, DMAPadding);                              // DMAPadding (5)               WO 0x1C54 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, Sign);                                    // Sign (5)                     WO 0x1C58 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, Length);                                  // Length (5)                   WO 0x1C5C (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, MultiPurposeAddress);                     // MultiPurposeAddress0 (5)     WO 0x1C60 (4)   FIFO
                                                                                // MultiPurposeAddress1 (5)     WO 0x1C64 (4)   FIFO
                                                                                // MultiPurposeAddress2 (5)     WO 0x1C68 (4)   FIFO
                                                                                // MultiPurposeAddress3 (5)     WO 0x1C6C (4)   FIFO
                                                                                // MultiPurposeAddress4 (5)     WO 0x1C70 (4)   FIFO
                                                                                // MultiPurposeAddress5 (5)     WO 0x1C74 (4)   FIFO
                                                                                // MultiPurposeAddress6 (5)     WO 0x1C78 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, Clipper);                                 // ClipperXBoundary (5)         WO 0x1C80 (4)   FIFO
                                                                                // ClipperYTopBoundary (5)      WO 0x1C98 (4)   FIFO
                                                                                // ClipperYBottomBoundary (5)   WO 0x1C9C (4)   FIFO
                                                                                // ClipperXLeftBoundary (5)     WO 0x1CA0 (4)   FIFO
                                                                                // ClipperXRightBoundary (5)    WO 0x1CA4 (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, MemoryPitch);                             // MemoryPitch (5)              WO 0x1C8C (4)   FIFO
        IMPORTNAMESPACE(Shared::MMIO, MemoryOrigin);                            // MemoryOrigin (5)             WO 0x1C94 (4)   FIFO

        IMPORTNAMESPACE(Shared::MMIO, Status);                                  // Status                       RO 0x1E14

        // DataALU0                     WO 0x1CC0 (4)   FIFO
        // DataALU1 (Reserved)          WO 0x1CC4 (4)   FIFO
        // DataALU2                     WO 0x1CC8 (4)   FIFO
        // DataALU3                     WO 0x1CCC (4)   FIFO
        // DataALU4                     WO 0x1CD0 (4)   FIFO
        // DataALU5 (Reserved)          WO 0x1CD4 (4)   FIFO
        // DataALU6                     WO 0x1CD8 (4)   FIFO
        // DataALU7                     WO 0x1CDC (4)   FIFO
        // DataALU8                     WO 0x1CE0 (4)   FIFO
        // DataALU9 (Reserved)          WO 0x1CE4 (4)   FIFO
        // DataALU10                    WO 0x1CE8 (4)   FIFO
        // DataALU11                    WO 0x1CEC (4)   FIFO
        // DataALU12                    WO 0x1CF0 (4)   FIFO
        // DataALU13 (Reserved)         WO 0x1CF4 (4)   FIFO
        // DataALU14                    WO 0x1CF8 (4)   FIFO
        // DataALU15                    WO 0x1CFC (4)   FIFO

        // BusFIFOStatus                RO 0x1E10
        // InterruptClear               WO 0x1E18
        // InterruptEnable              RW 0x1E1C
        // VerticalCount                RO 0x1E20
        // DMAMap30                     RW 0x1E30
        // DMAMap74                     RW 0x1E34
        // DMAMapB8                     RW 0x1E38
        // DMAMapFC                     RW 0x1E3C
        // Reset                        RW 0x1E40
        // OperatingMode                RW 0x1E54
        // DrawingRegIndirectWrite0     WO 0x1E80 (4)
        // DrawingRegIndirectWrite1     WO 0x1E84 (4)
        // DrawingRegIndirectWrite2     WO 0x1E88 (4)
        // DrawingRegIndirectWrite3     WO 0x1E8C (4)
        // DrawingRegIndirectWrite4     WO 0x1E90 (4)
        // DrawingRegIndirectWrite5     WO 0x1E94 (4)
        // DrawingRegIndirectWrite6     WO 0x1E98 (4)
        // DrawingRegIndirectWrite7     WO 0x1E9C (4)
        // DrawingRegIndirectWrite8     WO 0x1EA0 (4)
        // DrawingRegIndirectWrite9     WO 0x1EA4 (4)
        // DrawingRegIndirectWrite10    WO 0x1EA8 (4)
        // DrawingRegIndirectWrite11    WO 0x1EAC (4)
        // DrawingRegIndirectWrite12    WO 0x1EB0 (4)
        // DrawingRegIndirectWrite13    WO 0x1EB4 (4)
        // DrawingRegIndirectWrite14    WO 0x1EB8 (4)
        // DrawingRegIndirectWrite15    WO 0x1EBC (4)
        // AttributeIndex               RW 0x1FC0       0x3C0
        // AttributeData                WO 0x1FC0       0x3C0
        // AttributeData                RO 0x1FC1       0x3C1
        // InputStatus0                 RO 0x1FC2       0x3C2
        // MiscellaneousOutputW         WO 0x1FC2       0x3C2
        // SequencerIndex               RW 0x1FC4       0x3C4
        // SequencerData                RW 0x1FC5       0x3C5
        // DACStatus                    RO 0x1FC7       0x3C7
        // FeatureControlR              RO 0x1FCA       0x3CA
        // MiscellaneousOutputR         RO 0x1FCC       0x3CC
        // GraphicsControllerIndex      RW 0x1FCE       0x3CE
        // GraphicsControllerData       RW 0x1FCF       0x3CF
        // CRTCIndex                    RW 0x1FD4       0x3D4 (or 0x3B4 (8))
        // CRTCData                     RW 0x1FD5       0x3D5 (or 0x3B5 (8))
        // InputStatus1                 RO 0x1FDA       0x3DA (or 0x3BA (8))
        // FeatureControlW              WO 0x1FDA       0x3DA (or 0x3BA (8))
        // CRTCExtensionIndex           RW 0x1FDE       0x3DE
        // CRTCExtensionData            RW 0x1FDF       0x3DF
        // CacheFlush                   RW 0x1FFF
        // RAMDACWriteIndex             RW 0x3C00       0x3C8
        // RAMDACData                   RW 0x3C01       0x3C9
        // RAMDACPixelReadMask          RW 0x3C02       0x3C6
        // RAMDACReadIndex              RW 0x3C03       0x3C7
        // IndexedDataRegister          RW 0x3C0A
        // CursorPositionXLow           RW 0x3C0C
        // CursorPositionXHigh          RW 0x3C0D
        // CursorPositionYLow           RW 0x3C0E
        // CursorPositionYHigh          RW 0x3C0F

        // (1) The Memory Address for the direct access registers is a byte address offset from MGABASE1.
        // (2) I/O space accesses are decoded only if VGA emulation is active (see the OPTION configuration register) and
        //     iospace = 1 (see the DEVCTRL configuration register).
        // (3) The memory controller may become idle after the data processor; therefore, we recommend that all other drawing
        //     registers be initialized before these registers in order to maximize performance.
        // (4) Reads of these locations are not decoded.
        // (5) Since the address processor can become idle before the data processor, we recommend that you initialize these
        //     registers first, in order to take advantage of this idle time.
        //     (6) Accessing a register in this range instructs the drawing engine to start a drawing cycle.
        // (7) Word or dword accesses to these specific reserved locations will be decoded. (The PCI convention states that
        //     I/O space should only be accessed in bytes, and that a bridge will not perform byte packing.)
        // (8) VGA I/O addresses in the 3DXh range are for CGA emulation (the MISC<0> register (ioaddsel field) is ‘1’).
        //     VGA I/O addresses in the 3BXh range are for monochrome (MDA) emulation (the ioaddsel field is ‘0’).
        //     Exception: for CRTCEXT, the 3BEh and 3BFh I/O addresses are reserved, not decoded.

    }

}