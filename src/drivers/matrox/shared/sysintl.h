//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>
#include <hag/system/pci.h>

namespace Hag { namespace Matrox { namespace Shared { namespace Function
{

namespace System
{

extern bool s_Initialized;
extern Hag::System::PCI::Device_t s_Device;
extern uint32_t s_MemorySize;//Memory size in KB
extern FARPointer s_Font8x8;
extern FARPointer s_Font8x8Graphics;
extern FARPointer s_Font8x16;
extern FARPointer s_SystemFont;
extern FARPointer s_SystemFontGraphics;

}

}}}}