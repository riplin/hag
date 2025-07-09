//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>

namespace Hag::TDfx::Shared::ThreeD
{

typedef uint32_t Fixed12p4_t;
namespace Fixed12p4
{
    enum
    {
        Value = 0x0000ffff,
        Fraction = 0x0000000f,
        Integer = 0x0000fff0
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Fraction = 0x00,
            Integer = 0x04
        };
    }
}

typedef uint32_t Fixed12p12_t;
namespace Fixed12p12
{
    enum
    {
        Value = 0x00ffffff,
        Fraction = 0x00000fff,
        Integer = 0x00fff000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Fraction = 0x00,
            Integer = 0x0c
        };
    }
}

typedef uint32_t Fixed20p12_t;
namespace Fixed20p12
{
    enum
    {
        Value = 0xffffffff,
        Fraction = 0x00000fff,
        Integer = 0xfffff000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Fraction = 0x00,
            Integer = 0x0c
        };
    }
}

typedef uint32_t Fixed14p18_t;
namespace Fixed14p18
{
    enum
    {
        Value = 0xffffffff,
        Fraction = 0x0003ffff,
        Integer = 0xfffc0000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Fraction = 0x00,
            Integer = 0x12
        };
    }
}

typedef uint32_t Fixed2p30_t;
namespace Fixed2p30
{
    enum
    {
        Value = 0xffffffff,
        Fraction = 0x3fffffff,
        Integer = 0xc0000000
    };

    namespace Shift
    {
        enum
        {
            Value = 0x00,
            Fraction = 0x00,
            Integer = 0x1e
        };
    }
}

}