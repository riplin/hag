//Copyright 2025-Present riplin

#pragma once

#ifdef __SSE__
#include <hag/math/vec/vecmath.h>

namespace Hag::Math
{
    using namespace Hag::Math::vec;
}
#else
#include <hag/math/flt/fltmath.h>

namespace Hag::Math
{
    using namespace Hag::Math::flt;
}
#endif