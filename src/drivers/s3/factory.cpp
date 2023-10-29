//Copyright 2023-Present riplin

//This file is named differently to prevent its object file overwriting or being overwritten during build.
#include <memory>
#include <hag/drivers/s3/factory.h>
#include <hag/drivers/s3/hardware.h>
#include <hag/drivers/s3/trio.h>

#include <stdio.h>

namespace Hag { namespace S3
{

Factory Factory::m_Instance;

template<Register_t CrtControllerIndex>
Hag::Driver* CreateS3Driver(Hag::IAllocator& allocator)
{
    Hag::Driver* driver = NULL;
    ChipId_t chipId = Hardware::IdentifyChip();

    if (chipId == ChipId::Trio32 ||
        chipId == ChipId::Trio64 ||
        chipId == ChipId::Trio64x)
    {
        driver = allocator.ConstructWithIAllocator<Trio<CrtControllerIndex> >();
    }
    
    return driver;
}

Hag::Driver* Factory::InstantiateDriver(Hag::IAllocator& allocator) const
{
    Driver* driver = NULL;
    if (Hardware::InColorMode())
    {
        driver = CreateS3Driver<VGA::Register::CRTControllerDataD>(allocator);
    } else
    {
        driver = CreateS3Driver<VGA::Register::CRTControllerDataB>(allocator);
    }
    return driver;
}


}}
