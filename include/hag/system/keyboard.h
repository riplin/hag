//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>

namespace Hag::System::KB
{

namespace Register
{
    enum
    {
        PortB = 0x61,
        PortXT = 0x62
    };
}

typedef uint8_t Register_t;

inline uint8_t Read(Register_t reg) { return SYS_ReadPortByte(reg); }
inline void Write(Register_t reg, uint8_t value) { SYS_WritePortByte(reg, value); }

typedef uint8_t PortB_t;
namespace PortBCommand
{
    enum
    {
        Timer2GateSpeakerEnable = 0x01,
        SpeakerEnable = 0x02,
        RamParityCheckDisable = 0x04,
        IOChanParityCheckDisable = 0x08,
        IRQ1Reset = 0x80
    };

    inline void Write(PortB_t value) { KB::Write(Register::PortB, value); }
}

namespace PortBStatus
{
    enum
    {
        Timer2ClkGateSpeakerStatus = 0x01,
        SpeakerStatus = 0x02,
        NMIParityCheckStatus = 0x04,
        NMIIOChanCheckStatus = 0x08,
        LowerNibbleMask = 0x0f,
        ToggleRefreshRequest = 0x10,
        MirrorTimer2OutputCond = 0x20,
        IOChannelParityError = 0x40,
        RAMParityError = 0x80
    };

    inline PortB_t Read() { return KB::Read(Register::PortB); }
}

}