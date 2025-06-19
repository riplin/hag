//Copyright 2023-Present riplin

#pragma once

#include <hag/factory.h>

namespace Hag::S3
{

    class Factory : public Hag::Factory
    {
    public:

        static inline Hag::Factory& Instance() { return m_Instance; }
    protected:
        virtual Hag::Driver* InstantiateDriver(Hag::IAllocator& allocator) const;

    private:
        static Factory m_Instance;
    };

}
