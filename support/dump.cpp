//Copyright 2025-Present riplin

#include <support/dump.h>
#include <hag/system/pci.h>

namespace Support
{

struct ScanContext
{
    FILE* fp;
    FILE* fpbin;
    Device* devices;
    const char* vendorName;
    uint16_t deviceCount;
    uint16_t vendor;
};

bool FindDevices(uint8_t bus, uint8_t slot, uint8_t function, void* context)
{
    using namespace Hag::System;

    ScanContext* ctx = (ScanContext*)context;

    if (PCI::GetVendorId(bus, slot, function) == ctx->vendor)
    {
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "\n");

        uint16_t deviceID = PCI::GetDeviceId(bus, slot, function);
        bool found = false;
        for (uint16_t deviceIdx = 0; deviceIdx < ctx->deviceCount; ++deviceIdx)
        {
            if (ctx->devices[deviceIdx].DeviceID == deviceID)
            {
                found = true;
                if (ctx->fp != nullptr)
                    fprintf(ctx->fp, "%s:\n", ctx->devices[deviceIdx].Name);
                break;
            }
        }
        if (!found)
        {
            if (ctx->fp != nullptr)
                fprintf(ctx->fp, "Unknown %s device 0x%04X:\n", ctx->vendorName, deviceID);
        }
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "Bus: 0x%02X, Slot: 0x%02X, Function: 0x%02X\n", bus, slot, function);
        for (uint8_t pciRegIdx = 0; pciRegIdx < 64; ++pciRegIdx)
        {
            uint32_t pciReg = PCI::Read32(bus, slot, function, pciRegIdx << 2);
            if (ctx->fp != nullptr)
                fprintf(ctx->fp, "0x%02X: 0x%02X, 0x%02X, 0x%02X, 0x%02X,    0x%08lX\n",
                        pciRegIdx,
                        uint8_t(pciReg),
                        uint8_t(pciReg >> 8),
                        uint8_t(pciReg >> 16),
                        uint8_t(pciReg >> 24),
                        pciReg);
            if (ctx->fpbin != nullptr)
            {
                fwrite(&pciReg, sizeof(pciReg), 1, ctx->fpbin);
            }
        }
        if (ctx->fp != nullptr)
            fprintf(ctx->fp, "\n");
    }
    return true;
}

void PCIDump(FILE* fptext, FILE* fpbin, uint16_t vendor, const char* vendorName, Device* devices, uint16_t deviceCount)
{
    ScanContext context;
    context.fp = fptext;
    context.fpbin = fpbin;
    context.devices = devices;
    context.deviceCount = deviceCount;
    context.vendor = vendor;
    context.vendorName = vendorName;

    Hag::System::PCI::ScanBus(0, FindDevices, &context);
}

}