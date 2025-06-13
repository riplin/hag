//Copyright 2023-Present riplin

#include <stdlib.h>
#include <support/allocatr.h>

namespace Support
{

void* Allocator::Allocate(uint32_t size)
{
    return malloc(size);
}

void Allocator::Free(void* memory)
{
    free(memory);
}

}
