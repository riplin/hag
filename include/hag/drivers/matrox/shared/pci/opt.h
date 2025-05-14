//Copyright 2025-Present riplin

#pragma once

#include <hag/system/pci.h>

namespace Hag { namespace Matrox { namespace Shared { namespace PCI
{

namespace Register
{
    
enum
{
    Option = 0x40
};

}

typedef System::PCI::Register_t Option_t;

namespace Option
{
    enum //Mask
    {
        SystemClockSelection = 0x00000003,              // System clock selection. These bits select the source of the system clock:
        ClockPCI = 0x00000000,                          // 00 = select the PCI clock
        ClockPLL = 0x00000001,                          // 01 = select the output of the system clock PLL
        ClockMCLK = 0x00000002,                         // 10: selects an external source from the MCLK pin (permitted only if MCLK has
                                                        // been configured as an input)
        SystemClockDisable = 0x00000004,                // System clock disable. This bit controls the system clock output:
                                                        // 0 = enable system clock oscillations
                                                        // 1 = disable system clock oscillations
        GraphicsClockDividerSelect = 0x00000008,        // Graphics clock divider select. Selects the ratio by which the system clock is divided in
                                                        // order to produce the graphics clock when sysclksl = ‘01’.
        GraphicsClockDivideBy3 = 0x00000000,            // 0 = divide by 3
        GraphicsClockDivideBy1 = 0x00000008,            // 1 = divide by 1
        MemoryClockDividerSelect = 0x00000010,          // Memory clock divider select. Selects the ratio by which the system clock is divided in
                                                        // order to produce the memory clock when sysclksl = ‘01’.
        MemoryClockDivideBy2 = 0x00000000,              // 0 = divide by 2
        MemoryClockDivideBy1 = 0x00000000,              // 1 = divide by 1
        SystemPLLPowerDown = 0x00000020,                // System PLL power down
                                                        // 0 = power down
                                                        // 1 = power up
        VGAIOMapEnable = 0x00000100,                    // VGA I/O map enable.
                                                        // 0 = VGA I/O locations are not decoded
                                                        //     (hard reset mode if vgaboot = 0)
                                                        // 1 = VGA I/O locations are decoded
                                                        //     (hard reset mode if vgaboot = 1)
                                                        // On hard reset, the sampled vgaboot strap (VD<13>) will replace the vgaioen value.
                                                        // Note that the MGA control registers and MGA frame buffer map are always enabled
                                                        // for all modes.
        VGAFrameBufferMask = 0x00000E00,                // VGA frame buffer mask. This field allows re-mapping of the VGA frame buffer to the
                                                        // top of memory (values not shown in the table are reserved):
                                                        //  fbmskN  Size    Location
        VGAFB1MB70_7F = 0x00000000,                     //   000     1MB    700000h - 7FFFFFh
        VGAFB2MB60_7F = 0x00000200,                     //   001     2MB    600000h - 7FFFFFh
        VGAFB4MB40_7F = 0x00000600,                     //   011     4MB    400000h - 7FFFFFh
        VGAFB8MB00_7F = 0x00000e00,                     //   111     8MB    000000h - 7FFFFFh
        MemoryConfiguration = 0x00001000,               // Memory configuration. This bit indicates the configuration of the memory chips
                                                        // which comprise the frame buffer (refer to ‘SDRAM/SGRAM Configurations’ on page
                                                        // 6-5 for more information regarding pin configurations). It is used by the memory con-
                                                        // troller to map the addresses according to the following table:
                                                        // memconfig    Banks   Bank Size    Word Size  Total
                                                        //     0          2        128K         32       8MB
                                                        //     0          2        128K         16       4MB
                                                        //     1          2        512K         16       16MB
                                                        //     1          2         1M          8        16MB
                                                        //     1          2         2M          4        16MB
        SplitMode = 0x00002000,                         // Split frame buffer mode. When this field is ‘1’, the 8 MByte frame buffer is
                                                        // divided into two sections:
                                                        // 0 MBytes to (4 MBytes - 1) is the graphics buffer
                                                        // 4 MBytes to (8 MBytes - 1) is the video buffer
        HWPlaneWriteMask = 0x00004000,                  // Hardware plane write mask. This field is used to enable SGRAM special functions.
                                                        // This field must always be set to ‘0’ when SDRAM is used. (when SGRAM is used,
                                                        // software must set hardpwmsk to ‘1’ in order to take advantage of special SGRAM
                                                        // functions).
        DisableSGRamFunctions = 0x00000000,             // 0 = Special SGRAM functions are not available; however, a plane write mask cycle
                                                        //     will be emulated in the MGA-1064SG at a reduced performance level.
        EnableSGRamFunctions = 0x00004000,              // 1 = Special SGRAM functions are enabled, so plane write mask operations will be
                                                        //     performed by the memory (with optimal performance) and block mode operations
                                                        //     are available. Note that hardpwmsk must never be set to ‘1’ when the memory does
                                                        //     not consist of SGRAM
        UnifiedMemory = 0x00008000,                     // Unified memory. On hard reset, the sampled unimem strap (VD<15>) value will
                                                        // replace the value of unimem. The unimem strap must always be set to ‘0’.
        RefreshCounter = 0x000F0000,                    // Refresh counter. Defines the rate of the MGA-1064SG’s memory refresh. Page cycles
                                                        // and co-processor acknowledges will not be interrupted by a refresh request unless a
                                                        // second request is queued (in this case, the refresh request becomes the highest priority
                                                        // after the screen refresh). Since all banks have to be precharged, both queued refreshes
                                                        // will keep this new highest priority.
                                                        // When programming the rfhcnt register, the following rule must be respected:
                                                        // ram refresh period >= (rfhcnt<3:1> * 256 + rfhcnt<0> * 64 + 1) * MCLK period
                                                        // Note that setting rfhcnt to zero halts the memory refresh.
        EEPROMWriteEnable = 0x00100000,                 // EEPROM write enable. When set to 1, a write access to the BIOS EPROM aperture
                                                        // will program that location. When set to 0, write access to the BIOS EPROM aperture
                                                        // has no effect.
        ProductID = 0x1F000000,                         // Product ID. Sampled state of the VD<12:8> pins after a hard reset.
                                                        // These bits are available to help board designers encode their product options so that
                                                        // the software and diagnostics can know which options are installed. (This field could
                                                        // encode the amount of memory, an indication if a writable ROM is present, and so on).
                                                        // These bits do not control hardware within the chip.
        NoRetry = 0x20000000,                           // Retry disable. A ‘1’ disables generation of the retry sequence on the PCI bus (except
                                                        // during a VGA snoop cycle). At this setting, violation of the PCI latency rules may
                                                        // occur.
        BIOSEnable = 0x40000000,                        // BIOS enable. On hard reset, the sampled biosen strap (VD<14>) is loaded into this
                                                        // field.
                                                        // 0 = The ROMBASE space is automatically disabled.
                                                        // 1 = The ROMBASE space is enabled - rombase must be correctly initialized since it
                                                        // contains unpredictable data.
        BigEndian = 0x80000000                          // Power PC mode.
                                                        // 0 = No special swapping is performed. The host processor is assumed to be of little
                                                        // endian type.
                                                        // 1 = Enables byte swapping for the memory range MGABASE1 + 1C00h to
                                                        // MGABASE1 + 1EFFh, as well as MGABASE1 + 2C00h to MGABASE1 + 2DFFh.
                                                        // This swapping allows a big endian processor to access the information in the same
                                                        // manner as a little endian processor.
    };

    namespace Shift
    {
        enum
        {
            SystemClockSelection = 0x00,
            SystemClockDisable = 0x02,
            GraphicsClockDividerSelect = 0x03,
            MemoryClockDividerSelect = 0x04,
            SystemPLLPowerDown = 0x05,
            VGAIOMapEnable = 0x08,
            VGAFrameBufferMask = 0x09,
            MemoryConfiguration = 0x0C,
            SplitMode = 0x0D,
            HWPlaneWriteMask = 0x0E,
            UnifiedMemory = 0x0F,
            RefreshCounter = 0x10,
            EEPROMWriteEnable = 0x14,
            ProductID = 0x18,
            NoRetry = 0x1D,
            BIOSEnable = 0x1E,
            BigEndian = 0x1F
        };
    }

    inline Option_t Read(System::PCI::Device_t device) { return Option_t(System::PCI::Read32(device, Register::Option)); }
    inline System::PCI::Register8_t ReadByte0(System::PCI::Device_t device) { return System::PCI::Read8(device, Register::Option + 0); }
    inline System::PCI::Register8_t ReadByte1(System::PCI::Device_t device) { return System::PCI::Read8(device, Register::Option + 1); }
    inline System::PCI::Register8_t ReadByte2(System::PCI::Device_t device) { return System::PCI::Read8(device, Register::Option + 2); }
    inline System::PCI::Register8_t ReadByte3(System::PCI::Device_t device) { return System::PCI::Read8(device, Register::Option + 3); }
    inline void Write(System::PCI::Device_t device, Option_t value) { System::PCI::Write32(device, Register::Option, value); }
    inline void WriteByte0(System::PCI::Device_t device, uint8_t value) { System::PCI::Write8(device, Register::Option + 0, value); }
    inline void WriteByte1(System::PCI::Device_t device, uint8_t value) { System::PCI::Write8(device, Register::Option + 1, value); }
    inline void WriteByte2(System::PCI::Device_t device, uint8_t value) { System::PCI::Write8(device, Register::Option + 2, value); }
    inline void WriteByte3(System::PCI::Device_t device, uint8_t value) { System::PCI::Write8(device, Register::Option + 3, value); }
}

}}}}