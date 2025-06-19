//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>

namespace Hag::System::InterruptTable
{

    typedef uint16_t Interrupt_t;

    enum
    {
        GraphicsFont8x8 = 0x1f,
        CharacterTable = 0x43
    };

    template <Interrupt_t intr>
    inline static FARPointer& Pointer() { return *FARPointer(0x0000, intr << 2).ToPointer<FARPointer>(); }

}