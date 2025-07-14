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

#include <hag/drivers/3dfx/shared/io/dacmode.h>
#include <hag/drivers/3dfx/shared/io/drminit0.h>
#include <hag/drivers/3dfx/shared/io/drminit1.h>
#include <hag/drivers/3dfx/shared/io/pllctrl0.h>
#include <hag/drivers/3dfx/shared/io/pllctrl1.h>
#include <hag/drivers/3dfx/shared/io/pllctrl2.h>
#include <hag/drivers/3dfx/shared/io/vgainit0.h>
#include <hag/drivers/3dfx/shared/io/vgainit1.h>
#include <hag/drivers/3dfx/shared/io/viddostr.h>
#include <hag/drivers/3dfx/shared/io/viddsa.h>
#include <hag/drivers/3dfx/shared/io/vidprcfg.h>
#include <hag/drivers/3dfx/shared/io/vidscrs.h>

#include <hag/drivers/3dfx/shared/mmio2d/status.h>
#include <hag/drivers/3dfx/shared/mmio2d/intctrl.h>
#include <hag/drivers/3dfx/shared/mmio2d/baseaddr.h>
#include <hag/drivers/3dfx/shared/mmio2d/breserr.h>
#include <hag/drivers/3dfx/shared/mmio2d/clip.h>
#include <hag/drivers/3dfx/shared/mmio2d/cmd.h>
#include <hag/drivers/3dfx/shared/mmio2d/cmdextra.h>
#include <hag/drivers/3dfx/shared/mmio2d/colkey.h>
#include <hag/drivers/3dfx/shared/mmio2d/color.h>
#include <hag/drivers/3dfx/shared/mmio2d/dstfmt.h>
#include <hag/drivers/3dfx/shared/mmio2d/lncharea.h>
#include <hag/drivers/3dfx/shared/mmio2d/lnstl.h>
#include <hag/drivers/3dfx/shared/mmio2d/lnstp.h>
#include <hag/drivers/3dfx/shared/mmio2d/pattern.h>
#include <hag/drivers/3dfx/shared/mmio2d/rop.h>
#include <hag/drivers/3dfx/shared/mmio2d/size.h>
#include <hag/drivers/3dfx/shared/mmio2d/srcfmt.h>
#include <hag/drivers/3dfx/shared/mmio2d/xy.h>

#include <hag/drivers/3dfx/shared/mmio3d/alpham.h>
#include <hag/drivers/3dfx/shared/mmio3d/buffaddr.h>
#include <hag/drivers/3dfx/shared/mmio3d/buffstrd.h>
#include <hag/drivers/3dfx/shared/mmio3d/chrkey.h>
#include <hag/drivers/3dfx/shared/mmio3d/chrrng.h>
#include <hag/drivers/3dfx/shared/mmio3d/clip.h>
#include <hag/drivers/3dfx/shared/mmio3d/cmd.h>
#include <hag/drivers/3dfx/shared/mmio3d/color.h>
#include <hag/drivers/3dfx/shared/mmio3d/colpth.h>
#include <hag/drivers/3dfx/shared/mmio3d/delta.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbiaff.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbichrf.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbipi.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbipo.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbiswph.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbitrio.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbizff.h>
#include <hag/drivers/3dfx/shared/mmio3d/fbzm.h>
#include <hag/drivers/3dfx/shared/mmio3d/fogcol.h>
#include <hag/drivers/3dfx/shared/mmio3d/fogm.h>
#include <hag/drivers/3dfx/shared/mmio3d/fogtable.h>
#include <hag/drivers/3dfx/shared/mmio3d/intctrl.h>
#include <hag/drivers/3dfx/shared/mmio3d/lfbm.h>
#include <hag/drivers/3dfx/shared/mmio3d/ncctbl.h>
#include <hag/drivers/3dfx/shared/mmio3d/ovlbuf.h>
#include <hag/drivers/3dfx/shared/mmio3d/ssetupm.h>
#include <hag/drivers/3dfx/shared/mmio3d/start.h>
#include <hag/drivers/3dfx/shared/mmio3d/status.h>
#include <hag/drivers/3dfx/shared/mmio3d/stipple.h>
#include <hag/drivers/3dfx/shared/mmio3d/svertex.h>
#include <hag/drivers/3dfx/shared/mmio3d/swpbfpnd.h>
#include <hag/drivers/3dfx/shared/mmio3d/tbaseadr.h>
#include <hag/drivers/3dfx/shared/mmio3d/tdetail.h>
#include <hag/drivers/3dfx/shared/mmio3d/tlod.h>
#include <hag/drivers/3dfx/shared/mmio3d/trxinit1.h>
#include <hag/drivers/3dfx/shared/mmio3d/txtrm.h>
#include <hag/drivers/3dfx/shared/mmio3d/vertex.h>
#include <hag/drivers/3dfx/shared/mmio3d/zacol.h>

#include <hag/drivers/3dfx/shared/fifo/agpgaddr.h>
#include <hag/drivers/3dfx/shared/fifo/agpgstrd.h>
#include <hag/drivers/3dfx/shared/fifo/agphadh.h>
#include <hag/drivers/3dfx/shared/fifo/agphadl.h>
#include <hag/drivers/3dfx/shared/fifo/agpmvcmd.h>
#include <hag/drivers/3dfx/shared/fifo/agprqsz.h>
#include <hag/drivers/3dfx/shared/fifo/cmdamax.h>
#include <hag/drivers/3dfx/shared/fifo/cmdamin.h>
#include <hag/drivers/3dfx/shared/fifo/cmdbaddr.h>
#include <hag/drivers/3dfx/shared/fifo/cmdbsize.h>
#include <hag/drivers/3dfx/shared/fifo/cmdbump.h>
#include <hag/drivers/3dfx/shared/fifo/cmdfdpth.h>
#include <hag/drivers/3dfx/shared/fifo/cmdfthrs.h>
#include <hag/drivers/3dfx/shared/fifo/cmdhcnt.h>
#include <hag/drivers/3dfx/shared/fifo/cmdhint.h>
#include <hag/drivers/3dfx/shared/fifo/cmdrdpth.h>
#include <hag/drivers/3dfx/shared/fifo/cmdrdptl.h>
#include <hag/drivers/3dfx/shared/fifo/pck0.h>
#include <hag/drivers/3dfx/shared/fifo/pck1.h>
#include <hag/drivers/3dfx/shared/fifo/pck2.h>
#include <hag/drivers/3dfx/shared/fifo/pck3.h>
#include <hag/drivers/3dfx/shared/fifo/pck4.h>
#include <hag/drivers/3dfx/shared/fifo/pck5.h>
#include <hag/drivers/3dfx/shared/fifo/pck6.h>
#include <hag/drivers/3dfx/shared/fifo/yuvbaddr.h>
#include <hag/drivers/3dfx/shared/fifo/yuvstride.h>
#include <hag/drivers/3dfx/shared/fifo/fifodrct.h>
#include <hag/drivers/3dfx/shared/fifo/fifodefr.h>
#include <hag/drivers/3dfx/shared/fifo/fifofunc.h>

#include <hag/drivers/vga/modeset.h>

namespace Hag::TDfx::Banshee
{
    HAG_IMPORT_MODESETTING;

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

    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, MiscellaneousOutput);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, InputStatus0);
    IMPORTNAMESPACEANDTYPE(VGA, VideoSubsystemEnable);

    IMPORTNAMESPACE(VGA, SequencerIndex);
    IMPORTNAMESPACEANDTYPE(VGA, SequencerData);
    
    namespace Sequencer
    {
        IMPORTNAMESPACE(VGA::Sequencer, Register);
    
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, Reset);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, ClockingMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, EnableWritePlane);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, CharacterFontSelect);
        IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, MemoryModeControl);
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

    namespace IO
    {
        IMPORTNAMESPACE(Shared::IO, DACMode);
        IMPORTNAMESPACE(Shared::IO, DRAMInit0);
        IMPORTNAMESPACE(Shared::IO, DRAMInit1);
        IMPORTNAMESPACE(Shared::IO, PLLControl0);
        IMPORTNAMESPACE(Shared::IO, PLLControl1);
        IMPORTNAMESPACE(Shared::IO, PLLControl2);
        IMPORTNAMESPACE(Shared::IO, VGAInit0);
        IMPORTNAMESPACE(Shared::IO, VGAInit1);
        IMPORTNAMESPACE(Shared::IO, VideoDesktopOverlayStride);
        IMPORTNAMESPACE(Shared::IO, VideoDesktopStartAddress);
        IMPORTNAMESPACE(Shared::IO, VideoProcessorConfiguration);
        IMPORTNAMESPACE(Shared::IO, VideoScreenSize);
    }

    namespace TwoD
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Status);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InterruptControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, BaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, BresenhamError);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Clip);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Command);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, CommandExtra);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ColorKey);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Color);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, DestinationFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineStyle);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineStipple);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Pattern);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, RasterOperation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Size);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, SourceFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, XY);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ScreenBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ScreenStretchBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, HostBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, HostStretchBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, RectangleFillLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, PolyLineLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, PolygonLaunch);

        IMPORTNAMESPACE(Shared::TwoD, Rop);
        typedef Shared::TwoD::LaunchCommand_t LaunchCommand_t;
    }

    namespace ThreeD
    {
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, Status);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InterruptControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, BufferAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, BufferStride);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, ChromaRange);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, ClipLeftRight);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, ClipTopBottom);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, ClipLeftRight1);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, ClipTopBottom1);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, Triangle);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, Nop);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, FastFill);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, SwapBuffer);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, UserInterrupt);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, DrawTriangle);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD::Command, BeginTriangle);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Color);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbzColorPath);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Delta12p12);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Delta20p12);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Delta14p18);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Delta2p30);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiAFuncFail);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiChromaFail);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiPixelsIn);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiPixelsOut);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiSwapHistory);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiTrianglesOut);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FbiZFuncFail);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, RGBDepthMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FogColor);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FogMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, FogTable);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, LinearFrameBufferMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, NccTable);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, OverlayBufferAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, sSetupMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Start12p12);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Start14p18);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Start2p30);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Stipple);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, sARGB);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, SwapBufferPending);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, TextureBaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, TextureDetail);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, TextureLevelOfDetail);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, TrexInit1);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, TextureMode);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, Vertex);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::ThreeD, DepthAlphaConstant);
    }

    namespace MMIO2D
    {
        IMPORTNAMESPACE(Shared::MMIO2D, Status);
        IMPORTNAMESPACE(Shared::MMIO2D, InterruptControl);
        IMPORTNAMESPACE(Shared::MMIO2D, BaseAddress);
        IMPORTNAMESPACE(Shared::MMIO2D, BresenhamError);
        IMPORTNAMESPACE(Shared::MMIO2D, Clip);
        IMPORTNAMESPACE(Shared::MMIO2D, Command);
        IMPORTNAMESPACE(Shared::MMIO2D, CommandExtra);
        IMPORTNAMESPACE(Shared::MMIO2D, ColorKey);
        IMPORTNAMESPACE(Shared::MMIO2D, Color);
        IMPORTNAMESPACE(Shared::MMIO2D, DestinationFormat);
        IMPORTNAMESPACE(Shared::MMIO2D, LineStyle);
        IMPORTNAMESPACE(Shared::MMIO2D, LineStipple);
        IMPORTNAMESPACE(Shared::MMIO2D, Pattern);
        IMPORTNAMESPACE(Shared::MMIO2D, RasterOperation);
        IMPORTNAMESPACE(Shared::MMIO2D, Size);
        IMPORTNAMESPACE(Shared::MMIO2D, SourceFormat);
        IMPORTNAMESPACE(Shared::MMIO2D, XY);
        IMPORTNAMESPACE(Shared::MMIO2D, LaunchArea);
    }
    
    namespace MMIO3D
    {
        IMPORTNAMESPACE(Shared::MMIO3D, Status);
        IMPORTNAMESPACE(Shared::MMIO3D, InterruptControl);
        IMPORTNAMESPACE(Shared::MMIO3D, AlphaMode);
        IMPORTNAMESPACE(Shared::MMIO3D, BufferAddress);
        IMPORTNAMESPACE(Shared::MMIO3D, BufferStride);
        IMPORTNAMESPACE(Shared::MMIO3D, ChromaKey);
        IMPORTNAMESPACE(Shared::MMIO3D, ChromaRange);
        IMPORTNAMESPACE(Shared::MMIO3D, Clip);
        IMPORTNAMESPACE(Shared::MMIO3D, Command);
        IMPORTNAMESPACE(Shared::MMIO3D, Color);
        IMPORTNAMESPACE(Shared::MMIO3D, FbzColorPath);
        IMPORTNAMESPACE(Shared::MMIO3D, Delta);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiAFuncFail);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiChromaFail);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiPixelsIn);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiPixelsOut);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiSwapHistory);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiTrianglesOut);
        IMPORTNAMESPACE(Shared::MMIO3D, FbiZFuncFail);
        IMPORTNAMESPACE(Shared::MMIO3D, RGBDepthMode);
        IMPORTNAMESPACE(Shared::MMIO3D, FogColor);
        IMPORTNAMESPACE(Shared::MMIO3D, FogMode);
        IMPORTNAMESPACE(Shared::MMIO3D, FogTable);
        IMPORTNAMESPACE(Shared::MMIO3D, LinearFrameBufferMode);
        IMPORTNAMESPACE(Shared::MMIO3D, NccTable);
        IMPORTNAMESPACE(Shared::MMIO3D, OverlayBufferAddress);
        IMPORTNAMESPACE(Shared::MMIO3D, sSetupMode);
        IMPORTNAMESPACE(Shared::MMIO3D, Start);
        IMPORTNAMESPACE(Shared::MMIO3D, Stipple);
        IMPORTNAMESPACE(Shared::MMIO3D, sVertex);
        IMPORTNAMESPACE(Shared::MMIO3D, SwapBufferPending);
        IMPORTNAMESPACE(Shared::MMIO3D, TextureBaseAddress);
        IMPORTNAMESPACE(Shared::MMIO3D, TextureDetail);
        IMPORTNAMESPACE(Shared::MMIO3D, TextureLevelOfDetail);
        IMPORTNAMESPACE(Shared::MMIO3D, TrexInit1);
        IMPORTNAMESPACE(Shared::MMIO3D, TextureMode);
        IMPORTNAMESPACE(Shared::MMIO3D, Vertex);
        IMPORTNAMESPACE(Shared::MMIO3D, DepthAlphaConstant);
    }

    namespace Fifo
    {
        typedef Shared::Fifo::Command_t Command_t;
        
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPGraphicsAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPGraphicsStride);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPHostAddressHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPHostAddressLow);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPMoveCommand);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, AGPRequestSize);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandAMax);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandAMin);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandBaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandBaseSize);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandBump);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandFifoDepth);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandFifoFetchThreshhold);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandHoleCount);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandHoleInterrupt);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandReadPointerHigh);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, CommandReadPointerLow);
        IMPORTNAMESPACE(Shared::Fifo, Packet0);
        IMPORTNAMESPACE(Shared::Fifo, Packet1);
        IMPORTNAMESPACE(Shared::Fifo, Packet2);
        IMPORTNAMESPACE(Shared::Fifo, Packet3);
        IMPORTNAMESPACE(Shared::Fifo, Packet4);
        IMPORTNAMESPACE(Shared::Fifo, Packet5);
        IMPORTNAMESPACE(Shared::Fifo, Packet6);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, YUVBaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Fifo, YUVStride);

        namespace Direct
        {
            IMPORTNAMESPACE(Shared::Fifo::Direct, TwoD);
            IMPORTNAMESPACE(Shared::Fifo::Direct, ThreeD);
        }

        namespace Deferred
        {
            IMPORTNAMESPACE(Shared::Fifo::Deferred, TwoD);
        }

        using namespace Shared::Fifo::Function;
    }

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

//2D Register Map
//
// All 2D registers can be read, and all registers except for the status register are fully write-able. Reading a
// 2D register will always return the value that will be used if a new operation is begun without writing a
// new value to that register. This value will either be the last value written to the register, or, if an operation
// has been performed since the value was written, the value after all operations have completed.
// All registers for the 2D section are unsigned unless specified otherwise.
//
// Memory Base 0: Offset 0x0100000
//
// Register Name        Address     Reg     Bits    R/W     Description
//--------------------------------------------------------------------------------------------------------------------
// status               0x000(0)    0x0     31:0    R       Banshee status register
// intCtrl              0x004(4)    0x1     31:0    R/W     Interrupt control and status
// clip0Min             0x008(8)    0x2     28:0    R/W     Min X & Y clip values when clip select is 0
// clip0Max             0x00c(12)   0x3     28:0    R/W     Max X & Y clip values when clip select is 0
// dstBaseAddr          0x010(16)   0x4     23:0    R/W     Destination base address
// dstFormat            0x014(20)   0x5     17:0    R/W     Destination stride and bits per pixel
// srcColorkeyMin       0x018(24)   0x6     23:0    R/W     Source Colorkey range (min)
// srcColorkeyMax       0x01c(28)   0x7     23:0    R/W     Source Colorkey range (max)
// dstColorkeyMin       0x020(32)   0x8     23:0    R/W     Destination Colorkey range (min)
// dstColorkeyMax       0x024(36)   0x9     23:0    R/W     Destination Colorkey range (max)
// bresError0           0x028(40)   0xA     31:0    R/W     Initial error for lines, right edges & stretch blt x
// bresError1           0x02c(44)   0xB     31:0    R/W     Initial error for left poly edges & stretch blt y
// rop                  0x030(48)   0xC     31:0    R/W     4 Ternary Raster operations
// srcBaseAddr          0x034(52)   0xD     23:0    R/W     Source base address
// commandExtra         0x038(56)   0xE     31:0    R/W     Extra control bits
// lineStipple          0x03c(60)   0xF     31:0    R/W     Monochrome pattern for lines
// lineStyle            0x040(64)   0x10    28:0    R/W     Style register for lines
// pattern0Alias        0x044(68)   0x11    31:0    R/W     Alias to colorPattern(0)
// pattern1Alias        0x048(72)   0x12    31:0    R/W     Alias to colorPattern(1)
// clip1Min             0x04c(76)   0x13    28:0    R/W     Min X & Y clip values when clip select is 1
// clip1Max             0x050(80)   0x14    28:0    R/W     Max X & Y clip values when clip select is 1
// srcFormat            0x054(84)   0x15    18:0    R/W     Source stride and bits per pixel
// srcSize              0x058(88)   0x16    28:0    R/W     Height and width of source for stretch blts
// srcXY                0x05c(92)   0x17    28:0    R/W     Starting pixel of blt source data
//                                                          Starting position for lines
//                                                          Top-most point for a polygon fill
// colorBack            0x060(96)   0x18    31:0    R/W     Background color
// colorFore            0x064(100)  0x19    31:0    R/W     Foreground color
// dstSize              0x068(104)  0x1A    28:0    R/W     Destination width and height for blts and
//                                                          rectangle fills
// dstXY                0x06c(108)  0x1B    28:0    R/W     Starting X and Y of destination for blts
//                                                          End point for lines
// command              0x070(112)  0x1C    31:0    R/W     2D command mode & control bits
// RESERVED             0x074(116)  0x1D    31:0            Do not write
// RESERVED             0x078(120)  0x1E    31:0            Do not write
// RESERVED             0x07c(124)  0x1F    31:0            Do not write
// launchArea           0x080(128)  0x20    31:0    R       Initiates 2D commands
//                      to          to
//                      0x0ff(255)  0x3F
// colorPattern         0x100(256)  0x40    31:0    R/W     Pattern Registers (64 entries)
//                      to          to
//                      0x1fc(508)  


// 3D Memory Mapped Register Set

// A 4Mbyte (22-bit) FBI memory mapped register address is divided into the following fields:

// +--------+---------+--------+--------+----------+------+
// | AltMap | Swizzle |  Wrap  |  Chip  | Register | Byte |
// +--------+---------+--------+--------+----------+------+
// |   21   |   20    | 19  14 | 13  10 | 9      2 | 1  0 |
// +--------+---------+--------+--------+----------+------+

// The chip field selects one or more of the Banshee chips (FBI and/or TREX) to be accessed. Each bit in
// the chip field selects one chip for writing, with FBI controlled by the lsb of the chip field, and TREX#2
// controlled by the msb of the chip field. Note the chip field value of 0x0 selects all chips. The following
// table shows the chip field mappings: The current generation of Banshee only supports 1 TREX, so only
// bits 1:0 are valid, future generation of Banshee will support additional TREX cores.

// Chip Field      SST-1 Chip Accessed
// 0000            FBI + all TREX chips
// 0001            FBI
// 0010            TREX #0
// 0011            FBI + TREX #0
// 0100            TREX #1
// 0101            FBI + TREX #1
// 0110            TREX #0 + TREX #1
// 0111            FBI + TREX #0 + TREX #1
// 1000            TREX #2
// 1001            FBI + TREX #2
// 1010            TREX #0 + TREX #2
// 1011            FBI + TREX #0 + TREX #2
// 1100            TREX #1 + TREX #2
// 1101            FBI + TREX #1 + TREX #2
// 1110            TREX #0 + TREX #1 + TREX #2
// 1111            FBI + all TREX chips


// Note that TREX #0 is always connected to FBI in the system level diagrams of section 3, and TREX #1 is
// attached to TREX #0, etc. By utilizing the different chip fields, software can precisely control the data
// presented to individual chips which compose the Banshee graphics subsystem. Note that for reads, the
// chip field is ignored, and read data is always read from FBI. The register field selects the register to be
// accessed from the table below. All accesses to the memory mapped registers must be 32-bit accesses. No
// byte (8-bit) or halfword (16-bit) accesses are allowed to the memory mapped registers, so the byte (2-bit)
// field of all memory mapped register accesses must be 0x0. As a result, to modify individual bits of a 32-
// bit register, the entire 32-bit word must be written with valid bits in all positions.

// The table below shows the Banshee register set. The register set shown below is the address map when
// triangle registers address aliasing (remapping) is disabled(fbiinit3(0)=0). When The chip column
// illustrates which registers are stored in which chips. For the registers which are stored in TREX, the %
// symbol specifies that the register is unconditionally written to TREX regardless of the chip address.
// Similarly, the * symbol specifies that the register is only written to a given TREX if specified in the chip
// address. The R/W column illustrates the read/write status of individual registers. Reading from a register
// which is “write only” returns undefined data. Also, reading from a register that is TREX specific returns
// undefined data. Reads from all other memory mapped registers only contain valid data in the bits stored
// by the registers, and undefined/reserved bits in a given register must be masked by software. The sync
// column indicates whether the graphics processor must wait for the current command to finish before
// loading a particular register from the FIFO. A “yes” in the sync column means the graphics processor
// will flush the data pipeline before loading the register -- this will result in a small performance
// degradation when compared to those registers which do not need synchronization. The FIFO column
// indicates whether a write to a particular register will be pushed into the PCI bus FIFO. Care must be
// taken when writing to those registers not pushed into the FIFO in order to prevent race conditions between
// FIFOed and non-FIFOed registers. Also note that reads are not pushed into the PCI bus FIFO, and
// reading FIFOed registers will return the current value of the register, irrespective of pending writes to the
// register present in the FIFO.


// Memory Base 0: Offset 0x0200000

// Register Name   Address     Reg     Bits    Chip        R/W     Sync?       Description
//                             Num                                 /Fifo?
// status          0x000(0)    0x0     31:0    FBI         R        No / n/a   Banshee Status
// intrCtrl        0x004(4)    0x1     31:0    FBI         R/W      No / No    Interrupt Status and Control
// vertexAx        0x008(8)    0x2     15:0    FBI+TREX%     W      No / Yes   Vertex A x-coordinate location (12.4 format)
// vertexAy        0x00c(12)   0x3     15:0    FBI+TREX%     W      No / Yes   Vertex A y-coordinate location (12.4 format)
// vertexBx        0x010(16)   0x4     15:0    FBI+TREX%     W      No / Yes   Vertex B x-coordinate location (12.4 format)
// vertexBy        0x014(20)   0x5     15:0    FBI+TREX%     W      No / Yes   Vertex B y-coordinate location (12.4 format)
// vertexCx        0x018(24)   0x6     15:0    FBI+TREX%     W      No / Yes   Vertex C x-coordinate location (12.4 format)
// vertexCy        0x01c(28)   0x7     15:0    FBI+TREX%     W      No / Yes   Vertex C y-coordinate location (12.4 format)
// startR          0x020(32)   0x8     23:0    FBI           W      No / Yes   Starting Red parameter (12.12 format)
// startG          0x024(36)   0x9     23:0    FBI           W      No / Yes   Starting Green parameter (12.12 format)
// startB          0x028(40)   0xA     23:0    FBI           W      No / Yes   Starting Blue parameter (12.12 format)
// startZ          0x02c(44)   0xB     31:0    FBI           W      No / Yes   Starting Z parameter (20.12 format)
// startA          0x030(48)   0xC     23:0    FBI           W      No / Yes   Starting Alpha parameter (12.12 format)
// startS          0x034(52)   0xD     31:0    TREX*         W      No / Yes   Starting S/W parameter (14.18 format)
// startT          0x038(56)   0xE     31:0    TREX*         W      No / Yes   Starting T/W parameter (14.18 format)
// startW          0x03c(60)   0xF     31:0    FBI+TREX*     W      No / Yes   Starting 1/W parameter (2.30 format)
// dRdX            0x040(64)   0x10    23:0    FBI           W      No / Yes   Change in Red with respect to X (12.12 format)
// dGdX            0x044(68)   0x11    23:0    FBI           W      No / Yes   Change in Green with respect to X (12.12 format)
// dBdX            0x048(72)   0x12    23:0    FBI           W      No / Yes   Change in Blue with respect to X (12.12 format)
// dZdX            0x04c(76)   0x13    31:0    FBI           W      No / Yes   Change in Z with respect to X (20.12 format)
// dAdX            0x050(80)   0x14    23:0    FBI           W      No / Yes   Change in Alpha with respect to X (12.12 format)
// dSdX            0x054(84)   0x15    31:0    TREX*         W      No / Yes   Change in S/W with respect to X (14.18 format)
// dTdX            0x058(88)   0x16    31:0    TREX*         W      No / Yes   Change in T/W with respect to X (14.18 format)
// dWdX            0x05c(92)   0x17    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to X (2.30 format)
// dRdY            0x060(96)   0x18    23:0    FBI           W      No / Yes   Change in Red with respect to Y (12.12 format)
// dGdY            0x064(100)  0x19    23:0    FBI           W      No / Yes   Change in Green with respect to Y (12.12 format)
// dBdY            0x068(104)  0x1A    23:0    FBI           W      No / Yes   Change in Blue with respect to Y (12.12 format)
// dZdY            0x06c(108)  0x1B    31:0    FBI           W      No / Yes   Change in Z with respect to Y (20.12 format)
// dAdY            0x070(112)  0x1C    23:0    FBI           W      No / Yes   Change in Alpha with respect to Y (12.12 format)
// dSdY            0x074(116)  0x1D    31:0    TREX*         W      No / Yes   Change in S/W with respect to Y (14.18 format)
// dTdY            0x078(120)  0x1E    31:0    TREX*         W      No / Yes   Change in T/W with respect to Y (14.18 format)
// dWdY            0x07c(124)  0x1F    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to Y (2.30 format)
// triangleCMD     0x080(128)  0x20    31      FBI+TREX%     W      No / Yes   Execute TRIANGLE command (floating point)
// reserved        0x084(132)  0x21    n/a     n/a           W        n/a
// fvertexAx       0x088(136)  0x22    31:0    FBI+TREX%     W      No / Yes   Vertex A x-coordinate location (floating point)
// fvertexAy       0x08c(140)  0x23    31:0    FBI+TREX%     W      No / Yes   Vertex A y-coordinate location (floating point)
// fvertexBx       0x090(144)  0x24    31:0    FBI+TREX%     W      No / Yes   Vertex B x-coordinate location (floating point)
// fvertexBy       0x094(148)  0x25    31:0    FBI+TREX%     W      No / Yes   Vertex B y-coordinate location (floating point)
// fvertexCx       0x098(152)  0x26    31:0    FBI+TREX%     W      No / Yes   Vertex C x-coordinate location (floating point)
// fvertexCy       0x09c(156)  0x27    31:0    FBI+TREX%     W      No / Yes   Vertex C y-coordinate location (floating point)
// fstartR         0x0a0(160)  0x28    31:0    FBI           W      No / Yes   Starting Red parameter (floating point)
// fstartG         0x0a4(164)  0x29    31:0    FBI           W      No / Yes   Starting Green parameter (floating point)
// fstartB         0x0a8(168)  0x2A    31:0    FBI           W      No / Yes   Starting Blue parameter (floating point)
// fstartZ         0x0ac(172)  0x2B    31:0    FBI           W      No / Yes   Starting Z parameter (floating point)
// fstartA         0x0b0(176)  0x2C    31:0    FBI           W      No / Yes   Starting Alpha parameter (floating point)
// fstartS         0x0b4(180)  0x2D    31:0    TREX*         W      No / Yes   Starting S/W parameter (floating point)
// fstartT         0x0b8(184)  0x2E    31:0    TREX*         W      No / Yes   Starting T/W parameter (floating point)
// fstartW         0x0bc(188)  0x2F    31:0    FBI+TREX*     W      No / Yes   Starting 1/W parameter (floating point)
// fdRdX           0x0c0(192)  0x30    31:0    FBI           W      No / Yes   Change in Red with respect to X (floating point)
// fdGdX           0x0c4(196)  0x31    31:0    FBI           W      No / Yes   Change in Green with respect to X (floating point)
// fdBdX           0x0c8(200)  0x32    31:0    FBI           W      No / Yes   Change in Blue with respect to X (floating point)
// fdZdX           0x0cc(204)  0x33    31:0    FBI           W      No / Yes   Change in Z with respect to X (floating point)
// fdAdX           0x0d0(208)  0x34    31:0    FBI           W      No / Yes   Change in Alpha with respect to X (floating point)
// fdSdX           0x0d4(212)  0x35    31:0    TREX*         W      No / Yes   Change in S/W with respect to X (floating point)
// fdTdX           0x0d8(216)  0x36    31:0    TREX*         W      No / Yes   Change in T/W with respect to X (floating point)
// fdWdX           0x0dc(220)  0x37    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to X (floating point)
// fdRdY           0x0e0(224)  0x38    31:0    FBI           W      No / Yes   Change in Red with respect to Y (floating point)
// fdGdY           0x0e4(228)  0x39    31:0    FBI           W      No / Yes   Change in Green with respect to Y (floating point)
// fdBdY           0x0e8(232)  0x3A    31:0    FBI           W      No / Yes   Change in Blue with respect to Y (floating point)
// fdZdY           0x0ec(236)  0x3B    31:0    FBI           W      No / Yes   Change in Z with respect to Y (floating point)
// fdAdY           0x0f0(240)  0x3C    31:0    FBI           W      No / Yes   Change in Alpha with respect to Y (floating point)
// fdSdY           0x0f4(244)  0x3D    31:0    TREX*         W      No / Yes   Change in S/W with respect to Y (floating point)
// fdTdY           0x0f8(248)  0x3E    31:0    TREX*         W      No / Yes   Change in T/W with respect to Y (floating point)
// fdWdY           0x0fc(252)  0x3F    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to Y (floating point)
// ftriangleCMD    0x100(256)  0x40    31      FBI+TREX%     W      No / Yes   Execute TRIANGLE command (floating point)
// fbzColorPath    0x104(260)  0x41    27:0    FBI+TREX%   R/W      No / Yes   FBI Color Path Control
// fogMode         0x108(264)  0x42     5:0    FBI         R/W      No / Yes   Fog Mode Control
// alphaMode       0x10c(268)  0x43    31:0    FBI         R/W      No / Yes   Alpha Mode Control
// fbzMode         0x110(272)  0x44    20:0    FBI         R/W     Yes / Yes   RGB Buffer and Depth-Buffer Control
// lfbMode         0x114(276)  0x45    16:0    FBI         R/W     Yes / Yes   Linear Frame Buffer Mode Control
// clipLeftRight   0x118(280)  0x46    31:0    FBI         R/W     Yes / Yes   Left and Right of Clipping Register
// clipTopBottom   0x11c(284)  0x47    31:0    FBI         R/W     Yes / Yes   Top and Bottom of Clipping Register
// nopCMD          0x120(288)  0x48       0    FBI+TREX%     W     Yes / Yes   Execute NOP command
// fastfillCMD     0x124(292)  0x49     n/a    FBI           W     Yes / Yes   Execute FASTFILL command
// swapbufferCMD   0x128(296)  0x4A     8:0    FBI           W     Yes / Yes   Execute SWAPBUFFER command
// fogColor        0x12c(300)  0x4B    23:0    FBI           W     Yes / Yes   Fog Color Value
// zaColor         0x130(304)  0x4C    31:0    FBI           W     Yes / Yes   Constant Alpha/Depth Value
// chromaKey       0x134(308)  0x4D    23:0    FBI           W     Yes / Yes   Chroma Key Compare Value
// chromaRange     0x138(312)  0x4E    27:0    FBI           W     Yes / Yes   Chroma Range Compare Values, Modes & Enable
// userIntrCMD     0x13c(316)  0x4F     9:0    FBI           W     Yes / Yes   Execute USERINTERRUPT command
// stipple         0x140(320)  0x50    31:0    FBI         R/W     Yes / Yes   Rendering Stipple Value
// color0          0x144(324)  0x51    31:0    FBI         R/W     Yes / Yes   Constant Color #0
// color1          0x148(328)  0x52    31:0    FBI         R/W     Yes / Yes   Constant Color #1
// fbiPixelsIn     0x14c(332)  0x53    23:0    FBI         R          n/a      Pixel Counter (Number pixels processed)
// fbiChromaFail   0x150(336)  0x54    23:0    FBI         R          n/a      Pixel Counter (Number pixels failed Chroma test)
// fbiZfuncFail    0x154(340)  0x55    23:0    FBI         R          n/a      Pixel Counter (Number pixels failed Z test)
// fbiAfuncFail    0x158(344)  0x56    23:0    FBI         R          n/a      Pixel Counter (Number pixels failed Alpha test)
// fbiPixelsOut    0x15c(348)  0x57    23:0    FBI         R          n/a      Pixel Counter (Number pixels drawn)
// fogTable        0x160(352)  0x58    31:0    FBI         W       Yes / Yes   Fog Table
//                     to       to
//                 0x1dc(476)  0x77
// reserved        0x1e0(480)  0x78     n/a    n/a         n/a        n/a
//                     to       to
//                 0x1e8(488)  0x7A
// colBufferAddr   0x1ec(492)  0x7B    23:0    FBI         R/W     Yes / Yes   Color Buffer Base Address
// colBufferStride 0x1f0(496)  0x7C    15:0    FBI         R/W     Yes / Yes   Color Buffer Stride, Memory type
// auxBufferAddr   0x1f4(500)  0x7D    23:0    FBI         R/W     Yes / Yes   Auxiliary Buffer Base Address
// auxBufferStride 0x1f8(504)  0x7E    15:0    FBI         R/W     Yes / Yes   Auxiliary Buffer Stride, Memory type
// reserved        0x1fc(508)  0x7F    n/a     n/a         n/a        n/a
// clipLeftRight1  0x200(512)  0x80    31:0    FBI         R/W     Yes / Yes   Secondary Left/Right Clipping Register
// clipTopBottom1  0x204(516)  0x81    31:0    FBI         R/W     Yes / Yes   Secondary Top/Bottom Clipping Register
// reserved        0x208(520)  0x82     n/a    n/a         n/a        n/a
//                     to       to
//                 0x24b(587)  0x92
// swapPending     0x24c(588)  0x93     n/a    FBI           W      No / No    Swap buffer pending
// leftOverlayBuf  0x250(592)  0x94    27:0    FBI           W      No / Yes   Left Overlay address
// rightOverlayBuf 0x254(596)  0x95    27:0    FBI           W      No / Yes   Right Overlay address
// fbiSwapHistory  0x258(600)  0x96    31:0    FBI         R          n/a      Swap History Register
// fbiTrianglesOut 0x25c(604)  0x97    23:0    FBI         R          n/a      Triangle Counter (Number triangles drawn)
// sSetupMode      0x260(608)  0x98    19:0    FBI           W      No / Yes   Triangle setup mode
// sVx             0x264(612)  0x99    31:0    FBI+TMU*      W      No / Yes   Triangle setup X
// sVy             0x268(616)  0x9A    31:0    FBI+TMU*      W      No / Yes   Triangle setup Y
// sARGB           0x26c(620)  0x9B    31:0    FBI+TMU*      W      No / Yes   Triangle setup Alpha, Red, Green, Blue
// sRed            0x270(624)  0x9C    31:0    FBI           W      No / Yes   Triangle setup Red value
// sGreen          0x274(628)  0x9D    31:0    FBI           W      No / Yes   Triangle setup Green value
// sBlue           0x278(632)  0x9E    31:0    FBI           W      No / Yes   Triangle setup Blue value
// sAlpha          0x27c(636)  0x9F    31:0    FBI           W      No / Yes   Triangle setup Alpha value
// sVz             0x280(640)  0xA0    31:0    FBI           W      No / Yes   Triangle setup Z
// sWb             0x284(644)  0xA1    31:0    FBI+TMU*      W      No / Yes   Triangle setup Global W
// sWtmu0          0x288(648)  0xA2    31:0    TMU*          W      No / Yes   Triangle setup Tmu0 & Tmu1 W
// sS/W0           0x28c(652)  0xA3    31:0    TMU*          W      No / Yes   Triangle setup Tmu0 & Tmu1 S/W
// sT/W0           0x290(656)  0xA4    31:0    TMU*          W      No / Yes   Triangle setup Tmu0 & Tmu1 T/W
// sWtmu1          0x294(660)  0xA5    31:0    TMU1          W      No / Yes   Triangle setup Tmu1 only W
// sS/Wtmu1        0x298(664)  0xA6    31:0    TMU1          W      No / Yes   Triangle setup Tmu1 only S/W
// sT/Wtmu1        0x29c(668)  0xA7    31:0    TMU1          W      No / Yes   Triangle setup Tmu1 only T/W
// sDrawTriCMD     0x2a0(672)  0xA8    31:0    FBI+TMU*      W      No / Yes   Triangle setup (Draw)
// sBeginTriCMD    0x2a4(676)  0xA9    31:0    FBI           W      No / Yes   Triangle setup Start New triangle
// reserved        0x2a8(680)  0xAA     n/a    n/a         n/a        n/a
//                     to       to
//                 0x2fc(764)  0xBF
// textureMode     0x300(768)  0xC0    30:0    TREX*         W      No / Yes   Texture Mode Control
// tLOD            0x304(772)  0xC1    23:0    TREX*         W      No / Yes   Texture LOD Settings
// tDetail         0x308(776)  0xC2    21:0    TREX*         W      No / Yes   Texture Detail Settings
// texBaseAddr     0x30c(780)  0xC3    31:0    TREX*         W      No / Yes   Texture Base Address
// texBaseAddr_1   0x310(784)  0xC4    23:0    TREX*         W      No / Yes   Texture Base Address (supplemental LOD 1)
// texBaseAddr_2   0x314(788)  0xC5    23:0    TREX*         W      No / Yes   Texture Base Address (supplemental LOD 2)
// texBaseAddr_3_8 0x318(792)  0xC6    23:0    TREX*         W      No / Yes   Texture Base Address (supplemental LOD 3-8)
// reserved        0x31c(796)  0xC7     n/a    n/a         n/a        n/a
// trexInit1       0x320(800)  0xC8    31:0    TREX*         W     Yes / Yes   TREX Hardware Initialization (register 1
// nccTable0       0x324(804)  0xC9    31:0    TREX*         W     Yes / Yes   Narrow Channel Compression Table 0 (12 entries)
//                     to       to      or
//                 0x350(848)  0xD4    26:0
// nccTable1       0x354(852)  0xD5    31:0    TREX*         W     Yes / Yes   Narrow Channel Compression Table 1 (12 entries)
//                     to       to      or
//                 0x380(896)  0xE0    26:0
// reserved        0x384(900)  0xE1     n/a    n/a         n/a        n/a
//                     to       to
//                 0x3fc(1020) 0xFF

// The triangle parameter registers are aliased to a different address mapping to improve PCI bus
// throughput. The upper bit of the wrap field in the pci address is 0x1 (pci_ad[21]=1), the following table
// shows the addresses for the triangle parameter registers.

// Register Name   Address     Reg     Bits    Chip        R/W     Sync?       Description
//                             Num                                 /Fifo?
// status          0x000(0)    0x0     31:0    FBI         R/W      No / Yes   SST-1 Status
// intrCtrl        0x004(4)    0x1     19:0    FBI         R/W      No / No    Interrupt Status and Control
// vertexAx        0x008(8)    0x2     15:0    FBI+TREX%     W      No / Yes   Vertex A x-coordinate location (12.4 format)
// vertexAy        0x00c(12)   0x3     15:0    FBI+TREX%     W      No / Yes   Vertex A y-coordinate location (12.4 format)
// vertexBx        0x010(16)   0x4     15:0    FBI+TREX%     W      No / Yes   Vertex B x-coordinate location (12.4 format)
// vertexBy        0x014(20)   0x5     15:0    FBI+TREX%     W      No / Yes   Vertex B y-coordinate location (12.4 format)
// vertexCx        0x018(24)   0x6     15:0    FBI+TREX%     W      No / Yes   Vertex C x-coordinate location (12.4 format)
// vertexCy        0x01c(28)   0x7     15:0    FBI+TREX%     W      No / Yes   Vertex C y-coordinate location (12.4 format)
// startR          0x020(32)   0x8     23:0    FBI           W      No / Yes   Starting Red parameter (12.12 format)
// dRdX            0x024(36)   0x9     23:0    FBI           W      No / Yes   Change in Red with respect to X (12.12 format)
// dRdY            0x028(40)   0xA     23:0    FBI           W      No / Yes   Change in Red with respect to Y (12.12 format)
// startG          0x02c(44)   0xB     23:0    FBI           W      No / Yes   Starting Green parameter (12.12 format)
// dGdX            0x030(48)   0xC     23:0    FBI           W      No / Yes   Change in Green with respect to X (12.12 format)
// dGdY            0x034(52)   0xD     23:0    FBI           W      No / Yes   Change in Green with respect to Y (12.12 format)
// startB          0x038(56)   0xE     23:0    FBI           W      No / Yes   Starting Blue parameter (12.12 format)
// dBdX            0x03c(60)   0xF     23:0    FBI           W      No / Yes   Change in Blue with respect to X (12.12 format)
// dBdY            0x040(64)   0x10    23:0    FBI           W      No / Yes   Change in Blue with respect to Y (12.12 format)
// startZ          0x044(68)   0x11    31:0    FBI           W      No / Yes   Starting Z parameter (20.12 format)
// dZdX            0x048(72)   0x12    31:0    FBI           W      No / Yes   Change in Z with respect to X (20.12 format)
// dZdY            0x04c(76)   0x13    31:0    FBI           W      No / Yes   Change in Z with respect to Y (12.12 format)
// startA          0x050(80)   0x14    23:0    FBI           W      No / Yes   Starting Alpha parameter (12.12 format)
// dAdX            0x054(84)   0x15    23:0    FBI           W      No / Yes   Change in Alpha with respect to X (12.12 format)
// dAdY            0x058(88)   0x16    23:0    FBI           W      No / Yes   Change in Alpha with respect to Y (12.12 format)
// startS          0x05c(92)   0x17    31:0    TREX*         W      No / Yes   Starting S/W parameter (14.18 format)
// dSdX            0x060(96)   0x18    31:0    TREX*         W      No / Yes   Change in S/W with respect to X (14.18 format)
// dSdY            0x064(100)  0x19    31:0    TREX*         W      No / Yes   Change in S/W with respect to Y (14.18 format)
// startT          0x068(104)  0x1A    31:0    TREX*         W      No / Yes   Starting T/W parameter (14.18 format)
// dTdX            0x06c(108)  0x1B    31:0    TREX*         W      No / Yes   Change in T/W with respect to X (14.18 format)
// dTdY            0x070(112)  0x1C    31:0    TREX*         W      No / Yes   Change in T/W with respect to Y (14.18 format)
// startW          0x074(116)  0x1D    31:0    FBI+TREX*     W      No / Yes   Starting 1/W parameter (2.30 format)
// dWdX            0x078(120)  0x1E    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to X (2.30 format)
// dWdY            0x07c(124)  0x1F    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to Y (2.30 format)
// triangleCMD     0x080(128)  0x20    31      FBI+TREX%     W      No / Yes   Execute TRIANGLE command (sign bit)
// reserved        0x084(132)  0x21     n/a    n/a           W        n/a
// fvertexAx       0x088(136)  0x22    31:0    FBI+TREX%     W      No / Yes   Vertex A x-coordinate location (floating point)
// fvertexAy       0x08c(140)  0x23    31:0    FBI+TREX%     W      No / Yes   Vertex A y-coordinate location (floating point)
// fvertexBx       0x090(144)  0x24    31:0    FBI+TREX%     W      No / Yes   Vertex B x-coordinate location (floating point)
// fvertexBy       0x094(148)  0x25    31:0    FBI+TREX%     W      No / Yes   Vertex B y-coordinate location (floating point)
// fvertexCx       0x098(152)  0x26    31:0    FBI+TREX%     W      No / Yes   Vertex C x-coordinate location (floating point)
// fvertexCy       0x09c(156)  0x27    31:0    FBI+TREX%     W      No / Yes   Vertex C y-coordinate location (floating point)
// fstartR         0x0a0(160)  0x28    31:0    FBI           W      No / Yes   Starting Red parameter (floating point)
// fdRdX           0x0a4(164)  0x29    31:0    FBI           W      No / Yes   Change in Red with respect to X (floating point)
// fdRdY           0x0a8(168)  0x2A    31:0    FBI           W      No / Yes   Change in Red with respect to Y (floating point)
// fstartG         0x0ac(172)  0x2B    31:0    FBI           W      No / Yes   Starting Green parameter (floating point)
// fdGdX           0x0b0(176)  0x2C    31:0    FBI           W      No / Yes   Change in Green with respect to X (floating point)
// fdGdY           0x0b4(180)  0x2D    31:0    FBI           W      No / Yes   Change in Green with respect to Y (floating point)
// fstartB         0x0b8(184)  0x2E    31:0    FBI           W      No / Yes   Starting Blue parameter (floating point)
// fdBdX           0x0bc(188)  0x2F    31:0    FBI           W      No / Yes   Change in Blue with respect to X (floating point)
// fdBdY           0x0c0(192)  0x30    31:0    FBI           W      No / Yes   Change in Blue with respect to Y (floating point)
// fstartZ         0x0c4(196)  0x31    31:0    FBI           W      No / Yes   Starting Z parameter (floating point)
// fdZdX           0x0c8(200)  0x32    31:0    FBI           W      No / Yes   Change in Z with respect to X (floating point)
// fdZdY           0x0cc(204)  0x33    31:0    FBI           W      No / Yes   Change in Z with respect to Y (floating point)
// fstartA         0x0d0(208)  0x34    31:0    FBI           W      No / Yes   Starting Alpha parameter (floating point)
// fdAdX           0x0d4(212)  0x35    31:0    FBI           W      No / Yes   Change in Alpha with respect to X (floating point)
// fdAdY           0x0d8(216)  0x36    31:0    FBI           W      No / Yes   Change in Alpha with respect to Y (floating point)
// fstartS         0x0dc(220)  0x37    31:0    TREX*         W      No / Yes   Starting S/W parameter (floating point)
// fdSdX           0x0e0(224)  0x38    31:0    TREX*         W      No / Yes   Change in S/W with respect to X (floating point)
// fdSdY           0x0e4(228)  0x39    31:0    TREX*         W      No / Yes   Change in S/W with respect to Y (floating point)
// fstartT         0x0e8(232)  0x3A    31:0    TREX*         W      No / Yes   Starting T/W parameter (floating point)
// fdTdX           0x0ec(236)  0x3B    31:0    TREX*         W      No / Yes   Change in T/W with respect to X (floating point)
// fdTdY           0x0f0(240)  0x3C    31:0    TREX*         W      No / Yes   Change in T/W with respect to Y (floating point)
// fstartW         0x0f4(244)  0x3D    31:0    FBI+TREX*     W      No / Yes   Starting 1/W parameter (floating point)
// fdWdX           0x0f8(248)  0x3E    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to X (floating point)
// fdWdY           0x0fc(252)  0x3F    31:0    FBI+TREX*     W      No / Yes   Change in 1/W with respect to Y (floating point)
// ftriangleCMD    0x100(256)  0x40    31      FBI+TREX%     W      No / Yes   Execute TRIANGLE command (floating point)

}