//Copyright 2025-Present riplin

#pragma once

#include <hag/drivers/3dfx/shared/regs/3D/3dbase.h>

namespace Hag::TDfx::Shared::ThreeD
{

namespace Register
{
    enum
    {
        TrexInit1 = 0x320
    };
}

// trexInit1 Register
// The trexInit1 register is used for hardware initialization and configuration of the TREX portion of H3.

// Bit     Name                    Description
// 0       rsv_sl_int_slave        reserved
// 1       rsv_sl_int_en           reserved
// 6:2     ft_FIFO_sil             FBI-to-TREX interface FIFO stall input level. Free space level at which stall 
//                                 signal is sent back to transmitting chip.
// 10:7    tt_FIFO_sil             TREX-to-TREX interface FIFO stall input level. Free space level at which
//                                 stall signal is sent back to transmitting chip.
// 11      reserved
// 15:12   rsv_tf_ck_del_adj       reserved
// 16      rsv_rg_ttcii_inh        reserved
// 17      rsv_use_rg_ttcii_inh    reserved
// 18      send_config             Send config. Transmit configuration to FBI through the tf_ interface instead texel data. 0=normal, 1=send.
// 19      reset_FIFOs             Reset all of the FIFO’s inside TREX. 0=run, 1=assert the reset signal.
// 20      reset_graphics          Reset all of the graphics inside TREX. 0=run, 1=assert the reset signal.
// 22:21   rsv_palette_del         reserved
// 25:23   send_config_sel         Send config select. (not revision 0) Selects which data to transmit to FBI when
//                                 send_config==1.
//                                 000=reserved
//                                 001=reserved
//                                 010=reserved
//                                 011=trexInit1,
//                                 100=texBaseAddr[31:0], (for this function, 32 bits are retained and is non-maskable)
//                                 101,110,111=reserved.
// 26      use_4bit_st_frac        1=use 4 bits for s,t instead of 8. Default = 0.
// 27      a_attr_set_only         1=use only the A set of triangle attributes. Default = 0.
// 28      nop_per_tri             1=insert a nop per triangle. Default = 0.
// 29      always_cache_inv        1=always cache invalidate each triangle. Default = 0.
// 30      always_4texel_needed    1=always indicate that 4 texels are needed for each pixel. Default = 0.

// send_config
// It is possible to read trexInit1 and texBaseAddr through the the send_config path, which sends these
// registers over to the FBI section of H3 via the graphics tf bus. When send_config = 1,
// tf_data[31:0] = {a[7:0], r[7:0], g[7:0], b[7:0]}. TREX’s TC/TCA must be set to pass c_other.

typedef uint32_t TrexInit1_t;
namespace TrexInit1
{
    enum
    {
        FbiToTrexStallInputLevel = 0x0000007c, // 6:2 ft_FIFO_sil FBI-to-TREX interface FIFO stall input level. Free space level at which stall 
                                               //     signal is sent back to transmitting chip.

        TrexToTrexStallInputLevel = 0x00000780, // 10:7 tt_FIFO_sil TREX-to-TREX interface FIFO stall input level. Free space level at which
                                                // stall signal is sent back to transmitting chip.

        SendConfig = 0x00040000, // 18 send_config Send config. Transmit configuration to FBI through the tf_ interface instead texel data. 0=normal, 1=send.

        FifoStatus = 0x00080000, // 19 reset_FIFOs Reset all of the FIFO’s inside TREX. 0=run, 1=assert the reset signal.
        FifoRun = 0x00000000,
        FifoReset = 0x00080000,

        GraphicsStatus = 0x00100000, // 20 reset_graphics Reset all of the graphics inside TREX. 0=run, 1=assert the reset signal.
        GraphicsRun = 0x00000000,
        GraphicsReset = 0x00100000,

        SendConfigSelection = 0x03800000, // 25:23 send_config_sel Send config select. (not revision 0) Selects which data to transmit to FBI when
                                          // send_config==1.
                                          // 011=trexInit1,
                                          // 100=texBaseAddr[31:0], (for this function, 32 bits are retained and is non-maskable)
        SendTrexInit1 = 0x01800000,
        SendTexBaseAddr = 0x02000000,

        STFractionSize = 0x04000000, // 26      use_4bit_st_frac        1=use 4 bits for s,t instead of 8. Default = 0.
        STFraction8bit = 0x00000000,
        STFraction4bit = 0x04000000,

        UseAAttributesOnly = 0x08000000, // 27      a_attr_set_only         1=use only the A set of triangle attributes. Default = 0.

        InsertNopPerTriangle = 0x10000000, // 28      nop_per_tri             1=insert a nop per triangle. Default = 0.

        InvalidateCachePerTriangle = 0x20000000, // 29      always_cache_inv        1=always cache invalidate each triangle. Default = 0.

        Load4TexelsPerPixel = 0x400000000 // 30      always_4texel_needed    1=always indicate that 4 texels are needed for each pixel. Default = 0.
    };

    namespace Shift
    {
        enum
        {
            FbiToTrexStallInputLevel = 0x02,
            TrexToTrexStallInputLevel = 0x07,
            SendConfig = 0x12,
            FifoStatus = 0x13,
            GraphicsStatus = 0x14,
            SendConfigSelection = 0x17,
            STFractionSize = 0x1a,
            UseAAttributesOnly = 0x1b,
            InsertNopPerTriangle = 0x1c,
            InvalidateCachePerTriangle = 0x1d,
            Load4TexelsPerPixel = 0x1e
        };
    }
}

}