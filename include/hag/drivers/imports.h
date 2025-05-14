//Copyright 2025-Present riplin

#pragma once

#define IMPORTNAMESPACEANDTYPE(N, R)        \
typedef N::R ## _t R ## _t;                 \
namespace R                                 \
{                                           \
    using namespace N::R;                   \
}

#define IMPORTNAMESPACEANDTYPEANDSHIFT(N, R)\
typedef N::R ## _t R ## _t;                 \
namespace R                                 \
{                                           \
    using namespace N::R;                   \
    namespace Shift                         \
    {                                       \
        using namespace N::R::Shift;        \
    }                                       \
}

#define IMPORTNAMESPACE(N, n)               \
namespace n                                 \
{                                           \
    using namespace N::n;                   \
}
