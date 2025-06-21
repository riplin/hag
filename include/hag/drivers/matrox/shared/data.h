//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/bda.h>
#include <hag/drivers/vga/extmsapi.h>

namespace Hag::Matrox::Shared
{

namespace Data
{

extern uint8_t Font8x8[];
extern uint8_t Font8x16[];

extern uint16_t Font8x8Size;
extern uint16_t Font8x16Size;

extern VGA::Data::PalettePair PalettePair0[];
extern VGA::Data::PalettePair PalettePair2[];
extern VGA::Data::PalettePair PalettePair4[];
extern VGA::Data::PalettePair PalettePair578[];

}

}