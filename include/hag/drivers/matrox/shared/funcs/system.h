//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/ialloc.h>

#define HAG_IMPORT_SYSTEM                        \
    using namespace Shared::Function::System;

namespace Hag::Matrox::Shared::Function
{

namespace System
{
    bool Initialize(IAllocator& allocator);
    void Shutdown();
    uint32_t GetMemorySize(); // Returns video memory size in bytes
}

}