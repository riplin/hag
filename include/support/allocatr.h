//Copyright 2023-Present riplin

#pragma once

#include <hag/ialloc.h>

namespace Support
{

class Allocator : public Hag::IAllocator
{
public:
	virtual void* Allocate(uint32_t size);
	virtual void Free(void* memory);

};

}
