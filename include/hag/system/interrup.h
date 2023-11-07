//Copyright 2023-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>

#pragma pack(push, 1);

namespace Hag { namespace System { namespace InterruptTable
{
    typedef uint16_t Interrupt_t;

    enum
    {
        GraphicsFont8x8 = 0x1f,
        CharacterTable = 0x43
    };

    template <Interrupt_t intr>
    inline static FARPointer& Pointers() { return *((FARPointer*)(intr * 4)); }
}}}

#pragma pack(pop);
