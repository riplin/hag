//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag
{

    class IAllocator
    {
    public:
        virtual void* Allocate(uint32_t size) = 0;
        virtual void Free(void* memory) = 0;

        template<typename Type> Type* AllocateAs(uint32_t size)
        {
            return (Type*)Allocate(size);
        }

        template<typename Type>
        Type* Construct()
        {
            void* ptr = Allocate(sizeof(Type));
            if (ptr != nullptr)
            {
                return ::new(ptr) Type();
            }
            return nullptr;
        }

        template<typename Type>
        Type* ConstructWithIAllocator()
        {
            void* ptr = Allocate(sizeof(Type));
            if (ptr != nullptr)
            {
                return ::new(ptr) Type(*this);
            }
            return nullptr;
        }

        template<typename Type>
        void Destruct(Type* obj)
        {
            obj->~Type();
            Free(obj);
        }

        virtual bool AllocDosMem(uint16_t size, uint16_t& selector, uint16_t& segment) = 0;
        virtual bool FreeDosMem(uint16_t selector) = 0;
    };

}
