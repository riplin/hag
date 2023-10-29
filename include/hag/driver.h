//Copyright 2023-Present riplin

#pragma once

#include <hag/ialloc.h>

namespace Hag
{

    class Driver
    {
    public:
        inline Driver(IAllocator& allocator)
            : m_allocator(allocator)
        {}

        virtual ~Driver();

        void Dispose();

    protected:
        IAllocator& m_allocator;
    };

}
