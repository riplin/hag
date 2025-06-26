//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/ialloc.h>

#define HAG_3DFX_SYSTEM                                     \
    using namespace Hag::TDfx::Shared::Function::System;

namespace Hag::TDfx::Shared::Function::System
{

    uint32_t GetMemorySize(); // Returns video memory size in bytes

}