//Copyright 2025-Present riplin

#pragma once

#include <has/types.h>

#define HAG_MATROX_SYSTEM                                   \
    using namespace Hag::Matrox::Shared::Function::System;

namespace Hag::Matrox::Shared::Function::System
{

    uint32_t GetMemorySize(); // Returns video memory size in bytes

}