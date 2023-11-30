//Copyright 2023-Present riplin

#include <memory>
#include <hag/drivers/s3/factory.h>
#include <hag/drivers/s3/hardware.h>

#include <stdio.h>

namespace Hag { namespace S3
{

Factory Factory::m_Instance;

Hag::Driver* Factory::InstantiateDriver(Hag::IAllocator& allocator) const
{
    Hag::Driver* driver = NULL;
    Shared::ChipId_t chipId = Hardware::IdentifyChip();

    if (chipId == Shared::ChipId::Trio64 ||
        chipId == Shared::ChipId::Trio64x)
    {
        //driver = allocator.ConstructWithIAllocator<Trio64::Class>();
    }
    
    return driver;
}


}}
