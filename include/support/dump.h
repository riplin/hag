//Copyright 2025-Present riplin

#pragma once

#include <stdio.h>
#include <has/types.h>
#include <hag/drivers/vga/regtype.h>

namespace Support
{

struct Device
{
    uint16_t DeviceID;
    const char* Name;
};

extern void PCIDump(FILE* fptext, FILE* fpbin, uint16_t vendor, const char* vendorName, Device* devices, uint16_t deviceCount);
extern void BDADump(FILE* fptext, FILE* fpbin);
extern void VGADump(FILE* fptext, FILE* fpbin, Hag::VGA::Register_t baseIOPort);

}