//Copyright 2023-Present riplin

#pragma once

#include <hag/ialloc.h>

namespace Support
{

class Allocator : public Hag::IAllocator
{
public:
	virtual void* Allocate(uint32_t size) override;
	virtual void Free(void* memory) override;

	virtual bool AllocDosMem(uint16_t size, uint16_t& selector, uint16_t& segment) override;
	virtual bool FreeDosMem(uint16_t selector) override;
};

}
