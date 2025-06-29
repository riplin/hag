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

#include <hag/drivers/3dfx/shared/mmio2d/baseaddr.h>
#include <hag/drivers/3dfx/shared/mmio2d/breserr.h>
#include <hag/drivers/3dfx/shared/mmio2d/clip.h>
#include <hag/drivers/3dfx/shared/mmio2d/cmd.h>
#include <hag/drivers/3dfx/shared/mmio2d/cmdextra.h>
#include <hag/drivers/3dfx/shared/mmio2d/colkey.h>
#include <hag/drivers/3dfx/shared/mmio2d/color.h>
#include <hag/drivers/3dfx/shared/mmio2d/dstfmt.h>
#include <hag/drivers/3dfx/shared/mmio2d/intctrl.h>
#include <hag/drivers/3dfx/shared/mmio2d/lncharea.h>
#include <hag/drivers/3dfx/shared/mmio2d/lnstl.h>
#include <hag/drivers/3dfx/shared/mmio2d/lnstp.h>
#include <hag/drivers/3dfx/shared/mmio2d/pattern.h>
#include <hag/drivers/3dfx/shared/mmio2d/rop.h>
#include <hag/drivers/3dfx/shared/mmio2d/size.h>
#include <hag/drivers/3dfx/shared/mmio2d/srcfmt.h>
#include <hag/drivers/3dfx/shared/mmio2d/status.h>
#include <hag/drivers/3dfx/shared/mmio2d/xy.h>

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

namespace Hag::TDfx::Banshee
{

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
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, BaseAddress);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, BresenhamError);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Clip);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Command);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, CommandExtra);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ColorKey);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Color);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, DestinationFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, InterruptControl);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineStyle);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineStipple);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Pattern);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, RasterOperation);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Size);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, SourceFormat);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, Status);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, XY);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ScreenBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, ScreenStretchBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, HostBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, HostStretchBlitLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, RectangleFillLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, LineLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, PolyLineLaunch);
        IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::TwoD, PolygonLaunch);

        typedef Shared::TwoD::LaunchCommand_t LaunchCommand_t;
    }

    namespace MMIO2D
    {
        IMPORTNAMESPACE(Shared::MMIO2D, BaseAddress);
        IMPORTNAMESPACE(Shared::MMIO2D, BresenhamError);
        IMPORTNAMESPACE(Shared::MMIO2D, Clip);
        IMPORTNAMESPACE(Shared::MMIO2D, Command);
        IMPORTNAMESPACE(Shared::MMIO2D, CommandExtra);
        IMPORTNAMESPACE(Shared::MMIO2D, ColorKey);
        IMPORTNAMESPACE(Shared::MMIO2D, Color);
        IMPORTNAMESPACE(Shared::MMIO2D, DestinationFormat);
        IMPORTNAMESPACE(Shared::MMIO2D, InterruptControl);
        IMPORTNAMESPACE(Shared::MMIO2D, LineStyle);
        IMPORTNAMESPACE(Shared::MMIO2D, LineStipple);
        IMPORTNAMESPACE(Shared::MMIO2D, Pattern);
        IMPORTNAMESPACE(Shared::MMIO2D, RasterOperation);
        IMPORTNAMESPACE(Shared::MMIO2D, Size);
        IMPORTNAMESPACE(Shared::MMIO2D, SourceFormat);
        IMPORTNAMESPACE(Shared::MMIO2D, Status);
        IMPORTNAMESPACE(Shared::MMIO2D, XY);
        IMPORTNAMESPACE(Shared::MMIO2D, LaunchArea);
    }

    namespace Fifo
    {
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

}