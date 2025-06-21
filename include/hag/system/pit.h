//Copyright 2025-Present riplin

#pragma once

#include <hag/types.h>
#include <hag/system/sysasm.h>

namespace Hag::System::PIT
{

extern void Sleep(uint8_t count);//count * 8 * 1193 ticks time
extern void MiniSleep();//1193 ticks time

namespace Register
{
    enum
    {
        Channel0Data = 0x40,
        Channel1Data = 0x41,
        Channel2Data = 0x42,
        ModeCommand = 0x43
    };
}
typedef uint8_t Register_t;
typedef uint8_t ChannelData_t;
namespace Data
{

    inline ChannelData_t Read(Register_t reg) { return SYS_ReadPortByte(reg); }
    inline void Write(Register_t reg, ChannelData_t value) { SYS_WritePortByte(reg, value); }

    inline ChannelData_t ReadChannel0() { return Read(Register::Channel0Data); }
    inline ChannelData_t ReadChannel1() { return Read(Register::Channel1Data); }
    inline ChannelData_t ReadChannel2() { return Read(Register::Channel2Data); }

    inline void WriteChannel0(ChannelData_t value) { Write(Register::Channel0Data, value); }
    inline void WriteChannel1(ChannelData_t value) { Write(Register::Channel1Data, value); }
    inline void WriteChannel2(ChannelData_t value) { Write(Register::Channel2Data, value); }
}

typedef uint8_t Command_t;
namespace Command
{
    enum
    {
        CountStyle = 0x01,
        CountBinary = 0x00,
        CountBCD = 0x01,
        CounterMode = 0x0E,
        ModeZeroDetectionInterrupt = 0x00,
        ModeProgrammableOneShot = 0x02,
        ModeRateGenerator = 0x04,
        ModeSquareWaveGenerator = 0x06,     // Counts down twice by two at a time; latch status and check
                                            // value of OUT pin to determine which half-cycle is active
                                            // divisor factor 3 not allowed!
        ModeRateGenerator2 = 0x0C,
        ModeSquareWaveGenerator2 = 0x0E,    // Counts down twice by two at a time; latch status and check
                                            // value of OUT pin to determine which half-cycle is active
                                            // divisor factor 3 not allowed!
        ModeSoftwareTriggeredStrobe = 0x08,
        ModeHardwareTriggeredStrobe = 0x0A,
        CounterAccess = 0x30,
        CounterLatch = 0x00,                // Bugged on Intel Neptune/Mercury/Aries Chipset 8237IB
        LowByteOnly = 0x10,
        HighByteOnly = 0x20,
        LowByteHighByte = 0x30,
        ChannelSelect = 0xC0,
        SelectChannel0 = 0x00,
        SelectChannel1 = 0x40,
        SelectChannel2 = 0x80,
        SelectReadBackCommand = 0xC0
    };

    inline void Write(Command_t value) { SYS_WritePortByte(Register::ModeCommand, value); }
}

typedef uint8_t Status_t;
namespace Status
{
    enum
    {
        Selection = 0x07,
        SelectCounter0 = 0x01,
        SelectCounter1 = 0x02,
        SelectCounter2 = 0x04,
        What = 0x18,
        CounterStatusValue = 0x00,
        CounterValue = 0x08,
        CounterStatus = 0x10
    };

    inline Status_t Read() { return SYS_ReadPortByte(Register::ModeCommand); }
}

}