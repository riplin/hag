//Copyright 2023-Present riplin

#include <hag/factory.h>

//TODO: If there's a way to prevent Watcom from stripping unreferenced code, then this can be eliminated
#include <hag/drivers/s3/factory.h>

namespace Hag
{

Factory* Factory::m_factories = NULL;

Driver* Factory::CreateDriver(IAllocator& allocator)
{
    if (m_factories == NULL)
    {
        m_factories = &S3::Factory::Instance();
    }
    Driver* driver = NULL;
    Factory* factory = m_factories;
    while (factory != NULL)
    {
        driver = factory->InstantiateDriver(allocator);
        if (driver != NULL)
            break;
            
        factory = factory->m_next;
    }

    return driver;
}

}