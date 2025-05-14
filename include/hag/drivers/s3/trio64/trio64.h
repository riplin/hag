//Copyright 2023-Present riplin

#pragma once

#include <hag/drivers/imports.h>

#include <hag/drivers/vga/attribc/attrmode.h>
#include <hag/drivers/vga/attribc/bordcol.h>
#include <hag/drivers/vga/attribc/colplnen.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/attribc/horpxpan.h>
#include <hag/drivers/vga/attribc/index.h>
#include <hag/drivers/vga/attribc/palette.h>
#include <hag/drivers/vga/attribc/pixelpad.h>

#include <hag/drivers/s3/shared/crtc/attridxf.h>
#include <hag/drivers/s3/shared/crtc/attridxi.h>
#include <hag/drivers/s3/shared/crtc/biosflag.h>
#include <hag/drivers/s3/shared/crtc/bkwcomp1.h>
#include <hag/drivers/s3/shared/crtc/bkwcomp2.h>
#include <hag/drivers/s3/shared/crtc/bkwcomp3.h>
#include <hag/drivers/s3/shared/crtc/chipidrv.h>
#include <hag/drivers/s3/shared/crtc/conf1.h>
#include <hag/drivers/s3/shared/crtc/conf2.h>
#include <hag/drivers/s3/shared/crtc/conf3.h>
#include <hag/drivers/s3/shared/crtc/cpulatch.h>
#include <hag/drivers/vga/crtc/crtcovfl.h>
#include <hag/drivers/s3/shared/crtc/crtreglk.h>
#include <hag/drivers/vga/crtc/curendsl.h>
#include <hag/drivers/vga/crtc/curladhi.h>
#include <hag/drivers/vga/crtc/curladlo.h>
#include <hag/drivers/vga/crtc/curstrsl.h>
#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/s3/shared/crtc/devidhi.h>
#include <hag/drivers/s3/shared/crtc/devidlo.h>
#include <hag/drivers/vga/crtc/enhorbln.h>
#include <hag/drivers/vga/crtc/enhorsyn.h>
#include <hag/drivers/vga/crtc/enverbln.h>
#include <hag/drivers/s3/shared/crtc/exbiosf1.h>
#include <hag/drivers/s3/shared/crtc/exbiosf3.h>
#include <hag/drivers/s3/shared/crtc/exbiosf4.h>
#include <hag/drivers/s3/shared/crtc/exhorovf.h>
#include <hag/drivers/s3/shared/crtc/exmemct1.h>
#include <hag/drivers/s3/shared/crtc/exmemct2.h>
#include <hag/drivers/s3/shared/crtc/exmemct3.h>
#include <hag/drivers/s3/shared/crtc/exmscct.h>
#include <hag/drivers/s3/shared/crtc/exmscct1.h>
#include <hag/drivers/s3/shared/crtc/exmscct2.h>
#include <hag/drivers/s3/shared/crtc/exsynct1.h>
#include <hag/drivers/s3/shared/crtc/exsynct2.h>
#include <hag/drivers/s3/shared/crtc/exsynct3.h>
#include <hag/drivers/s3/shared/crtc/exsysct1.h>
#include <hag/drivers/s3/shared/crtc/exsysct2.h>
#include <hag/drivers/s3/shared/crtc/exsysct3.h>
#include <hag/drivers/s3/shared/crtc/exsysct4.h>
#include <hag/drivers/s3/shared/crtc/extmode.h>
#include <hag/drivers/s3/shared/crtc/extrdctl.h>
#include <hag/drivers/s3/shared/crtc/exverovf.h>
#include <hag/drivers/s3/shared/crtc/genout.h>
#include <hag/drivers/vga/crtc/hordisen.h>
#include <hag/drivers/vga/crtc/hortotal.h>
#include <hag/drivers/s3/shared/crtc/hwgcmode.h>
#include <hag/drivers/s3/shared/crtc/hwgcoxh.h>
#include <hag/drivers/s3/shared/crtc/hwgcoxl.h>
#include <hag/drivers/s3/shared/crtc/hwgcoyh.h>
#include <hag/drivers/s3/shared/crtc/hwgcoyl.h>
#include <hag/drivers/s3/shared/crtc/hwgcbgcs.h>
#include <hag/drivers/s3/shared/crtc/hwgcfgcs.h>
#include <hag/drivers/s3/shared/crtc/hwgcssah.h>
#include <hag/drivers/s3/shared/crtc/hwgcssal.h>
#include <hag/drivers/s3/shared/crtc/hwgcpdsx.h>
#include <hag/drivers/s3/shared/crtc/hwgcpdsy.h>
#include <hag/drivers/vga/crtc/index.h>
#include <hag/drivers/s3/shared/crtc/itlrtst.h>
#include <hag/drivers/s3/shared/crtc/linawctr.h>
#include <hag/drivers/vga/crtc/linecomp.h>
#include <hag/drivers/s3/shared/crtc/lnawposh.h>
#include <hag/drivers/s3/shared/crtc/lnawposl.h>
#include <hag/drivers/vga/crtc/maxscanl.h>
#include <hag/drivers/s3/shared/crtc/memconf.h>
#include <hag/drivers/s3/shared/crtc/misc1.h>
#include <hag/drivers/vga/crtc/modectrl.h>
#include <hag/drivers/s3/shared/crtc/modectrl.h>
#include <hag/drivers/vga/crtc/prstrwsc.h>
#include <hag/drivers/s3/shared/crtc/reglock1.h>
#include <hag/drivers/s3/shared/crtc/reglock2.h>
#include <hag/drivers/s3/shared/crtc/revision.h>
#include <hag/drivers/vga/crtc/scrnoffs.h>
#include <hag/drivers/vga/crtc/staddrhi.h>
#include <hag/drivers/vga/crtc/staddrlo.h>
#include <hag/drivers/s3/shared/crtc/stdsfifo.h>
#include <hag/drivers/vga/crtc/sthorbln.h>
#include <hag/drivers/vga/crtc/sthorsyn.h>
#include <hag/drivers/vga/crtc/stverbln.h>
#include <hag/drivers/s3/shared/crtc/sysconf.h>
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
#include <hag/drivers/vga/gfxc/roprotct.h>
#include <hag/drivers/vga/gfxc/srdata.h>

#include <hag/drivers/vga/sqrc/chfntsel.h>
#include <hag/drivers/vga/sqrc/clkmod.h>
#include <hag/drivers/s3/shared/sqrc/clksync1.h>
#include <hag/drivers/s3/shared/sqrc/clksync2.h>
#include <hag/drivers/s3/shared/sqrc/clksynth.h>
#include <hag/drivers/s3/shared/sqrc/clksyntl.h>
#include <hag/drivers/vga/sqrc/data.h>
#include <hag/drivers/s3/shared/sqrc/dclkvhi.h>
#include <hag/drivers/s3/shared/sqrc/dclkvlow.h>
#include <hag/drivers/vga/sqrc/enwrtpl.h>
#include <hag/drivers/s3/shared/sqrc/extbusrc.h>
#include <hag/drivers/s3/shared/sqrc/extseq9.h>
#include <hag/drivers/s3/shared/sqrc/extseqd.h>
#include <hag/drivers/vga/sqrc/index.h>
#include <hag/drivers/s3/shared/sqrc/mclkvhi.h>
#include <hag/drivers/s3/shared/sqrc/mclkvlow.h>
#include <hag/drivers/s3/shared/sqrc/memodctl.h>
#include <hag/drivers/s3/shared/sqrc/mscextsq.h>
#include <hag/drivers/s3/shared/sqrc/rclksync.h>
#include <hag/drivers/s3/shared/sqrc/regs.h>
#include <hag/drivers/vga/sqrc/reset.h>
#include <hag/drivers/s3/shared/sqrc/unlexseq.h>

#include <hag/drivers/s3/shared/advfnctl.h>
#include <hag/drivers/s3/shared/bgcolor.h>
#include <hag/drivers/s3/shared/bgmix.h>
#include <hag/drivers/s3/shared/bitplnrm.h>
#include <hag/drivers/s3/shared/bitplnwm.h>
#include <hag/drivers/s3/shared/chipid.h>
#include <hag/drivers/s3/shared/colcomp.h>
#include <hag/drivers/s3/shared/curxpos.h>
#include <hag/drivers/s3/shared/curxpos2.h>
#include <hag/drivers/s3/shared/curypos.h>
#include <hag/drivers/s3/shared/curypos2.h>
#include <hag/drivers/vga/dacdata.h>
#include <hag/drivers/vga/dacmask.h>
#include <hag/drivers/vga/dacrdidx.h>
#include <hag/drivers/vga/dacstat.h>
#include <hag/drivers/vga/dacwridx.h>
#include <hag/drivers/s3/shared/drawcmd.h>
#include <hag/drivers/s3/shared/drawcmd2.h>
#include <hag/drivers/s3/shared/dxpdsc.h>
#include <hag/drivers/s3/shared/dypasc.h>
#include <hag/drivers/s3/shared/featctl.h>
#include <hag/drivers/s3/shared/fgcolor.h>
#include <hag/drivers/s3/shared/fgmix.h>
#include <hag/drivers/s3/shared/gfxprocs.h>
#include <hag/drivers/s3/shared/instat0.h>
#include <hag/drivers/vga/instat1.h>
#include <hag/drivers/s3/shared/linerrt2.h>
#include <hag/drivers/s3/shared/linerrtr.h>
#include <hag/drivers/s3/shared/majapcn2.h>
#include <hag/drivers/s3/shared/majapcnt.h>
#include <hag/drivers/s3/shared/miscout.h>
#include <hag/drivers/s3/shared/patx.h>
#include <hag/drivers/s3/shared/paty.h>
#include <hag/drivers/s3/shared/pixdtrf.h>
#include <hag/drivers/s3/shared/rregdata.h>
#include <hag/drivers/s3/shared/setupos.h>
#include <hag/drivers/s3/shared/shrstrvt.h>
#include <hag/drivers/s3/shared/subsysct.h>
#include <hag/drivers/s3/shared/subsysst.h>
#include <hag/drivers/s3/trio64/vidmodes.h>
#include <hag/drivers/s3/shared/vidsuben.h>
#include <hag/drivers/s3/shared/wregdata.h>
#include <hag/drivers/s3/shared/xcoord2.h>
#include <hag/drivers/s3/shared/yc2asc2.h>

#include <hag/drivers/s3/shared/packed/bgcol.h>
#include <hag/drivers/s3/shared/packed/bprm.h>
#include <hag/drivers/s3/shared/packed/bpwm.h>
#include <hag/drivers/s3/shared/packed/colcmp.h>
#include <hag/drivers/s3/shared/packed/curxypos.h>
#include <hag/drivers/s3/shared/packed/curxyps2.h>
#include <hag/drivers/s3/shared/packed/dwgcmds.h>
#include <hag/drivers/s3/shared/packed/dxypsc.h>
#include <hag/drivers/s3/shared/packed/fgcol.h>
#include <hag/drivers/s3/shared/packed/lnerrtrs.h>
#include <hag/drivers/s3/shared/packed/mapc2.h>
#include <hag/drivers/s3/shared/packed/mfrs.h>
#include <hag/drivers/s3/shared/packed/mixers.h>
#include <hag/drivers/s3/shared/packed/mmapc.h>
#include <hag/drivers/s3/shared/packed/patxy.h>
#include <hag/drivers/s3/shared/packed/pcmfm2.h>
#include <hag/drivers/s3/shared/packed/pixdtr.h>
#include <hag/drivers/s3/shared/packed/scisbr.h>
#include <hag/drivers/s3/shared/packed/scistl.h>
#include <hag/drivers/s3/shared/packed/ssvt.h>
#include <hag/drivers/s3/shared/packed/xycoord2.h>

#include <hag/drivers/s3/shared/ddchw.h>
#include <hag/drivers/s3/shared/pmhw.h>

namespace Hag { namespace S3 { namespace Trio64
{

typedef Shared::Register_t Register_t;

namespace Register
{
    using namespace VGA::Register;
    using namespace Shared::Register;
}

IMPORTNAMESPACEANDTYPE(VGA, AttributeControllerData);
IMPORTNAMESPACE(VGA, AttributeControllerIndex);

namespace AttributeController
{

    //IMPORTNAMESPACEANDTYPE(Shared::AttributeController, Register);
    IMPORTNAMESPACE(VGA::AttributeController, Register);

    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, AttributeMode);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, BorderColor);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, ColorPlane);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, HorizontalPixelPanning);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, Palette);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::AttributeController, PixelPadding);
}

IMPORTNAMESPACEANDTYPE(VGA, CRTControllerData);
IMPORTNAMESPACE(VGA, CRTControllerIndex);

namespace CRTController
{
    IMPORTNAMESPACE(VGA::CRTController, Register);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, Register);

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
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, CPULatchData);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, AttributeIndexF);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, AttributeIndexI);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, DeviceIDHigh);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, DeviceIDLow);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, Revision);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ChipIDRevision);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, MemoryConfiguration);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, BackwardCompatibility1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, BackwardCompatibility2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, BackwardCompatibility3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, CRTRegisterLock);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, Configuration1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, Configuration2);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, RegisterLock1);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, RegisterLock2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, Miscellaneous1);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, StartDisplayFIFO);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, InterlaceRetraceStart);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, SystemConfiguration);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, BIOSFlag);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ModeControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMode);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, HardwareGraphicsCursorMode);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorOriginXH);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorOriginXL);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorOriginYH);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorOriginYL);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorForegroundColorStack);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorBackgroundColorStack);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorStorageStartAddressH);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorStorageStartAddressL);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorPatternDisplayStartX);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, HardwareGraphicsCursorPatternDisplayStartY);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedSystemControl1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedSystemControl2);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, ExtendedBIOSFlag1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMemoryControl1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMemoryControl2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedRAMDACControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExternalSyncControl1);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, ExternalSyncControl2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, LinearAddressWindowControl);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, LinearAddressWindowPositionH);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, LinearAddressWindowPositionL);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, GeneralOutputPort);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedHorizontalOverflow);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedVerticalOverflow);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, ExtendedMemoryControl3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExternalSyncControl3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMiscellaneousControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMiscellaneousControl1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedMiscellaneousControl2);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, Configuration3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedSystemControl3);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::CRTController, ExtendedSystemControl4);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, ExtendedBIOSFlag3);
    IMPORTNAMESPACEANDTYPE(Shared::CRTController, ExtendedBIOSFlag4);
}

IMPORTNAMESPACEANDTYPE(VGA, GraphicsControllerData);
IMPORTNAMESPACE(VGA, GraphicsControllerIndex);

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

IMPORTNAMESPACEANDTYPE(VGA, SequencerData);
IMPORTNAMESPACE(VGA, SequencerIndex);

namespace Sequencer
{

    IMPORTNAMESPACE(VGA::Sequencer, Register);
    IMPORTNAMESPACEANDTYPE(Shared::Sequencer, Register);

    namespace Register
    {
        enum
        {
            Unknown1A = 0x1A,                       //SR1A
            Unknown1B = 0x1B,                       //SR1B
        };
    }

    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, Reset);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, ClockingMode);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, EnableWritePlane);
    IMPORTNAMESPACEANDTYPEANDSHIFT(VGA::Sequencer, CharacterFontSelect);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, MemoryModeControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, UnlockExtendedSequencer);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, ExtendedSequencer9);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, ExternalBusRequestControl);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, MiscellaneousExtendedSequencer);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, ExtendedSequencerD);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, MClockValueLow);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, MClockValueHigh);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, DClockValueLow);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, DClockValueHigh);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, ClockSynthControl1);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, ClockSynthControl2);
    IMPORTNAMESPACEANDTYPE(Shared::Sequencer, ClockSynthTestHigh);
    IMPORTNAMESPACEANDTYPE(Shared::Sequencer, ClockSynthTestLow);
    IMPORTNAMESPACEANDTYPEANDSHIFT(Shared::Sequencer, RAMDACClockSynthControl);
}

IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, InputStatus1);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, FeatureControl);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, MiscellaneousOutput);
IMPORTNAMESPACEANDTYPE(VGA, DACMask);
IMPORTNAMESPACEANDTYPE(VGA, DACReadIndex);
IMPORTNAMESPACEANDTYPEANDSHIFT(VGA, DACStatus);
IMPORTNAMESPACEANDTYPE(VGA, DACWriteIndex);
IMPORTNAMESPACEANDTYPE(VGA, RAMDACData);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, InputStatus0);
IMPORTNAMESPACEANDTYPE(Shared, SetupOptionSelect);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, VideoSubsystemEnable);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, SubsystemStatus);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, SubsystemControl);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, AdvancedFunctionControl); typedef Shared::AdvancedFunctionControlLower_t AdvancedFunctionControlLower_t;
IMPORTNAMESPACEANDTYPE(Shared, CurrentYPosition);
IMPORTNAMESPACEANDTYPE(Shared, CurrentYPosition2);
IMPORTNAMESPACEANDTYPE(Shared, CurrentXPosition);
IMPORTNAMESPACEANDTYPE(Shared, CurrentXPosition2);
IMPORTNAMESPACEANDTYPE(Shared, DestinationYPositionAxialStepConstant);
IMPORTNAMESPACEANDTYPE(Shared, YCoordinate2AxialStepConstant2);
IMPORTNAMESPACEANDTYPE(Shared, DestinationXPositionDiagonalStepConstant);
IMPORTNAMESPACEANDTYPE(Shared, XCoordinate2);
IMPORTNAMESPACEANDTYPE(Shared, LineErrorTerm);
IMPORTNAMESPACEANDTYPE(Shared, LineErrorTerm2);
IMPORTNAMESPACEANDTYPE(Shared, MajorAxisPixelCount);
IMPORTNAMESPACEANDTYPE(Shared, MajorAxisPixelCount2);
IMPORTNAMESPACEANDTYPEANDSHIFT(Shared, GraphicsProcessorStatus);
IMPORTNAMESPACEANDTYPE(Shared, DrawingCommand);
IMPORTNAMESPACEANDTYPE(Shared, DrawingCommand2);
IMPORTNAMESPACEANDTYPE(Shared, ShortStrokeVectorTransfer);
IMPORTNAMESPACEANDTYPE(Shared, BackgroundColor);
IMPORTNAMESPACEANDTYPE(Shared, ForegroundColor);
IMPORTNAMESPACEANDTYPE(Shared, BitplaneWriteMask);
IMPORTNAMESPACEANDTYPE(Shared, BitplaneReadMask);
IMPORTNAMESPACEANDTYPE(Shared, ColorCompare);
IMPORTNAMESPACEANDTYPE(Shared, BackgroundMix);
IMPORTNAMESPACEANDTYPE(Shared, ForegroundMix);
IMPORTNAMESPACEANDTYPE(Shared, ReadRegisterData);
IMPORTNAMESPACEANDTYPE(Shared, PatternY);
IMPORTNAMESPACEANDTYPE(Shared, PatternX);


IMPORTNAMESPACEANDTYPE(Shared, MultifunctionControlMiscellaneous2);
IMPORTNAMESPACEANDTYPE(Shared, MultifunctionControlMiscellaneous);
IMPORTNAMESPACEANDTYPE(Shared, PixelControl);
IMPORTNAMESPACEANDTYPE(Shared, MinorAxisPixelCount);
IMPORTNAMESPACEANDTYPE(Shared, TopScissors);
IMPORTNAMESPACEANDTYPE(Shared, LeftScissors);
IMPORTNAMESPACEANDTYPE(Shared, BottomScissors);
IMPORTNAMESPACEANDTYPE(Shared, RightScissors);

IMPORTNAMESPACEANDTYPE(Shared, WriteRegisterData);


typedef Shared::PixelDataTransfer16_t PixelDataTransfer16_t;
typedef Shared::PixelDataTransfer32_t PixelDataTransfer32_t;

namespace PixelDataTransfer
{
    using namespace Shared::PixelDataTransfer;
}

namespace MMIO
{
    IMPORTNAMESPACE(Shared::MMIO, CurrentYPosition);
    IMPORTNAMESPACE(Shared::MMIO, CurrentYPosition2);
    IMPORTNAMESPACE(Shared::MMIO, CurrentXPosition);
    IMPORTNAMESPACE(Shared::MMIO, CurrentXPosition2);
    IMPORTNAMESPACE(Shared::MMIO, DestinationYPositionAxialStepConstant);
    IMPORTNAMESPACE(Shared::MMIO, YCoordinate2AxialStepConstant2);
    IMPORTNAMESPACE(Shared::MMIO, DestinationXPositionDiagonalStepConstant);
    IMPORTNAMESPACE(Shared::MMIO, XCoordinate2);
    IMPORTNAMESPACE(Shared::MMIO, LineErrorTerm);
    IMPORTNAMESPACE(Shared::MMIO, LineErrorTerm2);
    IMPORTNAMESPACE(Shared::MMIO, MajorAxisPixelCount);
    IMPORTNAMESPACE(Shared::MMIO, MajorAxisPixelCount2);
    IMPORTNAMESPACE(Shared::MMIO, GraphicsProcessorStatus);
    IMPORTNAMESPACE(Shared::MMIO, DrawingCommand);
    IMPORTNAMESPACE(Shared::MMIO, DrawingCommand2);
    IMPORTNAMESPACE(Shared::MMIO, ShortStrokeVectorTransfer);
    IMPORTNAMESPACE(Shared::MMIO, BackgroundColor);
    IMPORTNAMESPACE(Shared::MMIO, ForegroundColor);
    IMPORTNAMESPACE(Shared::MMIO, BitplaneWriteMask);
    IMPORTNAMESPACE(Shared::MMIO, BitplaneReadMask);
    IMPORTNAMESPACE(Shared::MMIO, ColorCompare);
    IMPORTNAMESPACE(Shared::MMIO, BackgroundMix);
    IMPORTNAMESPACE(Shared::MMIO, ForegroundMix);
    IMPORTNAMESPACE(Shared::MMIO, PixelDataTransfer);
    IMPORTNAMESPACE(Shared::MMIO, PatternY);
    IMPORTNAMESPACE(Shared::MMIO, PatternX);

    namespace Packed
    {
        IMPORTNAMESPACE(Shared::MMIO::Packed, CurrentXYPosition);
        IMPORTNAMESPACE(Shared::MMIO::Packed, CurrentXYPosition2);
        IMPORTNAMESPACE(Shared::MMIO::Packed, DestinationXYPositionStepConstant);
        IMPORTNAMESPACE(Shared::MMIO::Packed, XYCoordinate2);
        IMPORTNAMESPACE(Shared::MMIO::Packed, LineErrorTerms);
        IMPORTNAMESPACE(Shared::MMIO::Packed, DrawingCommands);
        IMPORTNAMESPACE(Shared::MMIO::Packed, ShortStrokeVectorTransfer);
        IMPORTNAMESPACE(Shared::MMIO::Packed, BackgroundColor);
        IMPORTNAMESPACE(Shared::MMIO::Packed, ForegroundColor);
        IMPORTNAMESPACE(Shared::MMIO::Packed, BitplaneWriteMask);
        IMPORTNAMESPACE(Shared::MMIO::Packed, BitplaneReadMask);
        IMPORTNAMESPACE(Shared::MMIO::Packed, ColorCompare);
        IMPORTNAMESPACE(Shared::MMIO::Packed, Mixers);
        IMPORTNAMESPACE(Shared::MMIO::Packed, ScissorsTopLeft);
        IMPORTNAMESPACE(Shared::MMIO::Packed, ScissorsBottomRight);
        IMPORTNAMESPACE(Shared::MMIO::Packed, PixelControlAndMultiFunctionMisc2);
        IMPORTNAMESPACE(Shared::MMIO::Packed, MultiFunctionAndReadSelect);
        IMPORTNAMESPACE(Shared::MMIO::Packed, MinorMajorAxisPixelCounts);
        IMPORTNAMESPACE(Shared::MMIO::Packed, MajorAxisPixelCount2);
        IMPORTNAMESPACE(Shared::MMIO::Packed, PixelDataTransfer);
        IMPORTNAMESPACE(Shared::MMIO::Packed, PatternXY);
    }
}

namespace PM
{
    using namespace Shared::PM;
}

namespace DDC
{
    using namespace Shared::DDC;
}

}}}