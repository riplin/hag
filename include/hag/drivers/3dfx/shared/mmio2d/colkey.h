//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/2D/colkey.h>

namespace Hag::TDfx::Shared::MMIO2D
{

namespace ColorKey
{

    inline Shared::TwoD::ColorKey_t ReadSourceMin(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceColorKeyMin));
    }

    inline Shared::TwoD::ColorKey_t ReadSourceMax(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceColorKeyMax));
    }

    inline Shared::TwoD::ColorKey_t ReadDestinationMin(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationColorKeyMin));
    }

    inline Shared::TwoD::ColorKey_t ReadDestinationMax(uint8_t* baseAddress)
    {
        return *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationColorKeyMax));
    }

    inline void WriteSourceMin(uint8_t* baseAddress, Shared::TwoD::ColorKey_t value)
    {
        *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceColorKeyMin)) = value;
    }

    inline void WriteSourceMax(uint8_t* baseAddress, Shared::TwoD::ColorKey_t value)
    {
        *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::SourceColorKeyMax)) = value;
    }

    inline void WriteDestinationMin(uint8_t* baseAddress, Shared::TwoD::ColorKey_t value)
    {
        *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationColorKeyMin)) = value;
    }

    inline void WriteDestinationMax(uint8_t* baseAddress, Shared::TwoD::ColorKey_t value)
    {
        *((Shared::TwoD::ColorKey_t volatile *)(baseAddress + Shared::TwoD::Register::Base + Shared::TwoD::Register::DestinationColorKeyMax)) = value;
    }

}

}