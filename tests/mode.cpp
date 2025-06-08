//Copyright 2025-Present riplin

#include <i86.h>
#include <stdio.h>
#include <string.h>

#ifndef MOCK
#include <vector>
#include <string>
#include <algorithm>
#endif
#include "../src/drivers/matrox/shared/sysintl.h"
#include "mode.h"
#include <hag/system/pci.h>
#include <hag/system/bda.h>
#include <hag/system/pit.h>
#include <hag/system/machid.h>
#include <hag/system/keyboard.h>
#include <hag/system/interrup.h>
#include <hag/drivers/vga/vidmodes.h>
#include <hag/vesa/vidmodes.h>
#include <hag/drivers/vga/vga.h>

#include <hag/drivers/matrox/shared/data.h>
#include <hag/drivers/matrox/shared/miscout.h>              //0x3C2, 0x3CC
#include <hag/drivers/matrox/shared/sqrc/memodctl.h>        //SR4

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


namespace Hag { namespace Matrox { namespace Shared { namespace Function
{

namespace Mode
{

Hag::System::BDA::VideoParameterTable DefaultParameters =
{
    0x28,
    0x18,
    0x08,
    0x0800,
    { 0x09, 0x03, 0x00, 0x02 },
    0x63,
    { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xBF, 0x1F, 0x00, 0xC7, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x14, 0x1F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x08, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode0And1SL350 =
{
    0x28,
    0x18,
    0x0E,
    0x0800,
    { 0x09, 0x03, 0x00, 0x02 },
    0xA3,
    { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xBF, 0x1F, 0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x83, 0x85, 0x5D, 0x14, 0x1F, 0x63, 0xBA, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x08, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode0And1SL400 =
{
    0x28,
    0x18,
    0x10,
    0x0800,
    { 0x08, 0x03, 0x00, 0x02 },
    0x67,
    { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xBF, 0x1F, 0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x14, 0x1F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x08 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode2And3SL200 =
{
    0x50,
    0x18,
    0x08,
    0x1000,
    { 0x01, 0x03, 0x00, 0x02 },
    0x63,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0xC7, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x08, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode2And3SL350 =
{
    0x50,
    0x18,
    0x0E,
    0x1000,
    { 0x01, 0x03, 0x00, 0x02 },
    0xA3,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x83, 0x85, 0x5D, 0x28, 0x1F, 0x63, 0xBA, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x08, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode2And3SL400 =
{
    0x50,
    0x18,
    0x10,
    0x1000,
    { 0x00, 0x03, 0x00, 0x02 },
    0x67,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x08 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode4And5SL200 =
{
    0x28,
    0x18,
    0x08,
    0x4000,
    { 0x09, 0x03, 0x00, 0x02 },
    0x63,
    { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80, 0xBF, 0x1F, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x14, 0x00, 0x96, 0xB9, 0xA2, 0xFF },
    { 0x00, 0x13, 0x15, 0x17, 0x02, 0x04, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x01, 0x00, 0x03, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x0F, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode6SL200 =
{
    0x50,
    0x18,
    0x08,
    0x4000,
    { 0x01, 0x01, 0x00, 0x06 },
    0x63,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xC2, 0xFF },
    { 0x00, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x17, 0x01, 0x00, 0x01, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode7SL350 =
{
    0x50,
    0x18,
    0x0E,
    0x1000,
    { 0x00, 0x03, 0x00, 0x03 },
    0xA6,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x83, 0x85, 0x5D, 0x28, 0x0D, 0x63, 0xBA, 0xA3, 0xFF },
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x0F, 0x08 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0A, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode7SL400 =
{
    0x50,
    0x18,
    0x10,
    0x1000,
    { 0x00, 0x03, 0x00, 0x02 },
    0x66,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x0F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x0F, 0x08 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0A, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable ModeDSL200 =
{
    0x28,
    0x18,
    0x08,
    0x2000,
    { 0x09, 0x0F, 0x00, 0x06 },
    0x63,
    { 0x2D, 0x27, 0x28, 0x90, 0x2B, 0x80, 0xBF, 0x1F, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x14, 0x00, 0x96, 0xB9, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable ModeESL200 =
{
    0x50,
    0x18,
    0x08,
    0x4000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0x63,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable ModeFSL350 =
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0xA2,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x85, 0x5D, 0x28, 0x0F, 0x63, 0xBA, 0xE3, 0xFF },
    { 0x00, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x0B, 0x00, 0x05, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x05, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode10SL350 =
{
    0x50,
    0x18,
    0x0E,
    0x8000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0xA3,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x85, 0x5D, 0x28, 0x0F, 0x63, 0xBA, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode11SL480 =
{
    0x50,
    0x1D,
    0x10,
    0xA000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0xE3,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xC3, 0xFF },
    { 0x00, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode12SL480 =
{
    0x50,
    0x1D,
    0x10,
    0xA000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0xE3,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x8C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode13SL200 =
{
    0x28,
    0x18,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x63,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G640x400 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x6F,
    { 0x67, 0x4F, 0x4F, 0x0B, 0x53, 0x9F, 0xC0, 0x1F, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0x8D, 0x8F, 0x50, 0x00, 0x8F, 0xC1, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G640x480 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x5F, 0x4F, 0x4F, 0x03, 0x51, 0x9D, 0x0B, 0x3E, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE9, 0x2B, 0xDF, 0x50, 0x00, 0xDF, 0x0C, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G800x600x4 =
{
    0x64,
    0x24,
    0x10,
    0xF000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0x2F,
    { 0x7F, 0x63, 0x63, 0x82, 0x6B, 0x1B, 0x72, 0xF0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x8C, 0x57, 0x32, 0x00, 0x57, 0x73, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x01, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G800x600 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x2F,
    { 0x7F, 0x63, 0x63, 0x03, 0x68, 0x18, 0x72, 0xF0, 0x00, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x2C, 0x57, 0x64, 0x00, 0x57, 0x73, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G1024x768 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0xA3, 0x7F, 0x7F, 0x07, 0x82, 0x93, 0x24, 0xF5, 0x00, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x28, 0xFF, 0x80, 0x00, 0xFF, 0x25, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G1280x1024 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x2F,
    { 0xCE, 0x9F, 0x9F, 0x12, 0xA5, 0x13, 0x28, 0x5A, 0x00, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0xFF, 0xA0, 0x00, 0xFF, 0x29, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable G1600x1200 =
{
    0x50,
    0x0B,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x2F,
    { 0x09, 0xC7, 0xC7, 0x0D, 0xCF, 0x07, 0xE0, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x23, 0xAF, 0xC8, 0x00, 0xAF, 0xE1, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable T80x60 =
{
    0x50,
    0x3B,
    0x08,
    0x2580,
    { 0x01, 0x03, 0x00, 0x02 },
    0xEF,
    { 0x60, 0x4F, 0x50, 0x83, 0x52, 0x9E, 0x0B, 0x3E, 0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x8C, 0xDF, 0x28, 0x1F, 0xE6, 0x06, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable T132x25 =
{
    0x84,
    0x18,
    0x10,
    0x19D0,
    { 0x01, 0x03, 0x00, 0x02 },
    0x6F,
    { 0x9E, 0x83, 0x84, 0x01, 0x87, 0x8D, 0xC0, 0x1F, 0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x86, 0x8F, 0x42, 0x1F, 0x96, 0xBB, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable T132x43 =
{
    0x84,
    0x2A,
    0x08,
    0x2C58,
    { 0x01, 0x03, 0x00, 0x02 },
    0x6F,
    { 0x9E, 0x83, 0x84, 0x01, 0x87, 0x8D, 0x88, 0x1F, 0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x66, 0x84, 0x57, 0x42, 0x1F, 0x5E, 0x83, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable T132x50 =
{
    0x84,
    0x31,
    0x08,
    0x3400,
    { 0x01, 0x03, 0x00, 0x02 },
    0x6F,
    { 0x9E, 0x83, 0x84, 0x01, 0x87, 0x8D, 0xC0, 0x1F, 0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x9E, 0x86, 0x8F, 0x42, 0x1F, 0x96, 0xBB, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable T132x60 =
{
    0x84,
    0x3B,
    0x08,
    0x3E00,
    { 0x01, 0x03, 0x00, 0x02 },
    0xEF,
    { 0x9E, 0x83, 0x84, 0x01, 0x87, 0x8D, 0x11, 0x3E, 0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x8E, 0xDF, 0x42, 0x1F, 0xE6, 0x0C, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

//Mode Y: 320x200x8bpp planar
//https://qbmikehawk.neocities.org/articles/mode-y/
Hag::System::BDA::VideoParameterTable ModeYSL200 =
{
    0x28,
    0x18,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0x63,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x8E, 0x8F, 0x28, 0x00, 0x96, 0xB9, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};
//Mode X: 320x240x8bpp planar
Hag::System::BDA::VideoParameterTable ModeXSL200 =
{
    0x28,
    0x18,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x06 },
    0xe3,
    { 0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0D, 0x3E, 0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xAC, 0xDF, 0x28, 0x00, 0xE7, 0x06, 0xE3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

//Mode Q = 256x256x256
//https://swag.outpostbbs.net/EGAVGA/0175.PAS.html
Hag::System::BDA::VideoParameterTable ModeQSL200 =
{
    0x28,
    0x18,
    0x08,
    0x2000,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xe3,
    { 0x5F, 0x3F, 0x40, 0x82, 0x4E, 0x9A, 0x23, 0xB2, 0x00, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xAC, 0xFF, 0x20, 0x40, 0x07, 0x17, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x00, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode80x50 =
{ // 3
    0x50,
    0x31,
    0x08,
    0x2040,
    { 0x00, 0x03, 0x00, 0x02 },
    0x67,
    { 0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F, 0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x01, 0x40, 0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x0C, 0x00, 0x0F, 0x08 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1B8 =
{ // 1B8
    0x50,
    0x0B,
    0x08,
    0x4003,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E, 0x0B, 0x3E, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xAC, 0xDF, 0x80, 0x00, 0xDF, 0x0C, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1F9 =
{ // 1F9
    0x50,
    0x0B,
    0x08,
    0x4001,
    { 0x01, 0x0F, 0x00, 0x0E },
    0x6F,
    { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E, 0xBF, 0x1F, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0xAE, 0x8F, 0x20, 0x00, 0x8F, 0xC0, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1F8 =
{ // 1F8
    0x50,
    0x0B,
    0x08,
    0x4001,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E, 0x0B, 0x3E, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xAC, 0xDF, 0x20, 0x00, 0xDF, 0x0C, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1E9 =
{ // 1E9
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E, 0x0B, 0x3E, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xAC, 0xDF, 0x40, 0x00, 0xDF, 0x0C, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1D9 =
{ // 1D9
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x2D, 0x27, 0x27, 0x91, 0x2B, 0x8E, 0x0B, 0x3E, 0x00, 0x41, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0xAC, 0xDF, 0x40, 0x00, 0xDF, 0x0C, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1F5 =
{ // 1F5
    0x50,
    0x0B,
    0x08,
    0x4001,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C, 0x99, 0xF0, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xAE, 0x57, 0x20, 0x00, 0x57, 0x9A, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1E6 =
{ // 1E6
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C, 0x99, 0xF0, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xAE, 0x57, 0x40, 0x00, 0x57, 0x9A, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1D6 =
{ // 1D6
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C, 0x99, 0xF0, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xAE, 0x57, 0x40, 0x00, 0x57, 0x9A, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1F3 =
{ // 1F3
    0x50,
    0x0B,
    0x08,
    0x4001,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06, 0x23, 0xF5, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA8, 0xFF, 0x20, 0x00, 0xFF, 0x24, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1B3 =
{ // 1B3
    0x50,
    0x0B,
    0x08,
    0x4003,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06, 0x23, 0xF5, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA8, 0xFF, 0x80, 0x00, 0xFF, 0x24, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1E4 =
{ // 1E4
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06, 0x23, 0xF5, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA8, 0xFF, 0x40, 0x00, 0xFF, 0x24, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1D4 =
{ // 1D4
    0x50,
    0x0B,
    0x08,
    0x4002,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x4C, 0x3F, 0x3F, 0x90, 0x43, 0x06, 0x23, 0xF5, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xA8, 0xFF, 0x40, 0x00, 0xFF, 0x24, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

Hag::System::BDA::VideoParameterTable Mode1B5 =
{ // 1B5
    0x50,
    0x0B,
    0x08,
    0x4003,
    { 0x01, 0x0F, 0x00, 0x0E },
    0xEF,
    { 0x3B, 0x31, 0x31, 0x9F, 0x35, 0x9C, 0x99, 0xF0, 0x00, 0x61, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xAE, 0x57, 0x80, 0x00, 0x57, 0x9A, 0xC3, 0xFF },
    { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x41, 0x02, 0x0F, 0x00 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF }
};

// Mode 0, 1
ParameterFontPair Mode0And1Parameters[] =
{
    { &DefaultParameters, Data::Font8x8, 256, 8, 0, false },
    { &Mode0And1SL350, Data::Font8x16, 256, 0, 0, true },
    { &Mode0And1SL400, Data::Font8x16, 256, 16, 0, true },
    { &DefaultParameters, Data::Font8x16, 256, 16, 0, true }
};

// Mode 2, 3
ParameterFontPair Mode2And3Parameters[] =
{
    { &Mode2And3SL200, Data::Font8x8, 256, 8, 0, false },
    { &Mode2And3SL350, Data::Font8x16, 256, 0, 0, true },
    { &Mode2And3SL400, Data::Font8x16, 256, 16, 0, true },
    { &DefaultParameters, Data::Font8x16, 256, 16, 0, true }
};

// Mode 4, 5
ParameterFontPair Mode4And5Parameters[] =
{
    { &Mode4And5SL200, NULL, 0, 0, 0, false }
};

// Mode 6
ParameterFontPair Mode6Parameters[] =
{
    { &Mode6SL200, NULL, 0, 0, 0, false }
};

// Mode 7
ParameterFontPair Mode7Parameters[] =
{
    { &DefaultParameters, Data::Font8x8, 256, 8, 0, false },
    { &Mode7SL350, Data::Font8x16, 256, 0, 0, false },
    { &Mode7SL400, Data::Font8x16, 256, 16, 0, true },
    { &DefaultParameters, Data::Font8x16, 256, 16, 0, true }
};

// Mode D
ParameterFontPair ModeDParameters[] =
{
    { &ModeDSL200, NULL, 0, 0, 0, false }
};

// Mode E
ParameterFontPair ModeEParameters[] =
{
    { &ModeESL200, NULL, 0, 0, 0, false }
};

// Mode F
ParameterFontPair ModeFParameters[] =
{
    { &ModeFSL350, NULL, 0, 0, 0, false }
};

// Mode 10
ParameterFontPair Mode10Parameters[] =
{
    { &Mode10SL350, NULL, 0, 0, 0, false }
};

// Mode 11
ParameterFontPair Mode11Parameters[] =
{
    { &Mode11SL480, NULL, 0, 0, 0, false }
};

// Mode 12
ParameterFontPair Mode12Parameters[] =
{
    { &Mode12SL480,  NULL, 0, 0, 0, false }
};

// Mode 13
ParameterFontPair Mode13Parameters[] =
{
    { &Mode13SL200, NULL, 0, 0, 0, false }
};

ParameterFontPair G640x400Parameters[] =
{
    { &G640x400, NULL, 0, 0, 0, false }
};

ParameterFontPair G640x480Parameters[] =
{
    { &G640x480, NULL, 0, 0, 0, false }
};

ParameterFontPair G800x600x4Parameters[] =
{
    { &G800x600x4, NULL, 0, 0, 0, false }
};

ParameterFontPair G800x600Parameters[] =
{
    { &G800x600, NULL, 0, 0, 0, false }
};

ParameterFontPair G1024x768Parameters[] =
{
    { &G1024x768, NULL, 0, 0, 0, false }
};

ParameterFontPair G1280x1024Parameters[] =
{
    { &G1280x1024, NULL, 0, 0, 0, false }
};

ParameterFontPair G1600x1200Parameters[] =
{
    { &G1600x1200, NULL, 0, 0, 0, false }
};

//Mode: 3
//Frequency: 0x00000000, mnps: 0x00000000
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair T80x50Parameters[] =
{
    { &Mode80x50, Data::Font8x8, 256, 8, 0, false }
};

//Mode: 1B8
//Frequency: 0x00003159, mnps: 0x0001F077
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1B8Parameters[] =
{
    { &Mode1B8, NULL, 0, 0, 0, false }
};

//Mode: 1F9
//Frequency: 0x00003159, mnps: 0x0001F077
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1F9Parameters[] =
{
    { &Mode1F9, NULL, 0, 0, 0, false }
};

//Mode: 1F8
//Frequency: 0x00003159, mnps: 0x0001F077
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1F8Parameters[] =
{
    { &Mode1F8, NULL, 0, 0, 0, false }
};

//Mode: 1E9
//Frequency: 0x00003159, mnps: 0x0001F077
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1E9Parameters[] =
{
    { &Mode1E9, NULL, 0, 0, 0, false }
};

//Mode: 1D9
//Frequency: 0x00003159, mnps: 0x0001F077
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1D9Parameters[] =
{
    { &Mode1D9, NULL, 0, 0, 0, false }
};

//Mode: 1F5
//Frequency: 0x00005F41, mnps: 0x0003E76C
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1F5Parameters[] =
{
    { &Mode1F5, NULL, 0, 0, 0, false }
};

//Mode: 1E6
//Frequency: 0x00005F41, mnps: 0x0003E76C
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1E6Parameters[] =
{
    { &Mode1E6, NULL, 0, 0, 0, false }
};

//Mode: 1D6
//Frequency: 0x00005F41, mnps: 0x0003E76C
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1D6Parameters[] =
{
    { &Mode1D6, NULL, 0, 0, 0, false }
};

//Mode: 1F3
//Frequency: 0x00008FA3, mnps: 0x00026A70
//Horizontal extensions: 0x40
//Vertical extensions: 0x00
ParameterFontPair Mode1F3Parameters[] =
{
    { &Mode1F3, NULL, 0, 0, 0, false }
};
//Mode: 1B3
//Frequency: 0x00008FA3, mnps: 0x00026A70
//Horizontal extensions: 0x40
//Vertical extensions: 0x00
ParameterFontPair Mode1B3Parameters[] =
{
    { &Mode1B3, NULL, 0, 0, 0, false }
};

//Mode: 1E4
//Frequency: 0x00008FA3, mnps: 0x00026A70
//Horizontal extensions: 0x40
//Vertical extensions: 0x00
ParameterFontPair Mode1E4Parameters[] =
{
    { &Mode1E4, NULL, 0, 0, 0, false }
};
//Mode: 1D4
//Frequency: 0x00008FA3, mnps: 0x00026A70
//Horizontal extensions: 0x40
//Vertical extensions: 0x00
ParameterFontPair Mode1D4Parameters[] =
{
    { &Mode1D4, NULL, 0, 0, 0, false }
};

//Mode: 1B5
//Frequency: 0x00005F41, mnps: 0x0003E76C
//Horizontal extensions: 0x00
//Vertical extensions: 0x00
ParameterFontPair Mode1B5Parameters[] =
{
    { &Mode1B5, NULL, 0, 0, 0, false }
};

ParameterFontPair T80x60Parameters[] =
{
    { &T80x60, Data::Font8x8, 256, 8, 0, false }
};

ParameterFontPair T132x25Parameters[] =
{
    { &T132x25, Data::Font8x16, 256, 16, 0, true }
};

ParameterFontPair T132x43Parameters[] =
{
    { &T132x43, Data::Font8x8, 256, 8, 0, false }
};

ParameterFontPair T132x50Parameters[] =
{
    { &T132x50, Data::Font8x8, 256, 8, 0, false }
};

ParameterFontPair T132x60Parameters[] =
{
    { &T132x60, Data::Font8x8, 256, 8, 0, false }
};

// Mode Y
ParameterFontPair ModeYParameters[] =
{
    { &ModeYSL200, NULL, 0, 0, 0, false }
};

// Mode X
ParameterFontPair ModeXParameters[] =
{
    { &ModeXSL200, NULL, 0, 0, 0, false }
};

// Mode Q
ParameterFontPair ModeQParameters[] =
{
    { &ModeQSL200, NULL, 0, 0, 0, false }
};

PaletteData Data0x32f2 = 
{
    Hag::System::BDA::VideoDisplayDataArea::GrayScale | Hag::System::BDA::VideoDisplayDataArea::MonochromeMonitor,
    0x00,
    0x0040,
    {
        0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x28, 0x2A, 0x01, 0x03, 0x09, 0x0B, 0x21, 0x23, 0x29, 0x2B,
        0x04, 0x06, 0x0C, 0x0E, 0x24, 0x26, 0x2C, 0x2E, 0x05, 0x07, 0x0D, 0x0F, 0x25, 0x27, 0x2D, 0x2F,
        0x10, 0x12, 0x18, 0x1A, 0x30, 0x32, 0x38, 0x3A, 0x11, 0x13, 0x19, 0x1B, 0x31, 0x33, 0x39, 0x3B,
        0x14, 0x16, 0x1C, 0x1E, 0x34, 0x36, 0x3C, 0x3E, 0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F
    }
};

PaletteData Data0x3236 =
{
    0x00,
    0x80,
    0x0040,
    {
        0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x25, 0x28, 0x02, 0x07, 0x1B, 0x20, 0x0F, 0x14, 0x28, 0x2C,
        0x0C, 0x11, 0x25, 0x2A, 0x14, 0x1E, 0x32, 0x36, 0x0F, 0x13, 0x27, 0x2C, 0x1B, 0x20, 0x34, 0x39,
        0x06, 0x0B, 0x1F, 0x24, 0x13, 0x18, 0x2C, 0x30, 0x09, 0x0D, 0x21, 0x26, 0x15, 0x1A, 0x2E, 0x33,
        0x13, 0x17, 0x2B, 0x30, 0x1F, 0x24, 0x38, 0x3D, 0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F
    }
};

PaletteData Data0x337a =
{
    Hag::System::BDA::VideoDisplayDataArea::GrayScale | Hag::System::BDA::VideoDisplayDataArea::MonochromeMonitor,
    0x00,
    0x0040,
    {
        0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x24, 0x2A, 0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x24, 0x2A,
        0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F, 0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F,
        0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x24, 0x2A, 0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x24, 0x2A,
        0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F, 0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F
    }
};

PaletteData Data0x32be =
{
    0x00,
    0x80,
    0x0040,
    {
        0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x14, 0x28, 0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x14, 0x28,
        0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F, 0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F,
        0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x14, 0x28, 0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x14, 0x28,
        0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F, 0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F
    }
};

PaletteData Data0x3402 =
{
    0x00,
    0x00,
    0x0040,
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
        0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F
    }
};

PaletteData Data0x3446 =
{
    Hag::System::BDA::VideoDisplayDataArea::GrayScale | Hag::System::BDA::VideoDisplayDataArea::MonochromeMonitor,
    0x00,
    0x0010,
    {
        0x00, 0x02, 0x08, 0x0A, 0x20, 0x22, 0x24, 0x2A, 0x15, 0x17, 0x1D, 0x1F, 0x35, 0x37, 0x3D, 0x3F
    }
};

PaletteData Data0x345a =
{
    0x00,
    0x80,
    0x0010,
    {
        0x00, 0x05, 0x11, 0x1C, 0x08, 0x0B, 0x14, 0x28, 0x0E, 0x18, 0x2D, 0x32, 0x20, 0x24, 0x38, 0x3F
    }
};

PaletteData Data0x346e =
{
    0x00,
    0x80,
    0x0010,
    {
        0x00, 0x05, 0x08, 0x0B, 0x0E, 0x11, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2D, 0x32, 0x38, 0x3F
    }
};

PaletteData Data0x3482 =
{
    0x00,
    0x40,
    0x00D8,
    {
        0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00, 0x3F, 0x3F,
        0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F,
        0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x00,
        0x00, 0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 0x3F, 0x3F, 0x00,
        0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F,
        0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27,
        0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x37,
        0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F, 0x1F, 0x1F, 0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F,
        0x2F, 0x1F, 0x3F, 0x37, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F,
        0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D, 0x3F, 0x3F,
        0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36,
        0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D,
        0x2D, 0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 0x3F, 0x2D,
        0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00,
        0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07,
        0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x15,
        0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C,
        0x0E, 0x00, 0x1C, 0x15, 0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C,
        0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E, 0x1C, 0x1C,
        0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15,
        0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x0E,
        0x0E, 0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E, 0x1C, 0x1C, 0x0E,
        0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14,
        0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16,
        0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1A,
        0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C, 0x14, 0x14, 0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C,
        0x18, 0x14, 0x1C, 0x1A, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C,
        0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00, 0x10, 0x10,
        0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08,
        0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00,
        0x00, 0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 0x10, 0x00,
        0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08,
        0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A,
        0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x0E,
        0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10, 0x08, 0x08, 0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10,
        0x0C, 0x08, 0x10, 0x0E, 0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10,
        0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B, 0x10, 0x10,
        0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D,
        0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x0B,
        0x0B, 0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B, 0x10, 0x10, 0x0B,
        0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10
    }
};

PalettePair PalettePair0[] =
{
    { &Data0x32f2, &Data0x3236, 0 },
    { NULL, NULL, 0 }
};

PalettePair PalettePair2[] =
{
    { &Data0x337a, &Data0x32be, 0 },
    { NULL, NULL, 0 }
};

PalettePair PalettePair4[] =
{
    { &Data0x3402, NULL, 0 },
    { NULL, NULL, 0 }
};

PalettePair PalettePair578[] =
{
    { &Data0x3446, &Data0x345a, 0 },
    { &Data0x346e, NULL, 16 },
    { &Data0x3482, NULL, 32 },
    { NULL, NULL, 0 }
};

VideoMode s_VideoModes[] =
{
    { // Mode 7
        80,
        25,
        BitsPerPixel::Bpp1,
        Flags::Text | Flags::Monochrome | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::T80x25x1bppM,
        0xB000,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexB,
        Hag::System::BDA::DetectedHardware::Monochrome80x25,
        Mode7Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair4,
        32,//KB
        0,
        0,
        0
    },
    { // Mode 0, 1
        40,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::T40x25x4bppC,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode0And1Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        32,//KB
        0,
        0,
        0
    },
    { // Mode 2, 3: 25 row mode (font 8x16)
        80,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::MultiParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::T80x25x4bppC,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode2And3Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        32,//KB
        0,
        0,
        0
    },
    { // Mode 2, 3: 50 row mode (font 8x8)
        80,
        50,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::T80x25x4bppC,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T80x50Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        32,//KB
        0,
        0,
        0
    },
    { // Mode 108
        80,
        60,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::T80x60x4bpp & 0xFF) + 0x20,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T80x60Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        10,//KB
        25200,//KHz
        0,
        0
    },
    { // Mode 109
        132,
        25,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::T132x25x4bpp & 0xFF) + 0x20,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T132x25Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        7,//KB
        41080,//KHz
        0,
        0
    },
    { // Mode 10A
        132,
        43,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::T132x43x4bpp & 0xFF) + 0x20,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T132x43Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        11,//KB
        41080,//KHz
        0,
        0
    },
    { // Mode 10B
        132,
        50,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::T132x50x4bpp & 0xFF) + 0x20,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T132x50Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        13,//KB
        41080,//KHz
        0,
        0
    },
    { // Mode 10C
        132,
        60,
        BitsPerPixel::Bpp4,
        Flags::Text | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::T132x60x4bpp & 0xFF) + 0x20,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        T132x60Parameters,
        Scanlines::Invalid,
        Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair0,
        16,//KB
        41080,//KHz
        0,
        0
    },
    { // Mode 6
        640,
        200,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G640x200x1bppM,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode6Parameters,
        Scanlines::S200,
        Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics | Hag::System::BDA::CRTModeControlRegValue::Monochrome | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation,
        0x3F,
        PalettePair2,
        32,//KB
        0,
        0,
        0
    },
    { // Mode F
        640,
        350,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G640x350x1bppM,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexB,
        Hag::System::BDA::DetectedHardware::Monochrome80x25,
        ModeFParameters,
        Scanlines::S350,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair4,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 11
        640,
        480,
        BitsPerPixel::Bpp1,
        Flags::Graphics | Flags::Monochrome | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        Hag::VGA::VideoMode::G640x480x1bppM,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode11Parameters,
        Scanlines::S480,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair0,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 4, 5
        320,
        200,
        BitsPerPixel::Bpp2,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G320x200x2bppC,
        0xB800,
        0x4000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode4And5Parameters,
        Scanlines::S200,
        Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics | Hag::System::BDA::CRTModeControlRegValue::VideoEnabled | Hag::System::BDA::CRTModeControlRegValue::Blinking,
        0x30,
        PalettePair2,
        32,//KB
        0,
        0,
        0
    },
    { // Mode D
        320,
        200,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G320x200x4bppC,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        ModeDParameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair2,
        64,//KB
        0,
        0,
        0
    },
    { // Mode E
        640,
        200,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G640x200x4bppC,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        ModeEParameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair2,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 10
        640,
        350,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G640x350x4bppC,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode10Parameters,
        Scanlines::S350,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair0,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 12
        640,
        480,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        Hag::VGA::VideoMode::G640x480x4bppC,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode12Parameters,
        Scanlines::S480,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair0,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 102
        800,
        600,
        BitsPerPixel::Bpp4,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCDisable,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::G800x600x4bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G800x600x4Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair0,
        118,//KB
        40000,
        0,
        0
    },
    { // Mode 13
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        Hag::VGA::VideoMode::G320x200x8bppC,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode13Parameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        64,//KB
        0,
        0,
        0
    },
    { // Mode Y
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x14,//There is no official mode for this.
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        ModeYParameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        63,//KB
        0,
        0,
        0
    },
    { // Mode X
        320,
        240,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Planar | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x15,//There is no official mode for this.
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        ModeXParameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        64,//KB
        0,
        0,
        0
    },
    { // Mode Q
        256,
        256,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        0x16,//There is no official mode for this.
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        ModeQParameters,
        Scanlines::S200,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        64,//KB
        0,
        0,
        0
    },
    { // Mode 1F9
        320,
        200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        ((Hag::Vesa::VideoMode::G320x200x8bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        63,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1F8
        320,
        240,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        ((Hag::Vesa::VideoMode::G320x240x8bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F8Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        75,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1F5
        400,
        300,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        ((Hag::Vesa::VideoMode::G400x300x8bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F5Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        118,//KB
        24385,//KHz
        0x00,
        0x00
    },
    { // Mode 1F3
        512,
        384,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        ((Hag::Vesa::VideoMode::G512x384x8bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F3Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        192,//KB
        36771,//KHz
        0x00,
        0x00
    },
    {// Mode 100
        640,
        400,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x400x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x400Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        250,//KB
        27270,//KHz
        0x40,
        0
    },
    { // Mode 101
        640,
        480,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x480x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x480Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        300,//KB
        25175,
        0x40,
        0
    },
    { // Mode 103
        800,
        600,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G800x600x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G800x600Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        467,//KB
        40000,
        0,
        0
    },
    { // Mode 105
        1024,
        768,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1024x768x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1024x768Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        768,//KB
        65000,
        0,
        0
    },
    { // Mode 107
        1280,
        1024,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1280x1024x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1280x1024Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        1280,//KB
        108000,
        0x40,
        0x21
    },
    { // Mode 11C
        1600,
        1200,
        BitsPerPixel::Bpp8,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1600x1200x8bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1600x1200Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        PalettePair578,
        1875,//KB
        162000,
        0x01,
        0x2D
    },
    { // Mode 10D
        320,
        200,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G320x200x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        63,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1E9
        320,
        240,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        ((Hag::Vesa::VideoMode::G320x240x15bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1E9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        150,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1E6
        400,
        300,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        ((Hag::Vesa::VideoMode::G400x300x15bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1E6Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        235,//KB
        24385,//KHz
        0x00,
        0x00
    },
    { // Mode 1E4
        512,
        384,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        ((Hag::Vesa::VideoMode::G512x384x15bpp & 0xFF) + 0x20) - 0xB0,//Can't fit otherwise...
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1E4Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        384,//KB
        36771,//KHz
        0x00,
        0x00
    },
    {// Mode 100
        640,
        400,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x400x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x400Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        500,//KB
        27270,//KHz
        0x40,
        0
    },
    { // Mode 110
        640,
        480,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x480x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x480Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        600,//KB
        25175,
        0x40,
        0
    },
    { // Mode 113
        800,
        600,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G800x600x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G800x600Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        938,//KB
        40000,
        0,
        0
    },
    { // Mode 116
        1024,
        768,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1024x768x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1024x768Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        1536,//KB
        65000,
        0,
        0
    },
    { // Mode 119
        1280,
        1024,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1280x1024x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1280x1024Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        2560,//KB
        108000,
        0x40,
        0x21
    },
    { // Mode 11D
        1600,
        1200,
        BitsPerPixel::Bpp15,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1600x1200x15bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1600x1200Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        3750,//KB
        162000,
        0x01,
        0x2D
    },
    { // Mode 10E
        320,
        200,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G320x200x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        63,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1D9
        320,
        240,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::G320x240x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1D9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        150,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1D6
        400,
        300,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        (Hag::Vesa::VideoMode::G400x300x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1D6Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        235,//KB
        24385,//KHz
        0x00,
        0x00
    },
    { // Mode 1D4
        512,
        384,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G512x384x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1D4Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        384,//KB
        36771,//KHz
        0x00,
        0x00
    },
    {// Mode 100
        640,
        400,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x400x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x400Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        500,//KB
        27270,//KHz
        0x40,
        0
    },
    { // Mode 111
        640,
        480,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x480x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x480Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        600,//KB
        25175,
        0x40,
        0
    },
    { // Mode 114
        800,
        600,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G800x600x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G800x600Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        938,//KB
        40000,
        0,
        0
    },
    { // Mode 117
        1024,
        768,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1024x768x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1024x768Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        1536,//KB
        65000,
        0,
        0
    },
    { // Mode 11A
        1280,
        1024,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1280x1024x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1280x1024Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        2560,//KB
        108000,
        0x40,
        0x21
    },
    { // Mode 11E
        1600,
        1200,
        BitsPerPixel::Bpp16,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1600x1200x16bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1600x1200Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        3750,//KB
        162000,
        0x01,
        0x2D
    },
    { // Mode 10F
        320,
        200,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G320x200x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1F9Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        63,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1B8
        320,
        240,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R60Hz,
        (Hag::Vesa::VideoMode::G320x240x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1B8Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        300,//KB
        12633,//KHz
        0x00,
        0x00
    },
    { // Mode 1B5
        400,
        300,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R72Hz,
        (Hag::Vesa::VideoMode::G400x300x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1B5Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        768,//KB
        24385,//KHz
        0x00,
        0x00
    },
    { // Mode 1B3
        512,
        384,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G512x384x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        Mode1B3Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        384,//KB
        36771,//KHz
        0x00,
        0x00
    },
    {// Mode 100
        640,
        400,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x400x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x400Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        1000,//KB
        27270,//KHz
        0x40,
        0
    },
    { // Mode 112
        640,
        480,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G640x480x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G640x480Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        1200,//KB
        25175,
        0x40,
        0
    },
    { // Mode 115
        800,
        600,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G800x600x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G800x600Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        1875,//KB
        40000,
        0,
        0
    },
    { // Mode 118
        1024,
        768,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1024x768x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1024x768Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        3072,//KB
        65000,
        0,
        0
    },
    { // Mode 11B
        1280,
        1024,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1280x1024x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1280x1024Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        5120,//KB
        108000,
        0x40,
        0x21
    },
    { // Mode 11F
        1600,
        1200,
        BitsPerPixel::Bpp32,
        Flags::Graphics | Flags::Color | Flags::Sequential | Flags::LinearFramebuffer | Flags::SingleParameter | Flags::MAFCVGA,
        RefreshRate::R70Hz,
        (Hag::Vesa::VideoMode::G1600x1200x32bpp & 0xFF) + 0x20,
        0xA000,
        0x8000,
        Hag::VGA::Register::CRTControllerIndexD,
        Hag::System::BDA::DetectedHardware::Color80x25,
        G1600x1200Parameters,
        Scanlines::Invalid,
        0xFF,//CRTModeControlRegValue
        0xFF,//CGAColorPaletteMaskSetting
        NULL,
        7500,//KB
        162000,
        0x01,
        0x2D
    },
};

uint32_t s_VideoModeCount = sizeof(s_VideoModes) / sizeof(VideoMode);

VideoMode* Get(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate)
{
    if (!System::s_Initialized)
        return NULL;

    for (uint32_t i = 0; i < sizeof(s_VideoModes) / sizeof(VideoMode); ++i)
    {
        VideoMode& mode = s_VideoModes[i];
        if (mode.Width == width &&
            mode.Height == height &&
            mode.Bpp == bpp &&
            (mode.Flags & (Flags::Chromacity | Flags::Mode | Flags::MemoryOrganization)) == flags &&
            ((mode.RefreshRate == refreshRate) || refreshRate == RefreshRate::DontCare))
        {
            if (mode.MemSizeKB > System::s_MemorySize)
                continue;
            
            //TODO RAMDAC Max frequency filter.
            return &mode;
        }
    }
    return NULL;
}

bool Has(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate)
{
    return Get(width, height, bpp, flags, refreshRate) != NULL;
}

VideoMode* ConfigureEGAFeatureBitSwitchesAdapter(VideoMode* videoMode, Hag::System::BDA::VideoModeOptions_t& videoModeOptions)
{
    using namespace Hag::System;

    struct TestValues
    {
        BDA::VideoDisplayDataArea_t VideoDisplayDataArea;
        VGA::Register_t CRTControllerIndexPort;
        BDA::DetectedHardware_t DetectedHardware1;
        BDA::DetectedHardware_t DetectedHardware2;
        BDA::VideoModeOptions_t VideoModeOptions;
    };

    static TestValues Monochrome = 
    {
        0x80,
        VGA::Register::CRTControllerIndexB,
        0xFF,
        BDA::DetectedHardware::Monochrome80x25,
        BDA::VideoModeOptions::Monochrome
    };

    static TestValues Color =
    {
        0x01,
        VGA::Register::CRTControllerIndexD,
        BDA::DetectedHardware::Monochrome80x25,
        0x00,
        BDA::VideoModeOptions::Color
    };

    enum
    {
        KeepOriginal = 0,
        Select80x25x1bppM = 1,
        Select80x25x4bppC = 2
    };

    uint8_t modeSelect = KeepOriginal;

    if (((BDA::VideoDisplayDataArea::Get() & BDA::VideoDisplayDataArea::VGA) != 0) &&
        (videoMode->LegacyMode != BDA::DisplayMode::Get()))
    {
        TestValues& config = (videoMode->ColorOrMonochromeText == BDA::DetectedHardware::Monochrome80x25) ? Monochrome : Color;

        if (BDA::VideoBaseIOPort::Get() != config.CRTControllerIndexPort)
        {
            modeSelect = Select80x25x1bppM;

            if ((BDA::DetectedHardware::Get() & BDA::DetectedHardware::InitialVideoModeMask) != config.DetectedHardware1)
            {
                modeSelect = Select80x25x4bppC;

                if ((BDA::DetectedHardware::Get() & BDA::DetectedHardware::InitialVideoModeMask) >= config.DetectedHardware2)
                {
                    modeSelect = KeepOriginal;

                    videoModeOptions &= ~BDA::VideoModeOptions::Monochrome;
                    videoModeOptions |= config.VideoModeOptions;

                    BDA::EGAFeatureBitSwitches_t bitSwitches = BDA::EGAFeatureBitSwitches::Get();
                    bitSwitches &= BDA::EGAFeatureBitSwitches::AdapterTypeMask;
                    bitSwitches -= BDA::EGAFeatureBitSwitches::MDAColor40x25_2;

                    if (bitSwitches <= BDA::EGAFeatureBitSwitches::CGAMono80x25)
                    {
                        bitSwitches -= BDA::EGAFeatureBitSwitches::MDAHiResEnhanced;

                        uint8_t mask = ~((bitSwitches >> 7) - 1);
                        BDA::EGAFeatureBitSwitches_t adapter = BDA::EGAFeatureBitSwitches::CGAMono80x25_2;

                        if ((bitSwitches != 0) && ((bitSwitches & 0x80) == 0))
                            adapter = BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2;

                        BDA::VideoDisplayDataArea_t displayDataArea = BDA::VideoDisplayDataArea::Get();
                        uint8_t lineMode200 = displayDataArea;

                        displayDataArea &= ~BDA::VideoDisplayDataArea::LineMode200;
                        lineMode200 &= BDA::VideoDisplayDataArea::LineMode200;

                        lineMode200 >>= 7;
                        lineMode200 &= config.VideoDisplayDataArea;

                        adapter -= lineMode200;
                        displayDataArea |= mask & config.VideoDisplayDataArea;

                        BDA::EGAFeatureBitSwitches::Get() &= BDA::EGAFeatureBitSwitches::FeatureConnectorMask;
                        BDA::EGAFeatureBitSwitches::Get() |= adapter;
                        BDA::VideoDisplayDataArea::Get() = displayDataArea;
                    }
                }
            }
        }
    }
    if (modeSelect == Select80x25x4bppC)
    {
        videoMode = Get(80, 25, BitsPerPixel::Bpp4, Flags::Text | Flags::Color | Flags::Sequential, RefreshRate::R70Hz);
    }
    if (modeSelect == Select80x25x1bppM)
    {
        videoMode = Get(80, 25, BitsPerPixel::Bpp1, Flags::Text | Flags::Monochrome | Flags::Sequential, RefreshRate::R70Hz);
    }
    return videoMode;
}

bool IsMDAHiResEnhanced()
{
    using namespace Hag::System;

    BDA::EGAFeatureBitSwitches_t adapterType = BDA::EGAFeatureBitSwitches::Get() &
                                                BDA::EGAFeatureBitSwitches::AdapterTypeMask;

    return (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced) ||
        (adapterType == BDA::EGAFeatureBitSwitches::MDAHiResEnhanced_2);
}

Scanlines_t GetNumberOfActiveScanlines(VideoMode& videoMode)
{
    using namespace Hag::System;

    Scanlines_t scanlines = videoMode.Scanlines;

    if (scanlines == Scanlines::Invalid)
    {
        scanlines = Scanlines::S400;

        if ((BDA::VideoDisplayDataArea::Get() & BDA::VideoDisplayDataArea::LineMode400) == 0)
        {
            scanlines = Scanlines::S350;

            if (!IsMDAHiResEnhanced() &&
                (BDA::DisplayMode::Get() != VGA::VideoMode::T80x25x1bppM)) //TODO: I don't like this comparison...
            {
                scanlines = Scanlines::S200;
            }
        }
    }
    return scanlines;
}

void LoadColorPalette(PaletteData* palette)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint8_t greyscale = 0;
    uint16_t colorIndex = 0;

    uint8_t* colors = palette->Colors;

    if ((VideoDisplayDataArea::Get() & 
        (VideoDisplayDataArea::GrayScale | VideoDisplayDataArea::MonochromeMonitor)) == 0x00)
    {
        uint16_t tripleCount = palette->Count * 3;
        for (uint16_t idx = 0; idx < tripleCount; ++idx)
        {
            RAMDACData::Write(*(colors++));
        }
    }
    else
    {
        for (uint16_t i = 0; i < palette->Count; ++i)
        {
            red = *(colors++);
            green = *(colors++);
            blue = *(colors++);
            greyscale = ((0x4D * uint16_t(red)) +
                            (0x97 * uint16_t(green)) +
                            (0x1c * uint16_t(blue)) + 0x80) >> 8;
            RAMDACData::Write(greyscale);
            RAMDACData::Write(greyscale);
            RAMDACData::Write(greyscale);
        }
    }
}

void LoadCompressedPalette(PaletteData* palette)
{
    using namespace Hag::VGA;

    uint8_t* colors = palette->Colors;

    for (uint16_t i = 0; i < palette->Count; ++i)
    {
        uint8_t packedValue = *(colors++) << 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
        packedValue <<= 2;
        RAMDACData::Write((packedValue >> 6) * 0x15);
    }
}

void LoadMonochromePalette(PaletteData* palette)
{
    using namespace Hag::VGA;

    uint8_t* colors = palette->Colors;
    for (uint16_t idx = 0; idx < palette->Count; ++idx)
    {
        uint8_t color = *(colors++);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
        RAMDACData::Write(color);
    }
}

void InitializeRAMDACPalette(VideoMode& videoMode)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    DACMask::Write(0xFF);

    if (videoMode.Palettes != NULL)
    {
        PalettePair* palettePair = videoMode.Palettes;
        while (palettePair->Primary != NULL)
        {
            PaletteData* palette = ((palettePair->Alternate != NULL) && 
                                    ((VideoDisplayDataArea::Get() & palettePair->Primary->Mask) != 0)) ?
                                    palettePair->Alternate : palettePair->Primary;

            DACWriteIndex::Write(palettePair->StartIndex);

            if (palette->Flags == 0x00)
            {
                LoadCompressedPalette(palette);
            }
            else if ((palette->Flags & 0x80) == 0x00)
            {
                LoadColorPalette(palette);
            }
            else
            {
                LoadMonochromePalette(palette);
            }
            ++palettePair;
        }
    }
    else
    {
        VGA::DACWriteIndex::Write(0);
        switch(videoMode.Bpp)
        {
        case BitsPerPixel::Bpp32:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol);
                VGA::RAMDACData::Write(idxcol);
                VGA::RAMDACData::Write(idxcol);
            }
            break;
        case BitsPerPixel::Bpp16:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 2);
                VGA::RAMDACData::Write(idxcol << 3);
            }
            break;
        default:
            for (int idxcol = 0; idxcol < 256; ++idxcol)
            {
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 3);
                VGA::RAMDACData::Write(idxcol << 3);
            }
            break;
        }
    }
}

void InitializePalettes(VideoMode& videoMode, Hag::System::BDA::VideoParameterTable& videoParameterTable)
{
    using namespace Hag::VGA;
    using namespace Hag::System::BDA;

    if ((VideoDisplayDataArea::Get() & VideoDisplayDataArea::PaletteLoadingDisabled) == 0)
    {
        AttributeControllerData::Write(AttributeController::Register::Palette0, videoParameterTable.AttributeControllerRegisters, 16);
        AttributeController::BorderColor::Write(
            videoParameterTable.AttributeControllerRegisters[AttributeController::Register::BorderColor]);
        InitializeRAMDACPalette(videoMode);
    }
}

Hag::VGA::Sequencer::ClockingMode_t ToggleScreenOnOff(Hag::VGA::Sequencer::ClockingMode_t screenToggle)
{
    VGA::Sequencer::ClockingMode_t originalClockingMode = VGA::Sequencer::ClockingMode::Read();

    screenToggle &= VGA::Sequencer::ClockingMode::ScreenOff;

    VGA::Sequencer::ClockingMode_t newClockingMode = (originalClockingMode & ~VGA::Sequencer::ClockingMode::ScreenOff) | screenToggle;

    VGA::Sequencer::ClockingMode::Write(newClockingMode);

    return originalClockingMode;
}

Hag::VGA::Sequencer::ClockingMode_t TurnScreenOn()
{
    return ToggleScreenOnOff(VGA::Sequencer::ClockingMode::ScreenOn);
}

Hag::VGA::Sequencer::ClockingMode_t TurnScreenOff()
{
    return ToggleScreenOnOff(VGA::Sequencer::ClockingMode::ScreenOff);
}

bool IsExtensionReg7Writeable()
{
    Shared::CRTCExtensionIndex::Write(0x07);
    Shared::CRTCExtensionData_t save = Shared::CRTCExtensionData::Read();
    Shared::CRTCExtensionData::Write(0x07, 0x0A);
    bool equal = Shared::CRTCExtensionData::Read() == 0x0A;
    Shared::CRTCExtensionData::Write(0x07, save);
    return equal;
}

uint8_t Data0x7814 = 0xEE;

void ResetCRTCExtensionRegisters()
{
    if (!IsExtensionReg7Writeable())
    {
        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
            ~(Shared::Indexed::MiscellaneousControl::MAFCFuncSelect | Shared::Indexed::MiscellaneousControl::VGADACBitDepth));

        Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device, Shared::Indexed::MultiplexControl::Bits8p);

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            ~Shared::Indexed::GeneralControl::PedestalControl) |
            ((Data0x7814 & 0x01) << 4));

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            (Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            ~Shared::Indexed::GeneralControl::GreenChannelSync) |
            (Data0x7814 & Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::GeneralControl::Write(System::s_Device,
            Shared::PCI::Indexed::GeneralControl::Read(System::s_Device) &
            (Shared::Indexed::GeneralControl::PedestalControl |
            Shared::Indexed::GeneralControl::GreenChannelSync));

        Shared::PCI::Indexed::CursorControl::Write(System::s_Device,
            Shared::Indexed::CursorControl::Disabled);

        Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
            (Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
            ~Shared::Indexed::PixelClockControl::ClockSelection) |
            Shared::Indexed::PixelClockControl::ClockPLL);
    }

    Shared::CRTCExtension::AddressGeneratorExtensions::Write(0x00);

    Shared::CRTCExtension::Miscellaneous::Write(
        (Data0x7814 & (Shared::CRTCExtension::Miscellaneous::CompositeSyncEnable >> 2)) << 2);

    Shared::CRTCExtension::MemoryPage::Write(0x00);
    Shared::CRTCExtension::HorizontalHalfCount::Write(0x00);
}

void ApplyVideoParameters(Hag::System::BDA::VideoParameterTable& videoParameterTable, Hag::VGA::Register_t baseVideoIOPort)
{
    SYS_ClearInterrupts();
    
    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset);
        
    VGA::SequencerData::Write(VGA::Sequencer::Register::ClockingMode,
                            videoParameterTable.SequencerRegisters,
                            sizeof(videoParameterTable.SequencerRegisters));

    VGA::MiscellaneousOutput::Write(videoParameterTable.MiscellaneousOutputRegister);
    
    for (int i = 0; i < 8000; ++i); //BOOOO

    VGA::Sequencer::Reset::Write(VGA::Sequencer::Reset::AsynchronousReset | VGA::Sequencer::Reset::SynchronousReset);

    SYS_RestoreInterrupts();

    VGA::CRTController::VerticalRetraceEnd::Write(baseVideoIOPort, 0x00);

    VGA::CRTControllerData::Write(baseVideoIOPort,
                                VGA::CRTController::Register::HorizontalTotal,
                                videoParameterTable.CRTCRegisters,
                                sizeof(videoParameterTable.CRTCRegisters));

    VGA::InputStatus1::Read(baseVideoIOPort + 0x06);
    VGA::FeatureControl::Write(baseVideoIOPort + 0x06, 0x00);

    VGA::AttributeController::AttributeMode::Write(
        videoParameterTable.AttributeControllerRegisters[VGA::AttributeController::Register::AttributeMode]);

    VGA::AttributeController::ColorPlane::Write(
        videoParameterTable.AttributeControllerRegisters[VGA::AttributeController::Register::ColorPlane]);
    VGA::AttributeController::HorizontalPixelPanning::Write(
        videoParameterTable.AttributeControllerRegisters[VGA::AttributeController::Register::HorizontalPixelPanning]);

    VGA::GraphicsControllerData::Write(VGA::GraphicsController::Register::SetResetData,
                                    videoParameterTable.GraphicsControllerRegisters,
                                    sizeof(videoParameterTable.GraphicsControllerRegisters));
}

void ClearScreen(VideoMode& videoMode)
{
    using namespace Hag::System;

    if (((BDA::VideoModeOptions::Get() & BDA::VideoModeOptions::DontClearDisplay) == 0) &&
        (BDA::VideoBufferSize::Get() != 0x0000))//This is probably VESA related...
    {
        uint16_t value = ((videoMode.Flags & Flags::Mode) == Flags::Text) ? 0x0720 : 0x0000;

        VGA::Sequencer::EnableWritePlane_t writePlane = VGA::Sequencer::EnableWritePlane::Read();
        if ((videoMode.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            VGA::Sequencer::EnableWritePlane::Write(
                VGA::Sequencer::EnableWritePlane::Plane1 |
                VGA::Sequencer::EnableWritePlane::Plane2 |
                VGA::Sequencer::EnableWritePlane::Plane3 |
                VGA::Sequencer::EnableWritePlane::Plane4);
        }

        uint16_t pages = max<uint16_t>((videoMode.MemSizeKB + 63)  >> 6, 1);
        for (uint16_t page = 0; page < pages; ++page)
        {
            Shared::CRTCExtension::MemoryPage::Write(page);
            uint16_t* ptr = FARPointer(videoMode.Segment, 0x0000).ToPointer<uint16_t>(videoMode.ClearCount << 1);
            if (value != 0)
            {
                for (uint16_t i = 0; i < videoMode.ClearCount; ++i)
                {
                    *ptr = value;
                    ++ptr;
                }
            }
            else
            {
                memset(ptr, 0, videoMode.ClearCount << 1);
            }
        }
        Shared::CRTCExtension::MemoryPage::Write(0);

        if ((videoMode.Flags & Flags::MemoryOrganization) == Flags::Planar)
        {
            VGA::Sequencer::EnableWritePlane::Write(writePlane);
        }
    }
}

void ConfigureFontLoadMemoryMapping()
{
    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane3);
    Shared::Sequencer::MemoryModeControl::Write(
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::Unknown1 |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::ExtendedMemoryAddress |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::SequentialAddressingMode);
    
    VGA::GraphicsController::ReadPlaneSelect::Write(VGA::GraphicsController::ReadPlaneSelect::Plane3);
    VGA::GraphicsController::GraphicsControllerMode::Write(VGA::GraphicsController::GraphicsControllerMode::Mode0);
    VGA::GraphicsController::MemoryMapModeControl::Write(VGA::GraphicsController::MemoryMapModeControl::A0000HtoAFFFFH);
}

void ConfigureTextMemoryMapping()
{
    using namespace Hag::System;

    VGA::Sequencer::EnableWritePlane::Write(VGA::Sequencer::EnableWritePlane::Plane1 | VGA::Sequencer::EnableWritePlane::Plane2);
    Shared::Sequencer::MemoryModeControl::Write(
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::Unknown1 |
        Hag::Matrox::Shared::Sequencer::MemoryModeControl::ExtendedMemoryAddress);

    VGA::GraphicsController::ReadPlaneSelect::Write(VGA::GraphicsController::ReadPlaneSelect::Plane1);
    VGA::GraphicsController::GraphicsControllerMode::Write(VGA::GraphicsController::GraphicsControllerMode::OddEvenAddressing);
    if (BDA::VideoBaseIOPort::Get() == VGA::Register::CRTControllerIndexD)
        VGA::GraphicsController::MemoryMapModeControl::Write(
            VGA::GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            VGA::GraphicsController::MemoryMapModeControl::B8000HtoBFFFFH);
    else
        VGA::GraphicsController::MemoryMapModeControl::Write(
            VGA::GraphicsController::MemoryMapModeControl::ChainOddEvenPlanes |
            VGA::GraphicsController::MemoryMapModeControl::B0000HtoB7FFFH);
}

void UploadFont(ParameterFontPair& parameterFontPair)
{
    static uint8_t Bank[] = { 0x00, 0x40, 0x80, 0xC0, 0x20, 0x60, 0xA0, 0xE0 };

    uint16_t offset = 0;
    uint16_t count = parameterFontPair.CharacterCount;
    uint8_t* font = parameterFontPair.Font;
    do
    {
        FARPointer ptr(0xA000, offset + (uint16_t(Bank[parameterFontPair.BankIndex]) << 8));            
        uint8_t span = parameterFontPair.CharacterHeight == 0 ? 16 : parameterFontPair.CharacterHeight;

        do
        {
            if ((parameterFontPair.CharacterHeight == 0) &&
                ((uint8_t(count) == 0x68) ||
                (uint8_t(count) == 0x87) ||
                (uint8_t(count) == 0x8F) ||
                (uint8_t(count) == 0x90) ||
                (uint8_t(count) == 0x96) ||
                (uint8_t(count) == 0x99)))
            {
                memcpy(ptr.ToPointer<uint8_t>(8), font, 8);
                ptr.Offset += 8;
                font += 9;

                memcpy(ptr.ToPointer<uint8_t>(7), font, 7);
                ptr.Offset += 8;
                font += 7;
            }
            else
            {
                memcpy(ptr.ToPointer<uint8_t>(span), font, span);
                ptr.Offset += span;
                font += span;
            }

            ptr.Offset += 32 - span;
            --count;

        } while (count != 0);

        if (!parameterFontPair.Patch)
            break;

        offset = uint16_t(*font) << 5;
        ++font;
        ++count;
    } while (offset != 0);
}

void ApplyGraphicsCharacterSetOverride()
{
    using namespace Hag::System;
    BDA::VideoParameterControlBlock* videoParameterControlBlock = 
        BDA::VideoParameterControlBlockPointer::Get().ToPointer<BDA::VideoParameterControlBlock>();
    
    if (!videoParameterControlBlock->GraphicsCharacterSetOverride.IsNull())
    {
        BDA::GraphicsCharacterSet* graphicsCharacterSet = 
            videoParameterControlBlock->GraphicsCharacterSetOverride.ToPointer<BDA::GraphicsCharacterSet>(sizeof(BDA::GraphicsCharacterSet) + 20);
        uint8_t* videoModes = graphicsCharacterSet->ApplicableVideoModes;
        while (*videoModes != 0xFF)
        {
            if (*videoModes == BDA::DisplayMode::Get())
                break;
            ++videoModes;
        }
        if (*videoModes == BDA::DisplayMode::Get())
        {
            BDA::RowsOnScreen::Get() = graphicsCharacterSet->NumberOfCharacterRowsDisplayed - 1;
            BDA::PointHeightOfCharacterMatrix::Get() = graphicsCharacterSet->CharacterLength;
            InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = graphicsCharacterSet->CharacterFontDefinitionTable;
        }
    }
}

void SetInterruptTableFontPointer(VideoMode& videoMode)
{
    using namespace Hag::System;
    if (GetNumberOfActiveScanlines(videoMode) != Scanlines::S200)
    {
        InterruptTable::Pointer<InterruptTable::CharacterTable>() = System::s_Font8x16;
    }
    BDA::CursorScanLines::Get().End = 0;
    BDA::CursorScanLines::Get().Start = 0;
    ApplyGraphicsCharacterSetOverride();
}

void AdjustOffset(uint8_t shift)
{
    uint16_t offset = (uint16_t(Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
                                Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8) << 4) |
                                VGA::CRTController::ScreenOffset::Read(VGA::Register::CRTControllerIndexD);

    if (shift == 0x00)
    {
        offset >>= 1;
    }
    else
    {
        offset <<= shift;
    }
    
    VGA::CRTController::ScreenOffset::Write(VGA::Register::CRTControllerIndexD, uint8_t(offset));

    Shared::CRTCExtension::AddressGeneratorExtensions::Write(((offset >> 4) &
        Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8) |
        (Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
        ~Shared::CRTCExtension::AddressGeneratorExtensions::Offset9_8));
}

void ApplyVesaSettings(VideoMode* videoMode)
{
    using namespace Hag::System;

    uint8_t offsetAdjust = 0;
    
    if ((videoMode->Flags & Flags::MAFCSelection) == Flags::MAFCVGA)
    {
        Shared::PCI::Option::WriteByte1(System::s_Device,
            Shared::PCI::Option::ReadByte1(System::s_Device) &
            ~Shared::PCI::Option::SplitMode);

        Shared::CRTCExtension::MemoryPage::Write(0x00);

        if (videoMode->Bpp == BitsPerPixel::Bpp32)
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x03;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                Shared::Indexed::MultiplexControl::Bits24p8x);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv4 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);

            offsetAdjust = 2;

        }
        else if ((videoMode->Bpp == BitsPerPixel::Bpp15) ||
                 (videoMode->Bpp == BitsPerPixel::Bpp16))
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x02;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
                Shared::Indexed::MiscellaneousControl::VGADAC8Bit);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                (videoMode->Bpp == BitsPerPixel::Bpp15) ?
                Shared::Indexed::MultiplexControl::Bits15p :
                Shared::Indexed::MultiplexControl::Bits16p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv2 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);

            offsetAdjust = 0;
        }
        else
        {
            BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2) | 0x01;

            Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
                Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
                ~Shared::Indexed::MiscellaneousControl::VGADACBitDepth);

            Shared::PCI::Indexed::MultiplexControl::Write(System::s_Device,
                Shared::Indexed::MultiplexControl::Bits8p);

            Shared::CRTCExtension::Miscellaneous::Write(
                Shared::CRTCExtension::Miscellaneous::ScaleDiv1 |
                Shared::CRTCExtension::Miscellaneous::MGAModeEnable);

            offsetAdjust = 1;
        }

        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) |
            Shared::Indexed::MiscellaneousControl::MAFCDisable);

        Shared::CRTCExtension::HorizontalHalfCount::Write(1);

        if (offsetAdjust != 0)
        {
            AdjustOffset(offsetAdjust - 1);
        }
    }
    else
    {
        BDA::VideoBufferSize::Get() = (System::s_MemorySize << 2);

        Shared::PCI::Indexed::MiscellaneousControl::Write(System::s_Device,
            Shared::PCI::Indexed::MiscellaneousControl::Read(System::s_Device) &
            ~Shared::Indexed::MiscellaneousControl::MAFCFuncSelect);
    }
}

void SetStartAddress(uint32_t address)
{
    VGA::CRTController::StartAddressLow::Write(VGA::Register::CRTControllerIndexD, uint8_t(address));
    VGA::CRTController::StartAddressHigh::Write(VGA::Register::CRTControllerIndexD, uint8_t(address >> 8));
    Shared::CRTCExtension::AddressGeneratorExtensions::Write(
            (Shared::CRTCExtension::AddressGeneratorExtensions::Read() &
                ~Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16) |
                (uint8_t(address >> 16) & Shared::CRTCExtension::AddressGeneratorExtensions::StartAddress19_16));
}

void ApplyMode(VideoMode* videoMode, Hag::System::BDA::VideoModeOptions_t videoModeOptions)
{
    using namespace Hag::System;

    BDA::DisplayMode::Get() = videoMode->LegacyMode;
    BDA::VideoModeOptions::Get() = videoModeOptions;
    BDA::VideoBaseIOPort::Get() = videoMode->CrtController;

    ResetCRTCExtensionRegisters();
    videoMode = ConfigureEGAFeatureBitSwitchesAdapter(videoMode, videoModeOptions);

    BDA::ActiveDisplayNumber::Get() = 0;
    BDA::VideoBufferOffset::Get() = 0;
    BDA::CursorPositions::Clear();

    BDA::VideoParameterTable& videoParameterTable = ((videoMode->Flags & Flags::ParameterCount) == Flags::MultiParameter) ? 
        *videoMode->ParametersAndFonts[GetNumberOfActiveScanlines(*videoMode)].VideoParameters :
        *videoMode->ParametersAndFonts[0].VideoParameters;

    BDA::NumberOfScreenColumns::Get() = videoParameterTable.NumCharacterColumns;
    BDA::RowsOnScreen::Get() = videoParameterTable.NumScreenRowsMinus1;
    BDA::PointHeightOfCharacterMatrix::Get() = videoParameterTable.CharacterMatrixHeightInPoints;

    if (videoMode->FrequencyKHz == 0)//Standard VGA
        BDA::VideoBufferSize::Get() = videoParameterTable.VideoBufferSize;

    BDA::CursorScanLines::Get().Start = videoParameterTable.CRTCRegisters[VGA::CRTController::Register::CursorStartScanLine];
    BDA::CursorScanLines::Get().End = videoParameterTable.CRTCRegisters[VGA::CRTController::Register::CursorEndScanLine];

    ApplyVideoParameters(videoParameterTable, BDA::VideoBaseIOPort::Get());

    VGA::AttributeController::PixelPadding::Write(0);

    if (videoMode->FrequencyKHz != 0)
        ApplyVesaSettings(videoMode);

    InitializePalettes(*videoMode, videoParameterTable);

    ClearScreen(*videoMode);

    if ((videoMode->Flags & Flags::Mode) == Flags::Text)
    {
        ParameterFontPair& parameterFontPair = ((videoMode->Flags & Flags::ParameterCount) == Flags::MultiParameter) ? 
            videoMode->ParametersAndFonts[GetNumberOfActiveScanlines(*videoMode)] :
            videoMode->ParametersAndFonts[0];

        if (parameterFontPair.Font != NULL)
        {
            ConfigureFontLoadMemoryMapping();
            UploadFont(parameterFontPair);
            ConfigureTextMemoryMapping();
        }
    }
    else
        SetInterruptTableFontPointer(*videoMode);

    if (videoMode->FrequencyKHz != 0)
        SetStartAddress(0);

    VGA::InputStatus1::Read(BDA::VideoBaseIOPort::Get() + 0x06);
    VGA::AttributeControllerIndex::Write(VGA::AttributeControllerIndex::EnableVideoDisplay);
    
    if (videoMode->CRTModeControlRegValue != 0xFF)
        BDA::CRTModeControlRegValue::Get() = videoMode->CRTModeControlRegValue;

    if (videoMode->CGAColorPaletteMaskSetting != 0xFF)
        BDA::CGAColorPaletteMaskSetting::Get() = videoMode->CGAColorPaletteMaskSetting;

    BDA::VideoDisplayDataArea::Get() &= ~BDA::VideoDisplayDataArea::Reserved;
}

//
//Calculate the M, N, P and S values for the PLL's given an input frequency in KHz
//
//inputs:
//   eax = requested frequency in KHz
//
//outputs:
//   ebx = M,N,P and S values
//         bl holds N in lower 7 bits
//         bh holds M in lower 5 bits
//         bh holds P in upper 3 bits
//         S 2 bits sit above bh
//         00000000 000000SS PPPMMMMM xNNNNNNN
//
uint32_t CalculatePLL_MNPS(uint32_t requestedFrequencyKHz)//Offset 0x5e4b
{
    requestedFrequencyKHz = min<uint32_t>(max<uint32_t>(requestedFrequencyKHz, 6262), 218864);
    
    uint32_t mnps = 0;
    uint32_t difference = 218864;
    uint32_t frequency = 220000;

    for (uint16_t N = 100; N < 128; ++N)
    {
        for (uint16_t M = 0; M < 32; ++M)
        {
            uint32_t approx = uint32_t((14318 * (N + 1)) / (M + 1));

            if (approx < 50000)
                continue;

            if (approx > 220000)
                continue;

            for (uint8_t P = 0; P < 4; ++P)
            {
                uint32_t attempt = approx / (1 << P);

                if (attempt < requestedFrequencyKHz)
                {
                    attempt = requestedFrequencyKHz - attempt;
                }
                else
                {
                    attempt = attempt - requestedFrequencyKHz;
                }

                if (attempt > difference)
                    continue;

                bool same = attempt == difference;

                difference = attempt;

                if (same && (approx >= frequency))
                    continue;

                frequency = approx;

                uint32_t S = 0;
                if (frequency <= 100000) S = 0;
                else if (frequency <= 140000) S = 1 << 16;
                else if (frequency <= 180000) S = 2 << 16;
                else S = 3 << 16;

                mnps = N | (M << 8);
                mnps |= ((1 << P) - 1) << 13;
                mnps |= S;
            }
        }
    }
    return mnps;
}

void SetupSquareWave()
{
    using namespace Hag::System;

    PIT::Command::Write(PIT::Command::ModeSquareWaveGenerator | PIT::Command::LowByteHighByte | PIT::Command::SelectChannel2);
    PIT::Data::WriteChannel2(0xA9);
    PIT::Data::WriteChannel2(0x04);
}

void Sleep2(uint8_t count)//count * 8 * 1193 ticks time
{
    using namespace Hag::System;

    SetupSquareWave();

    KB::Register_t port = (BIOS::MachineID::Read() == 0xFC) ? KB::Register::PortB : KB::Register::PortXT;

    count <<= 3;

    if (count != 0)
    {
        KB::PortB_t orgValue = KB::PortBStatus::Read();
        KB::PortBCommand::Write(orgValue |= KB::PortBCommand::Timer2GateSpeakerEnable);

        do
        {
            volatile KB::PortB_t status;
            do
            {
                status = KB::Read(port);
            } while ((status & KB::PortBStatus::MirrorTimer2OutputCond) != 0);

            do
            {
                status = KB::Read(port);
            } while ((status & KB::PortBStatus::MirrorTimer2OutputCond) != 0);
            --count;
        } while (count != 0);

        KB::PortBCommand::Write(orgValue);
    }
}

typedef uint8_t PixelClocksSettings_t;
namespace PixelClocksSettings
{
    enum
    {
        PLLSet = 0x03,
        PLLSetA = 0x00,
        PLLSetB = 0x01,
        PLLSetC = 0x02,
        Clock = 0xC,
        ClockPCI = 0x00,
        ClockPLL = 0x04,
        ClockVDOCLK = 0x08
    };
}

void ConfigurePixelClocks(uint32_t mnps, PixelClocksSettings_t PllAndClock)
{
    //5.7.8.3 (A) Step 1: Force the screen off.
    VGA::Sequencer::ClockingMode::Write(
        VGA::Sequencer::ClockingMode::Read() |
        VGA::Sequencer::ClockingMode::ScreenOff);

    //5.7.8.3 (A) Step 2: Set pixclkdis to '1' (disable the pixel clock and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) |
        Shared::Indexed::PixelClockControl::ClockDisable);

    VGA::DACWriteIndex_t pllIndex = Shared::Indexed::Register::PixelPLLMA;

    if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetB)
        pllIndex = Shared::Indexed::Register::PixelPLLMB;
    else if ((PllAndClock & PixelClocksSettings::PLLSet) == PixelClocksSettings::PLLSetC)
        pllIndex = Shared::Indexed::Register::PixelPLLMC;

    //5.7.8.3 (A) Step 3: Re-program the desired pixel PLL registers.
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 0, (mnps >> 8) & Shared::Indexed::PixelPLLM::MValue);
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 1, mnps & Shared::Indexed::PixelPLLN::NValue);
    Shared::PCI::IndexedData::Write(System::s_Device, pllIndex + 2, (mnps  >> 13) &
        (Shared::Indexed::PixelPLLP::PValue | Shared::Indexed::PixelPLLP::SValue));

    Shared::Indexed::PixelClockControl_t clockControl = Shared::Indexed::PixelClockControl::ClockPCI;

    if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockPLL)
        clockControl = Shared::Indexed::PixelClockControl::ClockPLL;
    else if ((PllAndClock & PixelClocksSettings::Clock) == PixelClocksSettings::ClockVDOCLK)
        clockControl = Shared::Indexed::PixelClockControl::ClockVDCLK;

    //5.7.8.3 (A) Step 3 (continued): Select another source for the pixel clock.
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        (Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockSelection) |
        clockControl);

    //5.7.8.3 (A) Step 4: Wait until the clock source is locked onto its new frequency (the pixlock bit is '1')
    //                    for the pixel PLL, or for the VDCLK pin to become stable.
    SYS_ClearInterrupts();
    Sleep2(1);
    SYS_RestoreInterrupts();

    volatile Shared::Indexed::PixelPLLStatus_t frequencyLock = Shared::Indexed::PixelPLLStatus::FrequencyNotLocked;
    uint16_t timeout = 0;
    do
    {
        --timeout;
        if (timeout == 0)
            break;

        frequencyLock = Shared::PCI::Indexed::PixelPLLStatus::Read(System::s_Device) &
            Shared::Indexed::PixelPLLStatus::FrequencyStatus;
    } while (frequencyLock == Shared::Indexed::PixelPLLStatus::FrequencyNotLocked);

    //5.7.8.3 (A) Step 5: Set pixclkdis to '0' (enable the pixel and video clocks)
    Shared::PCI::Indexed::PixelClockControl::Write(System::s_Device,
        Shared::PCI::Indexed::PixelClockControl::Read(System::s_Device) &
        ~Shared::Indexed::PixelClockControl::ClockDisable);

    VGA::Sequencer::ClockingMode::Write(VGA::Sequencer::ClockingMode::Read() & ~VGA::Sequencer::ClockingMode::ScreenOff);
}

bool Set(uint16_t width, uint16_t height, BitsPerPixel_t bpp, Flags_t flags, RefreshRate_t refreshRate, bool clearDisplay)
{
    using namespace Hag::System;

    VideoMode* videoMode = Get(width, height, bpp, flags, refreshRate);
    if (videoMode == NULL)
        return false;

    TurnScreenOff();
    VGA::MiscellaneousOutput::Write(~VGA::MiscellaneousOutput::VideoEnable);
    //Turn monitor off
    CRTCExtension::HorizontalCounterExtensions::Write(
        videoMode->HorizontalCounterExtensions |
        CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff);
    Shared::CRTCExtension::VerticalCounterExtensions::Write(videoMode->VerticalCounterExtensions);

    if (videoMode->FrequencyKHz != 0)
        ConfigurePixelClocks(CalculatePLL_MNPS(videoMode->FrequencyKHz), PixelClocksSettings::PLLSetC | PixelClocksSettings::ClockPLL);

    BDA::VideoModeOptions_t videoModeOptions = BDA::VideoModeOptions::Get();
    if (clearDisplay)
        videoModeOptions &= ~BDA::VideoModeOptions::DontClearDisplay;
    else
        videoModeOptions |= BDA::VideoModeOptions::DontClearDisplay;

    BDA::DetectedHardware::Get() = 
        (BDA::DetectedHardware::Get() &
        ~BDA::DetectedHardware::InitialVideoModeMask) |
        videoMode->ColorOrMonochromeText;

    videoMode = ConfigureEGAFeatureBitSwitchesAdapter(videoMode, videoModeOptions);

    InterruptTable::Pointer<InterruptTable::CharacterTable>() = System::s_Font8x8;
    InterruptTable::Pointer<InterruptTable::GraphicsFont8x8>() = System::s_Font8x8Graphics;

    videoModeOptions &= ~(BDA::VideoModeOptions::Unknown | BDA::VideoModeOptions::Inactive);

    ApplyMode(videoMode, videoModeOptions);

    //Turn monitor on
    CRTCExtension::HorizontalCounterExtensions::Write(
        CRTCExtension::HorizontalCounterExtensions::Read() &
        ~(CRTCExtension::HorizontalCounterExtensions::HorizontalSyncOff |
        CRTCExtension::HorizontalCounterExtensions::VerticalSyncOff));
    TurnScreenOn();

    return true;
}

#ifndef MOCK

bool IsExtendedMode(VideoMode& mode)
{
    return mode.FrequencyKHz != 0;
}

uint8_t CharacterClockInPixels(Hag::System::BDA::VideoParameterTable& table)
{
    //Dot clock select bit is in bit 0.
    uint8_t dotClockSelect = table.SequencerRegisters[VGA::Sequencer::Register::ClockingMode - 1] & VGA::Sequencer::ClockingMode::DotClockSelect;
    //dotClockSelect == 0 -> 9 pixels, 1 -> 8 pixels.
    return dotClockSelect == 0 ? 9 : 8;
}

uint8_t ScanlineDouble(Hag::System::BDA::VideoParameterTable& table)//Returns 0 if no doubling, 1 if there is.
{
    return (table.CRTCRegisters[VGA::CRTController::Register::CRTCModeControl] & VGA::CRTController::CRTCModeControl::VerticalTotalDouble) >> 2;
}

uint32_t HorizontalTotalChars(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint32_t horizontalTotal = table.CRTCRegisters[VGA::CRTController::Register::HorizontalTotal];
    if (IsExtendedMode(mode))
    {
        //Extension bit 8 is in bit 0.
        uint32_t horizontalTotalBit8 = mode.HorizontalCounterExtensions & CRTCExtension::HorizontalCounterExtensions::HorizontalTotal8;
        //Move to bit 8.
        horizontalTotalBit8 <<= 8;
        //Move bit in to place.
        horizontalTotal |= horizontalTotalBit8;
    }
    return horizontalTotal + 5;
}

uint32_t HorizontalTotalPixels(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    return HorizontalTotalChars(mode, table) * CharacterClockInPixels(table);
}

uint32_t HorizontalDisplayEnableEndChars(Hag::System::BDA::VideoParameterTable& table)
{
    return table.CRTCRegisters[VGA::CRTController::Register::HorizontalDisplayEnd] + 1;
}

uint32_t HorizontalDisplayEnableEndPixels(Hag::System::BDA::VideoParameterTable& table)
{
    return HorizontalDisplayEnableEndChars(table) * CharacterClockInPixels(table);
}

uint32_t VerticalTotalLines(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint32_t verticalTotal = table.CRTCRegisters[VGA::CRTController::Register::VerticalTotal];
    //Extension bit 8 is in bit 0.
    uint32_t verticalTotalBit8 = table.CRTCRegisters[VGA::CRTController::Register::CRTCOverflow] & VGA::CRTController::CRTCOverflow::VerticalTotalHigh1;
    //Move to bit 8.
    verticalTotalBit8 <<= 8;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit8;
    //Extension bit 9 is in bit 5.
    uint32_t verticalTotalBit9 = table.CRTCRegisters[VGA::CRTController::Register::CRTCOverflow] & VGA::CRTController::CRTCOverflow::VerticalTotalHigh2;
    //Move to bit 9.
    verticalTotalBit9 <<= 4;
    //Move bit in to place.
    verticalTotal |= verticalTotalBit9;
    if (IsExtendedMode(mode))
    {
        //Extension bits 11 and 10 are in bits 1 and 0.
        uint32_t verticalTotalBits11And10 = mode.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VerticalTotal11_10;
        //Move to bits 11 and 10.
        verticalTotalBits11And10 <<= 10;
        //Move bits in to place.
        verticalTotal |= verticalTotalBits11And10;
    }
    return (verticalTotal + 2) << ScanlineDouble(table);
}

uint32_t VerticalDisplayEnableEndLines(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint32_t verticalDisplayEnd = table.CRTCRegisters[VGA::CRTController::Register::VerticalDisplayEnd];
    //Extension bit 8 is in bit 1.
    uint32_t verticalDisplayEndBit8 = table.CRTCRegisters[VGA::CRTController::Register::CRTCOverflow] & VGA::CRTController::CRTCOverflow::VerticalDisplayEndHigh1;
    //Move to bit 8.
    verticalDisplayEndBit8 <<= 7;
    //Move bit in to place.
    verticalDisplayEnd |= verticalDisplayEndBit8;
    //Extension bit 9 is in bit 6.
    uint32_t verticalDisplayEndBit9 = table.CRTCRegisters[VGA::CRTController::Register::CRTCOverflow] & VGA::CRTController::CRTCOverflow::VerticalDisplayEndHigh2;
    //Move to bit 9.
    verticalDisplayEndBit9 <<= 3;
    //Move bit in to place.
    verticalDisplayEnd |= verticalDisplayEndBit9;
    if (IsExtendedMode(mode))
    {
        //Extension bit 10 is in bit 2.
        uint32_t verticalDisplayEndBit10 = mode.VerticalCounterExtensions & CRTCExtension::VerticalCounterExtensions::VeticalDisplayEnableEnd10;
        //Move to bit 10.
        verticalDisplayEndBit10 <<= 8;
        //Move bit in to place.
        verticalDisplayEnd |= verticalDisplayEndBit10;
    }
    return (verticalDisplayEnd + 1) << ScanlineDouble(table);
}

uint32_t GetFrequencyKHz(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint8_t clockSelect = table.MiscellaneousOutputRegister & VGA::MiscellaneousOutput::ClockSelect;
    if (clockSelect == VGA::MiscellaneousOutput::ClockSelect25p175MHz)
    {
        return 25175;
    }
    else if (clockSelect == VGA::MiscellaneousOutput::ClockSelect28p322MHz)
    {
        return 28322;
    }
    else
    {
        return mode.FrequencyKHz;
    }
}

float RefreshRateHz(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint32_t horizontalTotalPixels = HorizontalTotalPixels(mode, table);
    uint32_t verticalTotalLines = VerticalTotalLines(mode, table);

    float refreshRate = (float(GetFrequencyKHz(mode, table)) / (horizontalTotalPixels * verticalTotalLines)) * 1000.0f;

    return refreshRate;
}

typedef std::string String_t;
typedef std::vector<std::string> StringVector_t;

StringVector_t s_HorizontalTimings;
StringVector_t s_HorizontalTimingNames;

String_t GenerateHorizontalName(uint16_t count, VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    char buf[20];
    if (table.AttributeControllerRegisters[VGA::AttributeController::Register::AttributeMode] & 
        VGA::AttributeController::AttributeMode::SelectGraphicsMode)
    {
        uint32_t width = HorizontalDisplayEnableEndPixels(table);
        sprintf(buf, "H%02ipix%i", count, width);
    }
    else
    {
        uint32_t width = HorizontalDisplayEnableEndChars(table);
        sprintf(buf, "H%02ichar%i", count, width);
    }
    return String_t(buf);
}

String_t GenerateVerticalName(uint16_t count, VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    char buf[20];
    if (table.AttributeControllerRegisters[VGA::AttributeController::Register::AttributeMode] & 
        VGA::AttributeController::AttributeMode::SelectGraphicsMode)
    {
        uint32_t height = VerticalDisplayEnableEndLines(mode, table);
        sprintf(buf, "V%02iline%i", count, height);
    }
    else
    {
        uint16_t charHeight = table.CRTCRegisters[VGA::CRTController::Register::MaximumScanLine] & VGA::CRTController::MaximumScanLine::MaximumScanLineCount;
        uint32_t height = VerticalDisplayEnableEndLines(mode, table) / (charHeight + 1);
        sprintf(buf, "V%02ichar%i", count, height);
    }
    return String_t(buf);
}

String_t GenerateGraphicName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "G%02i", count);

    return String_t(buf);
}

String_t GenerateConfigurationName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "C%02i", count);

    return String_t(buf);
}

String_t GenerateAttributeName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "A%02i", count);

    return String_t(buf);
}

String_t GenerateFontConfigName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "F%02i", count);

    return String_t(buf);
}

String_t GenerateParameterName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "P%02i", count);

    return String_t(buf);
}

String_t GenerateParameterArrayName(uint16_t count)
{
    char buf[10];

    sprintf(buf, "PA%02i", count);

    return String_t(buf);
}

String_t GenerateResolutionName(uint16_t count, VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    char buf[20];
    if (table.AttributeControllerRegisters[VGA::AttributeController::Register::AttributeMode] & 
        VGA::AttributeController::AttributeMode::SelectGraphicsMode)
    {
        uint32_t width = HorizontalDisplayEnableEndPixels(table);
        uint32_t height = VerticalDisplayEnableEndLines(mode, table);
        sprintf(buf, "T%02ipix%ix%i", count, width, height);
    }
    else
    {
        uint32_t width = HorizontalDisplayEnableEndChars(table);
        uint16_t charHeight = table.CRTCRegisters[VGA::CRTController::Register::MaximumScanLine] & VGA::CRTController::MaximumScanLine::MaximumScanLineCount;
        uint32_t height = VerticalDisplayEnableEndLines(mode, table) / (charHeight + 1);
        sprintf(buf, "T%02ichar%ix%i", count, width, height);
    }

    return String_t(buf);
}

uint16_t PrintHorizontalTimings(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table, const char* name)
{
    char buf[100];
    printf("HorizontalTimings %sHorizontalTimings = ", name);

    sprintf(buf, "{ 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X }",
        table.CRTCRegisters[0x00],
        table.CRTCRegisters[0x01],
        table.CRTCRegisters[0x02],
        table.CRTCRegisters[0x03] | 0x80,
        table.CRTCRegisters[0x04],
        table.CRTCRegisters[0x05]);

    printf(buf);
    printf(";\n");

    for (uint16_t idx = 0; idx < s_HorizontalTimings.size(); ++idx)
    {
        if (s_HorizontalTimings[idx] == buf)
            return idx;
    }

    s_HorizontalTimings.push_back(buf);
    s_HorizontalTimingNames.push_back(GenerateHorizontalName(s_HorizontalTimingNames.size(), mode, table));

    return s_HorizontalTimings.size() - 1;
}

StringVector_t s_VerticalTimings;
StringVector_t s_VerticalTimingNames;

uint16_t PrintVerticalTimings(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table, const char* name)
{
    char buf[100];
    printf("VerticalTimings %sVerticalTimings = ", name);

    sprintf(buf, "{ 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X }",
        table.CRTCRegisters[0x06],
        table.CRTCRegisters[0x07],
        table.CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::StartVerticalBlankHigh,
        table.CRTCRegisters[0x10],
        table.CRTCRegisters[0x11] & VGA::CRTController::VerticalRetraceEnd::VerticalRetraceEndCount,
        table.CRTCRegisters[0x12],
        table.CRTCRegisters[0x15],
        table.CRTCRegisters[0x16]);

    printf(buf);
    printf(";\n");

    for (uint16_t idx = 0; idx < s_VerticalTimings.size(); ++idx)
    {
        if (s_VerticalTimings[idx] == buf)
            return idx;
    }

    s_VerticalTimings.push_back(buf);
    s_VerticalTimingNames.push_back(GenerateVerticalName(s_VerticalTimingNames.size(), mode, table));

    return s_VerticalTimings.size() - 1;
}

StringVector_t s_ResolutionTimings;
StringVector_t s_ResolutionTimingNames;

uint16_t PrintResolutionTimings(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table, const char* name, uint16_t horNameIdx, uint16_t verNameIdx)
{
    printf("ResolutionTimings %sTimings = { ", name);

    printf("0x%08X, ", mode.FrequencyKHz);
    printf("0x%02X, ", mode.HorizontalCounterExtensions);
    printf("0x%02X, ", mode.VerticalCounterExtensions);
    printf("0x%02X, ", table.MiscellaneousOutputRegister & (VGA::MiscellaneousOutput::SelectNegativeHorizontalSyncPulse | VGA::MiscellaneousOutput::SelectNegativeVerticalSyncPulse));
    printf("0x%02X, ", table.CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::DoubleScan);
    printf("%sHorizontalTimings, %sVerticalTimings };\n", name, name);

    char buf[150];

    sprintf(buf, "{ 0x%08X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, %s, %s }",
        mode.FrequencyKHz,
        mode.HorizontalCounterExtensions,
        mode.VerticalCounterExtensions,
        table.MiscellaneousOutputRegister & (VGA::MiscellaneousOutput::SelectNegativeHorizontalSyncPulse | VGA::MiscellaneousOutput::SelectNegativeVerticalSyncPulse),
        table.CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::DoubleScan,
        s_HorizontalTimingNames[horNameIdx].c_str(),
        s_VerticalTimingNames[verNameIdx].c_str());

    for (uint16_t idx = 0; idx < s_ResolutionTimings.size(); ++idx)
    {
        if (s_ResolutionTimings[idx] == buf)
            return idx;
    }

    s_ResolutionTimings.push_back(buf);
    s_ResolutionTimingNames.push_back(GenerateResolutionName(s_ResolutionTimings.size(), mode, table));

    return s_ResolutionTimings.size() - 1;
}

StringVector_t s_Graphics;
StringVector_t s_GraphicNames;

uint16_t PrintGraphicsData(Hag::System::BDA::VideoParameterTable& table, const char* name)
{
    char buf[100];
    printf("VGA::GraphicsControllerData_t %sGraphicsControllerConfig[] = ", name);

    sprintf(buf, "{ 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X }",
        table.GraphicsControllerRegisters[0x00],
        table.GraphicsControllerRegisters[0x01],
        table.GraphicsControllerRegisters[0x02],
        table.GraphicsControllerRegisters[0x03],
        table.GraphicsControllerRegisters[0x04],
        table.GraphicsControllerRegisters[0x05],
        table.GraphicsControllerRegisters[0x06],
        table.GraphicsControllerRegisters[0x07],
        table.GraphicsControllerRegisters[0x08]);

    printf(buf);
    printf(";\n");

    for (uint16_t idx = 0; idx < s_Graphics.size(); ++idx)
    {
        if (s_Graphics[idx] == buf)
            return idx;
    }

    s_Graphics.push_back(buf);
    s_GraphicNames.push_back(GenerateGraphicName(s_GraphicNames.size()));

    return s_Graphics.size() - 1;
}

StringVector_t s_Configurations;
StringVector_t s_ConfigurationNames;

uint16_t PrintConfiguration(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table, const char* name, uint16_t gfxNameIdx)
{
    printf("Configuration %sConfig  = { ", name);
    printf("0x%02X, ", table.NumCharacterColumns);
    printf("0x%02X, ", table.NumScreenRowsMinus1);
    printf("0x%02X, ", table.CharacterMatrixHeightInPoints);
    printf("0x%04X, ", (mode.FrequencyKHz == 0) ? table.VideoBufferSize : 0);
    printf("0x%02X, { ", table.MiscellaneousOutputRegister & ~(VGA::MiscellaneousOutput::SelectNegativeHorizontalSyncPulse | VGA::MiscellaneousOutput::SelectNegativeVerticalSyncPulse));
    printf("0x%02X, ", table.SequencerRegisters[0]);
    printf("0x%02X, ", table.SequencerRegisters[1]);
    printf("0x%02X, ", table.SequencerRegisters[2]);
    printf("0x%02X, }, ", table.SequencerRegisters[3]);
    printf("0x%02X, ", table.CRTCRegisters[0x08]);
    printf("0x%02X, ", table.CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::LineCompareHigh);
    printf("0x%02X, ", table.CRTCRegisters[0x11] & ~VGA::CRTController::VerticalRetraceEnd::VerticalRetraceEndCount);
    printf("0x%02X, ", table.CRTCRegisters[0x14]);
    printf("0x%02X, ", table.CRTCRegisters[0x17]);
    printf("0x%02X, ", table.CRTCRegisters[0x18]);
    printf("%sGraphicsControllerConfig, ", name);
    printf("0x%02X, ", table.AttributeControllerRegisters[0x10]);
    printf("0x%02X, ", table.AttributeControllerRegisters[0x11]);
    printf("0x%02X, ", table.AttributeControllerRegisters[0x12]);
    printf("0x%02X };\n", table.AttributeControllerRegisters[0x13]);

    char buf[200];

    sprintf(buf, "{ 0x%02X, 0x%02X, 0x%02X, 0x%04X, 0x%02X, { 0x%02X, 0x%02X, 0x%02X, 0x%02X, }, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, %s, 0x%02X, 0x%02X, 0x%02X, 0x%02X }",
        table.NumCharacterColumns,
        table.NumScreenRowsMinus1,
        table.CharacterMatrixHeightInPoints,
        (mode.FrequencyKHz == 0) ? table.VideoBufferSize : 0,
        table.MiscellaneousOutputRegister & ~(VGA::MiscellaneousOutput::SelectNegativeHorizontalSyncPulse | VGA::MiscellaneousOutput::SelectNegativeVerticalSyncPulse),
        table.SequencerRegisters[0],
        table.SequencerRegisters[1],
        table.SequencerRegisters[2],
        table.SequencerRegisters[3],
        table.CRTCRegisters[0x08],
        table.CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::LineCompareHigh,
        table.CRTCRegisters[0x11] & ~VGA::CRTController::VerticalRetraceEnd::VerticalRetraceEndCount,
        table.CRTCRegisters[0x14],
        table.CRTCRegisters[0x17],
        table.CRTCRegisters[0x18],
        s_GraphicNames[gfxNameIdx].c_str(),
        table.AttributeControllerRegisters[0x10],
        table.AttributeControllerRegisters[0x11],
        table.AttributeControllerRegisters[0x12],
        table.AttributeControllerRegisters[0x13]);

    for (uint16_t idx = 0; idx < s_Configurations.size(); ++idx)
    {
        if (s_Configurations[idx] == buf)
            return idx;
    }

    s_Configurations.push_back(buf);
    s_ConfigurationNames.push_back(GenerateConfigurationName(s_ConfigurationNames.size()));

    return s_Configurations.size() - 1;
}

StringVector_t s_Attributes;
StringVector_t s_AttributeNames;

uint16_t PrintAttributeData(Hag::System::BDA::VideoParameterTable& table, const char* name)
{
    char buf[150];
    printf("VGA::AttributeControllerData_t %sAttributePalette[] = ", name);
    sprintf(buf, "{ 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X }",
        table.AttributeControllerRegisters[0x00],
        table.AttributeControllerRegisters[0x01],
        table.AttributeControllerRegisters[0x02],
        table.AttributeControllerRegisters[0x03],
        table.AttributeControllerRegisters[0x04],
        table.AttributeControllerRegisters[0x05],
        table.AttributeControllerRegisters[0x06],
        table.AttributeControllerRegisters[0x07],
        table.AttributeControllerRegisters[0x08],
        table.AttributeControllerRegisters[0x09],
        table.AttributeControllerRegisters[0x0A],
        table.AttributeControllerRegisters[0x0B],
        table.AttributeControllerRegisters[0x0C],
        table.AttributeControllerRegisters[0x0D],
        table.AttributeControllerRegisters[0x0E],
        table.AttributeControllerRegisters[0x0F]);

    printf(buf);
    printf(";\n");

    for (uint16_t idx = 0; idx < s_Attributes.size(); ++idx)
    {
        if (s_Attributes[idx] == buf)
            return idx;
    }

    s_Attributes.push_back(buf);
    s_AttributeNames.push_back(GenerateAttributeName(s_AttributeNames.size()));

    return s_Attributes.size() - 1;
}

StringVector_t s_FontConfigs;
StringVector_t s_FontConfigNames;

uint16_t PrintFontConfig(ParameterFontPair& paramFont, const char* name)
{
    printf("FontConfiguration %sFont = { Data::%s, 0xFF, ", name,
        (paramFont.Font == Data::Font8x8) ? "Font8x8" : "Font8x16");
    printf("0x%02X, ", paramFont.CharacterHeight);
    printf("0x%02X, ", paramFont.BankIndex);
    printf("%s, ", paramFont.Patch ? "true" : "false");
    printf("0x%02X, ", paramFont.VideoParameters->CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::MaximumScanLineCount);
    printf("0x%02X, ", paramFont.VideoParameters->CRTCRegisters[0x0A]);
    printf("0x%02X };\n", paramFont.VideoParameters->CRTCRegisters[0x0B]);

    char buf[100];

    sprintf(buf, "{ Data::%s, 0xFF, 0x%02X, 0x%02X, %s, 0x%02X, 0x%02X, 0x%02X }",
        (paramFont.Font == Data::Font8x8) ? "Font8x8" : "Font8x16",
        paramFont.CharacterHeight,
        paramFont.BankIndex,
        paramFont.Patch ? "true" : "false",
        paramFont.VideoParameters->CRTCRegisters[0x09] & VGA::CRTController::MaximumScanLine::MaximumScanLineCount,
        paramFont.VideoParameters->CRTCRegisters[0x0A],
        paramFont.VideoParameters->CRTCRegisters[0x0B]);

    for (uint16_t idx = 0; idx < s_FontConfigs.size(); ++idx)
    {
        if (s_FontConfigs[idx] == buf)
            return idx;
    }

    s_FontConfigs.push_back(buf);
    s_FontConfigNames.push_back(GenerateFontConfigName(s_FontConfigNames.size()));

    return s_FontConfigs.size() - 1;
}

StringVector_t s_Parameters;
StringVector_t s_ParameterNames;

uint16_t PrintVideoParameters(const char* name, uint16_t resNameIdx, uint16_t cfgNameIdx, uint16_t atrNameIdx, uint16_t fntNameIdx)
{
    printf("VideoParameters %sVideoParameters = { %sTimings, %sConfig, %sAttributePalette, %sFont };\n", name, name, name, name, name);

    char buf[100];
    sprintf(buf, "{ %s, %s, %s, %s }",
        s_ResolutionTimingNames[resNameIdx].c_str(),
        s_ConfigurationNames[cfgNameIdx].c_str(),
        s_AttributeNames[atrNameIdx].c_str(),
        s_FontConfigNames[fntNameIdx].c_str());

    for (uint16_t idx = 0; idx < s_Parameters.size(); ++idx)
    {
        if (s_Parameters[idx] == buf)
            return idx;
    }

    s_Parameters.push_back(buf);
    s_ParameterNames.push_back(GenerateParameterName(s_ParameterNames.size()));

    return s_Parameters.size() - 1;
}

StringVector_t s_ParameterArrays;
StringVector_t s_ParameterArrayNames;

uint16_t PrintVideoParametersArray(const char* name, bool multi, uint16_t* parameterNameIndices)
{
    char buf[100];
    if (multi)
    {
        printf("VideoParameters* %sParameters[] = { &%sSL200VideoParameters, &%sSL350VideoParameters, &%sSL400VideoParameters, &%sSL480VideoParameters };\n",
            name, name, name, name, name);

        sprintf(buf, "{ &%s, &%s, &%s, &%s }",
            s_ParameterNames[parameterNameIndices[0]].c_str(),
            s_ParameterNames[parameterNameIndices[1]].c_str(),
            s_ParameterNames[parameterNameIndices[2]].c_str(),
            s_ParameterNames[parameterNameIndices[3]].c_str());
    }
    else
    {
        printf("VideoParameters* %sParameters[] = { &%sVideoParameters };\n", name, name);
        sprintf(buf, "{ &%s }",
            s_ParameterNames[parameterNameIndices[0]].c_str());
    }

    for (uint16_t idx = 0; idx < s_ParameterArrays.size(); ++idx)
    {
        if (s_ParameterArrays[idx] == buf)
            return idx;
    }

    s_ParameterArrays.push_back(buf);
    s_ParameterArrayNames.push_back(GenerateParameterArrayName(s_ParameterArrayNames.size()));

    return s_ParameterArrays.size() - 1;
}

struct ModeToName
{
    uint16_t mode;
    const char* name;
};

ModeToName modeToName[] = 
{
    { 0x00, "T40x25x4bppG" },
    { 0x01, "T40x25x4bppC" },
    { 0x02, "T80x25x4bppG" },
    { 0x03, "T80x25x4bppC" },
    { 0x04, "G320x200x2bppC" },
    { 0x05, "G320x200x2bppG" },
    { 0x06, "G640x200x1bppM" },
    { 0x07, "T80x25x1bppM" },
    { 0x0D, "G320x200x4bppC" },
    { 0x0E, "G640x200x4bppC" },
    { 0x0F, "G640x350x1bppM" },
    { 0x10, "G640x350x4bppC" },
    { 0x11, "G640x480x1bppM" },
    { 0x12, "G640x480x4bppC" },
    { 0x13, "G320x200x8bppC" },
    { 0x100, "G640x400x8bpp" },
    { 0x101, "G640x480x8bpp" },
    { 0x102, "G800x600x4bpp" },
    { 0x103, "G800x600x8bpp" },
    { 0x104, "G1024x768x4bpp" },
    { 0x105, "G1024x768x8bpp" },
    { 0x106, "G1280x1024x4bpp" },
    { 0x107, "G1280x1024x8bpp" },
    { 0x108, "T80x60x4bpp" },
    { 0x109, "T132x25x4bpp" },
    { 0x10A, "T132x43x4bpp" },
    { 0x10B, "T132x50x4bpp" },
    { 0x10C, "T132x60x4bpp" },
    { 0x10D, "G320x200x15bpp" },
    { 0x10E, "G320x200x16bpp" },
    { 0x10F, "G320x200x32bpp" },
    { 0x110, "G640x480x15bpp" },
    { 0x111, "G640x480x16bpp" },
    { 0x112, "G640x480x32bpp" },
    { 0x113, "G800x600x15bpp" },
    { 0x114, "G800x600x16bpp" },
    { 0x115, "G800x600x32bpp" },
    { 0x116, "G1024x768x15bpp" },
    { 0x117, "G1024x768x16bpp" },
    { 0x118, "G1024x768x32bpp" },
    { 0x119, "G1280x1024x15bpp" },
    { 0x11A, "G1280x1024x16bpp" },
    { 0x11B, "G1280x1024x32bpp" },
    { 0x11C, "G1600x1200x8bpp" },
    { 0x11D, "G1600x1200x15bpp" },
    { 0x11E, "G1600x1200x16bpp" },
    { 0x11F, "G1600x1200x32bpp" },
    { 0x121, "G640x400x15bpp" },
    { 0x122, "G640x400x16bpp" },
    { 0x124, "G640x400x32bpp" },
    { 0x123, "G640x400x24bpp" },
    { 0x1B3, "G512x384x32bpp" },
    { 0x1B5, "G400x300x32bpp" },
    { 0x1B8, "G320x240x32bpp" },
    { 0x1D4, "G512x384x16bpp" },
    { 0x1D6, "G400x300x16bpp" },
    { 0x1D9, "G320x240x16bpp" },
    { 0x1E4, "G512x384x15bpp" },
    { 0x1E6, "G400x300x15bpp" },
    { 0x1E9, "G320x240x15bpp" },
    { 0x1F3, "G512x384x8bpp" },
    { 0x1F5, "G400x300x8bpp" },
    { 0x1F8, "G320x240x8bpp" },
    { 0x1F9, "G320x200x8bpp" },
};

void PrintDescriptor(VideoMode& mode, const char* name)
{
    printf("    { // Mode %s\n", name);
    printf("        %i,\n", mode.Width);
    printf("        %i,\n", mode.Height);
    printf("        BitsPerPixel::Bpp%i,\n", mode.Bpp);
    printf("        ");
    printf(((mode.Flags & Flags::Mode) == Flags::Text) ? "Flags::Text | " : "Flags::Graphics | ");
    printf(((mode.Flags & Flags::Chromacity) == Flags::Color) ? "Flags::Color | " : "Flags::Monochrome | ");
    printf(((mode.Flags & Flags::MemoryOrganization) == Flags::Sequential) ? "Flags::Sequential | " : "Flags::Planar | ");
    printf(((mode.Flags & Flags::LinearFramebuffer) == Flags::LinearFramebuffer) ? "Flags::LinearFramebuffer | " : "");
    printf(((mode.Flags & Flags::ParameterCount) == Flags::SingleParameter) ? "Flags::SingleParameter | " : "Flags::MultiParameter | ");
    printf(((mode.Flags & Flags::MAFCSelection) == Flags::MAFCDisable) ? "Flags::MAFCDisable,\n" : "Flags::MAFCVGA,\n");
    printf("        RefreshRate::R%iHz,\n", mode.RefreshRate);

    printf("        ");
    bool videoModeFound = false;
    for (uint32_t i = 0; i < sizeof(modeToName) / sizeof(ModeToName); ++i)
    {
        if (mode.LegacyMode == modeToName[i].mode)
        {
            videoModeFound = true;
            if (mode.LegacyMode >= 0x100)
            {
                printf("Hag::VGA::VideoMode::%s,\n", modeToName[i].name);
            }
            else
            {
                printf("Hag::Vesa::VideoMode::%s,\n", modeToName[i].name);
            }

            break;
        }
    }

    if (!videoModeFound)
    {
        printf("0x%02X,\n", mode.LegacyMode);
    }

    printf("        0x%04X,\n", mode.Segment);

    if (mode.CrtController == Hag::VGA::Register::CRTControllerIndexD)
    {
        printf("        Hag::VGA::Register::CRTControllerIndexD,\n");
    }
    else
    {
        printf("        Hag::VGA::Register::CRTControllerIndexB,\n");
    }

    if (mode.ColorOrMonochromeText == Hag::System::BDA::DetectedHardware::Color80x25)
    {
        printf("        Hag::System::BDA::DetectedHardware::Color80x25,\n");
    }
    else
    {
        printf("        Hag::System::BDA::DetectedHardware::Monochrome80x25,\n");
    }

    switch(mode.Scanlines)
    {
    case Scanlines::Invalid:
        printf("        Scanlines::Invalid,\n");
        break;
    case Scanlines::S200:
        printf("        Scanlines::S200,\n");
        break;
    case Scanlines::S350:
        printf("        Scanlines::S350,\n");
        break;
    case Scanlines::S400:
        printf("        Scanlines::S400,\n");
        break;
    case Scanlines::S480:
        printf("        Scanlines::S480,\n");
        break;
    }

    if (mode.CRTModeControlRegValue == 0xFF)
    {
        printf("        0xFF");
    }
    else
    {
        bool firstPrinted = false;
        printf("        ");
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text) != 0)
        {
            printf("Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Monochrome) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Monochrome");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::VideoEnabled) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::VideoEnabled");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Blinking) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Blinking");
            firstPrinted = true;
        }
        if (!firstPrinted)
        {
            printf("0x00");
        }
    }
    printf(",\n");

    printf("        0x%02X,\n", mode.CGAColorPaletteMaskSetting);

    printf("        ");
    if (mode.Palettes == PalettePair0)
    {
        printf("Data::PalettePair0,\n");
    }
    else if (mode.Palettes == PalettePair2)
    {
        printf("Data::PalettePair2,\n");
    }
    else if (mode.Palettes == PalettePair4)
    {
        printf("Data::PalettePair4,\n");
    }
    else if (mode.Palettes == PalettePair578)
    {
        printf("Data::PalettePair578,\n");
    }
    else
    {
        printf("NULL,\n");
    }

    printf("        Mode%sParameters\n    },\n", name);
}

void PrintDescriptor2(VideoMode& mode, const char* name, const char* parameterArrayName)
{
    printf("    { // Mode %s\n", name);
    printf("        %i,\n", mode.Width);
    printf("        %i,\n", mode.Height);
    printf("        BitsPerPixel::Bpp%i,\n", mode.Bpp);
    printf("        ");
    printf(((mode.Flags & Flags::Mode) == Flags::Text) ? "Flags::Text | " : "Flags::Graphics | ");
    printf(((mode.Flags & Flags::Chromacity) == Flags::Color) ? "Flags::Color | " : "Flags::Monochrome | ");
    printf(((mode.Flags & Flags::MemoryOrganization) == Flags::Sequential) ? "Flags::Sequential | " : "Flags::Planar | ");
    printf(((mode.Flags & Flags::LinearFramebuffer) == Flags::LinearFramebuffer) ? "Flags::LinearFramebuffer | " : "");
    printf(((mode.Flags & Flags::ParameterCount) == Flags::SingleParameter) ? "Flags::SingleParameter | " : "Flags::MultiParameter | ");
    printf(((mode.Flags & Flags::MAFCSelection) == Flags::MAFCDisable) ? "Flags::MAFCDisable,\n" : "Flags::MAFCVGA,\n");
    printf("        RefreshRate::R%iHz,\n", mode.RefreshRate);

    printf("        ");
    bool videoModeFound = false;
    for (uint32_t i = 0; i < sizeof(modeToName) / sizeof(ModeToName); ++i)
    {
        if (mode.LegacyMode == modeToName[i].mode)
        {
            videoModeFound = true;
            if (mode.LegacyMode >= 0x20)
            {
                printf("Hag::VGA::VideoMode::%s,\n", modeToName[i].name);
            }
            else
            {
                printf("Hag::Vesa::VideoMode::%s,\n", modeToName[i].name);
            }

            break;
        }
    }

    if (!videoModeFound)
    {
        printf("0x%02X,\n", mode.LegacyMode);
    }

    printf("        0x%04X,\n", mode.Segment);

    if (mode.CrtController == Hag::VGA::Register::CRTControllerIndexD)
    {
        printf("        Hag::VGA::Register::CRTControllerIndexD,\n");
    }
    else
    {
        printf("        Hag::VGA::Register::CRTControllerIndexB,\n");
    }

    if (mode.ColorOrMonochromeText == Hag::System::BDA::DetectedHardware::Color80x25)
    {
        printf("        Hag::System::BDA::DetectedHardware::Color80x25,\n");
    }
    else
    {
        printf("        Hag::System::BDA::DetectedHardware::Monochrome80x25,\n");
    }

    switch(mode.Scanlines)
    {
    case Scanlines::Invalid:
        printf("        Scanlines::Invalid,\n");
        break;
    case Scanlines::S200:
        printf("        Scanlines::S200,\n");
        break;
    case Scanlines::S350:
        printf("        Scanlines::S350,\n");
        break;
    case Scanlines::S400:
        printf("        Scanlines::S400,\n");
        break;
    case Scanlines::S480:
        printf("        Scanlines::S480,\n");
        break;
    }

    if (mode.CRTModeControlRegValue == 0xFF)
    {
        printf("        0xFF");
    }
    else
    {
        bool firstPrinted = false;
        printf("        ");
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text) != 0)
        {
            printf("Hag::System::BDA::CRTModeControlRegValue::Mode2Or3Text");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Mode4Or5Graphics");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Monochrome) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Monochrome");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::VideoEnabled) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::VideoEnabled");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::GraphicsOperation");
            firstPrinted = true;
        }
        if ((mode.CRTModeControlRegValue & Hag::System::BDA::CRTModeControlRegValue::Blinking) != 0)
        {
            if (firstPrinted)
            {
                printf(" | ");
            }
            printf("Hag::System::BDA::CRTModeControlRegValue::Blinking");
            firstPrinted = true;
        }
        if (!firstPrinted)
        {
            printf("0x00");
        }
    }
    printf(",\n");

    printf("        0x%02X,\n", mode.CGAColorPaletteMaskSetting);

    printf("        ");
    if (mode.Palettes == PalettePair0)
    {
        printf("Data::PalettePair0,\n");
    }
    else if (mode.Palettes == PalettePair2)
    {
        printf("Data::PalettePair2,\n");
    }
    else if (mode.Palettes == PalettePair4)
    {
        printf("Data::PalettePair4,\n");
    }
    else if (mode.Palettes == PalettePair578)
    {
        printf("Data::PalettePair578,\n");
    }
    else
    {
        printf("NULL,\n");
    }

    printf("        %s\n    },\n", parameterArrayName);
}

uint16_t GetOffset(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint16_t offset = table.CRTCRegisters[VGA::CRTController::Register::ScreenOffset];
    if (mode.FrequencyKHz != 0)
    {
        if (mode.Bpp == BitsPerPixel::Bpp32)
        {
            offset <<= 1;
        }
        if (mode.Bpp == BitsPerPixel::Bpp8)
        {
            offset >>= 1;
        }
    }
    return offset;
}

uint16_t CalculateOffset(VideoMode& mode, Hag::System::BDA::VideoParameterTable& table)
{
    uint16_t screenWidthChars = table.CRTCRegisters[VGA::CRTController::Register::HorizontalDisplayEnd] + 1;

    if ((mode.FrequencyKHz != 0) && ((mode.Flags & Flags::Mode) != Flags::Text) && (mode.Bpp >= BitsPerPixel::Bpp8))
    {
        BitsPerPixel_t bpp = mode.Bpp;
        if (bpp == BitsPerPixel::Bpp15)
            bpp = BitsPerPixel::Bpp16;
            
        return uint16_t((screenWidthChars * 8 * bpp) / 128);
    }
    else
    {
        uint8_t everySecond = table.SequencerRegisters[0] & VGA::Sequencer::ClockingMode::LoadSerializersEverySecondCharacterClock;
        uint8_t everyFourth = table.SequencerRegisters[0] & VGA::Sequencer::ClockingMode::LoadSerializersEveryFourthCaracterClock;
        uint8_t charWidth = (everyFourth != 0) ? 4 : ((everySecond != 0) ? 2 : 1);

        return screenWidthChars / (charWidth * 2);
    }
}

void PrintNewModeSettings()
{
    using namespace Hag::System;

    char name[100];
    char subname[50];
    std::vector<uint16_t> parameterArrayNameIndices;

    for (uint32_t i = 0; i < sizeof(s_VideoModes) / sizeof(VideoMode); ++i)
    {
        VideoMode& mode = s_VideoModes[i];

        uint32_t loopCount = 1;

        if ((mode.Flags & Flags::ParameterCount) == Flags::MultiParameter)
            loopCount = 4;

        uint16_t parameterNameIndices[4] = {0,0,0,0};
        for (uint32_t tableIdx = 0; tableIdx < loopCount; ++tableIdx)
        {
            BDA::VideoParameterTable& table = *mode.ParametersAndFonts[tableIdx].VideoParameters;

            uint32_t modeID = mode.LegacyMode;
            if (modeID >= 0x20)
            {
                modeID = (modeID - 0x20) | 0x100;
                sprintf(subname, "%X", modeID);
            }
            else
            {
                if (modeID == 0x14){sprintf(subname, "Y");}
                else if (modeID == 0x15){sprintf(subname, "X");}
                else if (modeID == 0x16){sprintf(subname, "Q");}
                else sprintf(subname, "%X", modeID);
            }

            if (loopCount > 1)
            {
                const char* sl = "200";
                if (tableIdx == 1)
                    sl = "350";
                if (tableIdx == 2)
                    sl = "400";
                if (tableIdx == 3)
                    sl = "480";
                
                sprintf(name, "Mode%sSL%s", subname, sl);
            }
            else
            {
                sprintf(name, "Mode%s", subname);
            }
            printf("\n\n");
            uint16_t horNameIdx = PrintHorizontalTimings(mode, table, name);
            uint16_t verNameIdx = PrintVerticalTimings(mode, table, name);
            uint16_t resNameIdx = PrintResolutionTimings(mode, table, name, horNameIdx, verNameIdx);
            uint16_t gfxNameIdx = PrintGraphicsData(table, name);
            uint16_t cfgNameIdx = PrintConfiguration(mode, table, name, gfxNameIdx);
            uint16_t atrNameIdx = PrintAttributeData(table, name);
            uint16_t fntNameIdx = PrintFontConfig(mode.ParametersAndFonts[tableIdx], name);
            uint16_t parNameIdx = PrintVideoParameters(name, resNameIdx, cfgNameIdx, atrNameIdx, fntNameIdx);
            parameterNameIndices[tableIdx] = parNameIdx;
            sprintf(name, "Mode%s", subname);
        }
        parameterArrayNameIndices.push_back(PrintVideoParametersArray(name, loopCount > 1, parameterNameIndices));
    }

    printf("\nModeDescriptor s_Descriptors[] =\n{\n");
    for (uint32_t i = 0; i < sizeof(s_VideoModes) / sizeof(VideoMode); ++i)
    {
        VideoMode& mode = s_VideoModes[i];

        uint32_t modeID = mode.LegacyMode;
        if (modeID >= 0x20)
        {
            modeID = (modeID - 0x20) | 0x100;
            sprintf(name, "%X", modeID);
        }
        else
        {
            if (modeID == 0x14){sprintf(name, "Y");}
            else if (modeID == 0x15){sprintf(name, "X");}
            else if (modeID == 0x16){sprintf(name, "Q");}
            else sprintf(name, "%X", modeID);
        }
        PrintDescriptor(mode, name);
    }
    printf("};\n");

    for (uint32_t i = 0; i < sizeof(s_VideoModes) / sizeof(VideoMode); ++i)
    {
        VideoMode& mode = s_VideoModes[i];

        uint32_t loopCount = 1;

        if ((mode.Flags & Flags::ParameterCount) == Flags::MultiParameter)
            loopCount = 4;

        for (uint32_t tableIdx = 0; tableIdx < loopCount; ++tableIdx)
        {
            BDA::VideoParameterTable& table = *mode.ParametersAndFonts[tableIdx].VideoParameters;

            uint32_t modeID = mode.LegacyMode;
            if (modeID >= 0x20)
            {
                modeID = (modeID - 0x20) | 0x100;
                sprintf(subname, "%X", modeID);
            }
            else
            {
                if (modeID == 0x14){sprintf(subname, "Y");}
                else if (modeID == 0x15){sprintf(subname, "X");}
                else if (modeID == 0x16){sprintf(subname, "Q");}
                else sprintf(subname, "%X", modeID);
            }

            if (loopCount > 1)
            {
                const char* sl = "200";
                if (tableIdx == 1)
                    sl = "350";
                if (tableIdx == 2)
                    sl = "400";
                if (tableIdx == 3)
                    sl = "480";
                
                sprintf(name, "Mode%sSL%s", subname, sl);
            }
            else
            {
                sprintf(name, "Mode%s", subname);
            }

            /*
            uint16_t offset = GetOffset(mode, table);
            uint16_t calculatedOffset = CalculateOffset(mode, table);
            if (offset != calculatedOffset)
            {
                printf("\n%s\n", name);
                printf("raw stored       : 0x%02X = %i\n", table.CRTCRegisters[VGA::CRTController::Register::ScreenOffset], table.CRTCRegisters[VGA::CRTController::Register::ScreenOffset]);
                printf("Stored offset    : 0x%02X = %i\n", offset, offset);
                printf("Calculated offset: 0x%02X = %i\n", calculatedOffset, calculatedOffset);
            }
            */
        }
    }

    printf("\n\n\n//Horizontal timings:\n\n");
    for (StringVector_t::iterator
         itr = s_HorizontalTimings.begin(),
         nitr = s_HorizontalTimingNames.begin();
         (itr != s_HorizontalTimings.end()) &&
         (nitr != s_HorizontalTimingNames.end());
         ++itr, ++nitr)
    {
        printf("HorizontalTimings %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Vertical timings:\n\n");
    for (StringVector_t::iterator
        itr = s_VerticalTimings.begin(),
        nitr = s_VerticalTimingNames.begin();
        (itr != s_VerticalTimings.end()) &&
        (nitr != s_VerticalTimingNames.end());
        ++itr, ++nitr)
    {
        printf("VerticalTimings %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Resolution timings:\n\n");
    for (StringVector_t::iterator
        itr = s_ResolutionTimings.begin(),
        nitr = s_ResolutionTimingNames.begin();
        (itr != s_ResolutionTimings.end()) &&
        (nitr != s_ResolutionTimingNames.end());
        ++itr, ++nitr)
    {
        printf("ResolutionTimings %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Graphics:\n\n");
    for (StringVector_t::iterator
        itr = s_Graphics.begin(),
        nitr = s_GraphicNames.begin();
        (itr != s_Graphics.end()) &&
        (nitr != s_GraphicNames.end());
        ++itr, ++nitr)
    {
        printf("VGA::GraphicsControllerData_t %s[] = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Attributes:\n\n");
    for (StringVector_t::iterator
        itr = s_Attributes.begin(),
        nitr = s_AttributeNames.begin();
        (itr != s_Attributes.end()) &&
        (nitr != s_AttributeNames.end());
        ++itr, ++nitr)
    {
        printf("VGA::AttributeControllerData_t %s[] = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Configurations:\n\n");
    for (StringVector_t::iterator
        itr = s_Configurations.begin(),
        nitr = s_ConfigurationNames.begin();
        (itr != s_Configurations.end()) &&
        (nitr != s_ConfigurationNames.end());
        ++itr, ++nitr)
    {
        printf("Configuration %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Font Configurations:\n\n");
    for (StringVector_t::iterator
        itr = s_FontConfigs.begin(),
        nitr = s_FontConfigNames.begin();
        (itr != s_FontConfigs.end()) &&
        (nitr != s_FontConfigNames.end());
        ++itr, ++nitr)
    {
        printf("FontConfiguration %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Video Parameters:\n\n");
    for (StringVector_t::iterator
        itr = s_Parameters.begin(),
        nitr = s_ParameterNames.begin();
        (itr != s_Parameters.end()) &&
        (nitr != s_ParameterNames.end());
        ++itr, ++nitr)
    {
        printf("VideoParameters %s = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\n//Video Parameter Arrays:\n\n");
    for (StringVector_t::iterator
        itr = s_ParameterArrays.begin(),
        nitr = s_ParameterArrayNames.begin();
        (itr != s_ParameterArrays.end()) &&
        (nitr != s_ParameterArrayNames.end());
        ++itr, ++nitr)
    {
        printf("VideoParameters* %s[] = %s;\n", nitr->c_str(), itr->c_str());
    }

    printf("\nModeDescriptor s_Descriptors[] =\n{\n");
    for (uint32_t i = 0; i < sizeof(s_VideoModes) / sizeof(VideoMode); ++i)
    {
        VideoMode& mode = s_VideoModes[i];

        uint32_t modeID = mode.LegacyMode;
        if (modeID >= 0x20)
        {
            modeID = (modeID - 0x20) | 0x100;
            sprintf(name, "%X", modeID);
        }
        else
        {
            if (modeID == 0x14){sprintf(name, "X");}
            else if (modeID == 0x15){sprintf(name, "Q");}
            else if (modeID == 0x16){sprintf(name, "80x50");}
            else sprintf(name, "%X", modeID);
        }
        PrintDescriptor2(mode, name, s_ParameterArrayNames[parameterArrayNameIndices[i]].c_str());
    }
    printf("};\n");

}
#endif

}

}}}}