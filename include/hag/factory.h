//Copyright 2023-Present riplin

#pragma once

#include <hag/driver.h>
#include <has/ialloc.h>

namespace Hag
{

    class Factory
    {
    public:
        static Driver* CreateDriver(Has::IAllocator& allocator);

    protected:
        virtual Driver* InstantiateDriver(Has::IAllocator& allocator) const = 0;

    private:
        static Factory* m_factories;

        Factory* m_next;
    };

}
