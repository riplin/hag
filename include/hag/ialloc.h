//Copyright 2023-Present riplin

#pragma once

#include <memory>
#include <hag/types.h>

namespace Hag
{

    class IAllocator
    {
    public:
        virtual void* Allocate(uint32_t size) = 0;
        virtual void Free(void* memory) = 0;

        //Memory in the first 1MiB area.
        virtual void far* AllocateLow(uint16_t size) = 0;
        virtual void FreeLow(void far* memory) = 0;

        template<typename Type> Type* AllocateAs(uint32_t size)
        {
            return (Type*)Allocate(size);
        }

        template<typename Type> Type far* AllocateLowAs(uint32_t size)
        {
            return (Type far*)AllocateLow(size);
        }

        template<typename Type>
        Type* Construct()
        {
            void* ptr = Allocate(sizeof(Type));
            if (ptr != NULL)
            {
                return ::new(ptr) Type();
            }
            return NULL;
        }

        template<typename Type>
        Type* ConstructWithIAllocator()
        {
            void* ptr = Allocate(sizeof(Type));
            if (ptr != NULL)
            {
                return ::new(ptr) Type(*this);
            }
            return NULL;
        }

        template<typename Type>
        void Destruct(Type* obj)
        {
            obj->~Type();
            Free(obj);
        }

        template<typename Type>
        Type far* ConstructLow()
        {
            void far* ptr = AllocateLow(sizeof(Type));
            if (ptr != NULL)
            {
                return ::new(ptr) Type();
            }
            return NULL;
        }

        template<typename Type>
        Type far* ConstructLowWithIAllocator()
        {
            void far* ptr = AllocateLow(sizeof(Type));
            if (ptr != NULL)
            {
                return ::new(ptr) Type(*this);
            }
            return NULL;
        }

        template<typename Type>
        void DestructLow(Type far* obj)
        {
            obj->~Type();
            FreeLow(obj);
        }

    };

}
