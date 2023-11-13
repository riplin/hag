//Copyright 2023-Present riplin

#include <hag/system/pci.h>

namespace Hag { namespace System { namespace PCI
{

static bool ScanBusInternal(uint8_t bus, ScanBusCallback_t callback, void* context)
{
    for (uint8_t slot = 0; slot < 32; ++slot)
    {
        if (GetVendorId(bus, slot, 0) == Vendor::Invalid)
            continue;

        if (!callback(bus, slot, 0, context))
            return false;

        uint8_t functionCount = 1;
        if (GetHeaderType(bus, slot, 0) & HeaderType::MultiFunction)
            functionCount = 8;

        for (uint8_t function = 1; function < functionCount; ++function)
        {
            if (GetVendorId(bus, slot, 0) == Vendor::Invalid)
                continue;
            
            if (!callback(bus, slot, function, context))
                return false;

            uint8_t pciClass = GetClass(bus, slot, function);
            uint8_t subClass = GetSubClass(bus, slot, function);
            if (pciClass == Class::BridgeDevice &&
                subClass == BridgeDevice::PCItoPCIBridgeA)
            {
                uint8_t headerType = GetHeaderType(bus, slot, function);
                if ((headerType & HeaderType::MultiFunctionMask) ==
                    HeaderType::PCItoPCI)
                {
                    uint8_t secondaryBus = GetHeader1SecondaryBus(bus, slot, function);
                    if (!ScanBusInternal(secondaryBus, callback, context))
                        return false;
                }
            }
        }
    }
    return true;
}

void ScanBus(uint8_t bus, ScanBusCallback_t callback, void* context)
{
    ScanBusInternal(bus, callback, context);
}

struct FindDeviceContext
{
    uint16_t VendorId;
    uint16_t DeviceId;
    uint16_t Bus;
    uint16_t Slot;
    uint16_t Function;
};

bool FindDeviceCallback(uint8_t bus, uint8_t slot, uint8_t function, void* ctx)
{
    FindDeviceContext& context = *(FindDeviceContext*)ctx;
    if (context.VendorId == GetVendorId(bus, slot, function) &&
        context.DeviceId == GetDeviceId(bus, slot, function))
    {
        context.Bus = bus;
        context.Slot = slot;
        context.Function = function;
        return false;
    }

    return true;
}

bool FindDevice(uint16_t vendorId, uint16_t deviceId, uint8_t& bus, uint8_t& slot, uint8_t& function)
{
    FindDeviceContext context = { vendorId, deviceId, 0xFF, 0xFF, 0xFF };
    ScanBus(0, FindDeviceCallback, &context);

    bus = context.Bus;
    slot = context.Slot;
    function = context.Function;
    return context.Bus != 0xFF;
}

}}}
