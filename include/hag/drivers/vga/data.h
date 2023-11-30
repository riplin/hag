//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/attribc/data.h>
#include <hag/drivers/vga/crtc/data.h>
#include <hag/drivers/vga/gfxc/data.h>
#include <hag/drivers/vga/sqrc/data.h>

namespace Hag { namespace VGA
{

struct VideoParameters
{
    uint8_t DisplayedCharacterColumns;
    uint8_t DisplayedRowsMinus1;
    uint8_t CharacterMatrixHeightPoints;
    uint16_t VideoBufferSize;
    SequencerData_t SequencerRegisters[4];//SR1-SR4
    MiscellaneousOutput_t MiscOutputRegisterValues;
    CRTControllerData_t CRTCRegisters[25];//CR0-CR18
    AttributeControllerData_t AttributeControllerRegs[20];
    GraphicsControllerData_t GraphicsControllerRegs[9];//GR0-GR8
};

namespace Data
{

extern uint8_t CharacterPatch8x14[];
extern uint8_t CharacterPatch8x16[];
extern uint8_t Characters8x8[];
extern uint8_t Characters8x14[];
extern uint8_t Characters8x16[];

extern uint8_t ColorPalette[];
extern uint8_t MonochromePalette[];
extern uint8_t SecondPalette[];
extern uint8_t Color216Palette[];
extern uint8_t Monochrome64Palette[];
extern uint8_t ColorPalette3[];
extern uint8_t MonochromePalette3[];
extern uint8_t ColorPalette2[];
extern uint8_t MonochromePalette2[];

}

}}
