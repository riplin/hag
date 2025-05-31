//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/bda.h>

namespace Hag { namespace Matrox { namespace Shared
{

namespace Data
{

#pragma pack(push, 1);

struct PaletteData
{
    Hag::System::BDA::VideoDisplayDataArea_t Mask;
    uint8_t Flags;
    uint16_t Count;
    uint8_t Colors[];
};

struct PalettePair
{
    PaletteData* Primary;
    PaletteData* Alternate;
    uint8_t StartIndex;
};

#pragma pack(pop);

extern uint8_t Font8x8[];
extern uint8_t Font8x16[];

extern uint16_t Font8x8Size;
extern uint16_t Font8x16Size;

extern PalettePair PalettePair0[];
extern PalettePair PalettePair2[];
extern PalettePair PalettePair4[];
extern PalettePair PalettePair578[];

}

}}}