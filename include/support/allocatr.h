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

	//Memory in the first 1MiB area.
	virtual void far* AllocateLow(uint16_t size);
	virtual void FreeLow(void far* memory);
};

}
