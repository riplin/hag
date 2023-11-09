//Copyright 2023-Present riplin

#include <hag/system/pci.h>

namespace Hag { namespace System { namespace PCI
{

void ScanBus(uint8_t bus, ScanBusCallback_t callback, void* context)
{
    for (uint8_t slot = 0; slot < 32; ++slot)
    {
        uint16_t vendorId = GetVendorId(bus, slot, 0);
        if (vendorId == Vendor::Invalid)
            return;

        callback(bus, slot, context);

        uint8_t functionCount = 1;
        if (GetHeaderType(bus, slot, 0) & HeaderType::MultiFunction)
            functionCount = 8;

        for (uint8_t function = 0; function < functionCount; ++function)
        {
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
                    ScanBus(secondaryBus, callback, context);
                }
            }
        }
    }
}

}}}
