//Copyright 2023-Present riplin

#include <hag/factory.h>

//TODO: If there's a way to prevent Watcom from stripping unreferenced code, then this can be eliminated
#include <hag/drivers/s3/factory.h>

namespace Hag
{

Factory* Factory::m_factories = nullptr;

Driver* Factory::CreateDriver(Has::IAllocator& allocator)
{
    if (m_factories == nullptr)
    {
        m_factories = &S3::Factory::Instance();
    }
    Driver* driver = nullptr;
    Factory* factory = m_factories;
    while (factory != nullptr)
    {
        driver = factory->InstantiateDriver(allocator);
        if (driver != nullptr)
            break;
            
        factory = factory->m_next;
    }

    return driver;
}

}