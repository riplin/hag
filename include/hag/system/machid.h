//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/farptr.h>

namespace Hag::System::BIOS
{

namespace Address
{
    enum
    {
        Segment = 0xF000,
        MachineID = 0xFFFE
    };
}

typedef uint8_t MachineID_t;
namespace MachineID
{
    inline MachineID_t Read() { return *FARPointer(Address::Segment, Address::MachineID).ToPointer<MachineID_t>(); }
}


}