//Copyright 2023-Present riplin

#pragma once

#include <hag/driver.h>
#include <hag/ialloc.h>

namespace Hag
{

    class Factory
    {
    public:
        static Driver* CreateDriver(IAllocator& allocator);

    protected:
        virtual Driver* InstantiateDriver(IAllocator& allocator) const = 0;

    private:
        static Factory* m_factories;

        Factory* m_next;
    };

}
