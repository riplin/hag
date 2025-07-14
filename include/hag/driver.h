//Copyright 2023-Present riplin

#pragma once

#include <has/ialloc.h>

namespace Hag
{

    class Driver
    {
    public:
        inline Driver(Has::IAllocator& allocator)
            : m_allocator(allocator)
        {}

        virtual ~Driver();

        void Dispose();

    protected:
        Has::IAllocator& m_allocator;
    };

}
